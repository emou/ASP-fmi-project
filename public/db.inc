<!-- #include FILE="util.inc" -->
<!-- #include FILE="html.inc" -->

<% 
var DB = {
  getRows: function(recordset, model) {
    var rows = [];
    if (recordset.EOF == true)
        return rows;

    recordset.MoveFirst()
    while (recordset.EOF != true) {
      var attributes = {};

      for (var i = 0; i<recordset.Fields.Count; i++){
        var field = recordset.Fields(i);
        attributes[field.name] = field.value;
      }

      if(model!=undefined) {
          rows.push(new model(attributes));
      }
      else {
          rows.push(attributes);
      }
      recordset.MoveNext();

    }
    return rows;
  },

  _conditions: function(values, tab) {
      var params=[];
      var prefix=(tab && (tab + ".")) || "";

      for(var k in values) {
        var val=values[k];
        if(k==null || val==null) {
            throw new Error("Empty parameters while building conditions for query!");
        }
        params.push(DB.quote_column(k) + '=' + DB.quote_value(val));
      }

      return params;
  },

  where_clause: function(values, opts) {
      if(!values) return "";
      var op=(opts && opts['or'])?' or ':' and ';
      var sql=' where ';

      return sql + DB._conditions(values).join(op);
  },

  set_clause: function(values, opts) {
        if(!values) return "";
        var sql=' set ';

        return sql + DB._conditions(values).join(', ');
  },

  tuple: function(names) {
    return "(" + names.join() + ")";
  },

  quote_table: function(name) {
    return '"' + name + '"';
  },

  quote_value: function(val) {
    if(typeof(val) == 'string' || val instanceof String) {
        return "'" + val + "'";
    }
    return val;
  },

  quote_column: function(col) {
      return DB.quote_table(col);
  }

};

DB.Column = function(options) {
    this.name = options['column_name'];
};

DB.Connection = function(dsn, exec_on_connect) {

    this.dsn = dsn;
    this.exec_on_connect = exec_on_connect;

    /* Caching */
    this.fk_table = {};

    /* PostgreSQL specific */
    var sql =  "select tc.constraint_name, tc.table_name,\
                kcu.column_name, \
                ccu.table_name AS foreign_table_name, \
                ccu.column_name AS foreign_column_name \
                from information_schema.table_constraints AS tc \
                JOIN information_schema.key_column_usage AS kcu \
                    ON tc.constraint_name = kcu.constraint_name \
                JOIN information_schema.constraint_column_usage AS ccu \
                    ON ccu.constraint_name = tc.constraint_name \
                where constraint_type='FOREIGN KEY'";

    this.connect();

    for(var rs = this.execute(sql); rs.EOF != true; rs.MoveNext()) {
        var t = rs('table_name').value;
        var a = this.fk_table[t] || [];
        a.push({
                column: rs('column_name').value,
                foreign_column: rs('foreign_column_name').value,
                foreign_table: rs('foreign_table_name').value
              });

        this.fk_table[t] = a;
    }

};

DB.Connection.prototype = {
    connect: function() {
        try {
            this._conn = Server.createObject("ADODB.Connection");
            this._conn.open("DSN=" + this.dsn);
        }
        catch(excection) {
            // Hide sensitive info
            throw new Error("Could not connect to the DB");
        }

        if(this.exec_on_connect) {
            this.execute(this.exec_on_connect);
        }
    },


    model: function(table_name, opts) {
        return DB.model(this, table_name, opts);
    },
 
    columns: function(table_name) {
        var rs      = this._conn.OpenSchema(4, [null, null, table_name]); // 4 means: return columns
        return DB.getRows(rs, DB.Column);
    },
 
    execute: function(sql) {
        return this._conn.execute(sql);
    }
 
};

//Returns a new meta class, representing a database table
DB.model = function(db, table_name, opts) {
    function klass(attributes) {
      this.update_attributes(attributes);
    };

    opts = opts || {};

    klass.order = opts['order'];

    klass.db         = db;
    klass.table_name = table_name;
    klass.columns    = db.columns(table_name);
    rs               = klass.db._conn.OpenSchema(28, [null, null, klass.table_name]); // 28 means: return primary keys
    klass.primary_keys = [];
    while (rs.EOF != true) {
      var attributes = {};

      klass.primary_keys.push(rs('column_name').value); // "cast" to string
      rs.MoveNext();
    }

    // instance methods
    klass.prototype = {
        update_attributes: function(attributes) {
            for (var key in attributes) this[key] = attributes[key];
        },

        db_attributes: function(opts) {
            var cols = (opts && opts['pks_only']) ?
                                                  klass.primary_keys :  //only primary keys
                                                  map(klass.columns, function(c) { return klass.columns[c].name }); //all attributes
            var attrs = {};
            for (var i in cols) {
                var attr = cols[i];
                if (this[attr])
                    attrs[attr] = this[attr];
            }
            return attrs;
        },

        pk_attributes: function() {
            return this.db_attributes({'pks_only': true});
        },

        save: function(update) {
            var quoted_table_name = DB.quote_table(klass.table_name);

            var db_attrs = this.db_attributes();
            if(db_attrs == {})
                return null;

            var cols = [];
            var vals = [];

            for(var k in db_attrs) {
                cols.push(k);
                vals.push(DB.quote_value(db_attrs[k]));
            }

            var sql = '';
            if(update || this.exists()) {
                sql = 'update ' + quoted_table_name;
                sql+= DB.set_clause(db_attrs);
                sql+= DB.where_clause(this.pk_attributes());
            }
            else {
                sql = 'insert into ' + quoted_table_name;
                sql+= DB.tuple(cols) + ' values ' + DB.tuple(vals);
            }

            db.execute(sql);
        },

        exists: function() {
            /* Get the values for the primary-key attributes */
            var filter=this.pk_attributes();

            /* Check if there are rows with those primary-key values */
            return klass.count(filter)>0;
        },

        /* Returns the associated records in a many-to-one relationship for the many side.
         * For example:
         *      book.associated('Author');
         * Does *not* support different roles (or multiple foreign keys for a single role) at the moment,
         * but it would be easy to add this just by returning a 2D array of rows: one array per role.
         * */
        associated: function(table_name) {

            table_name = table_name.capitalize();
            var fks = klass.db.fk_table[klass.table_name];
            var model_klass = eval(table_name);

            /* Multiple foreign keys are possible, but *not supported* yet! */
            for(var i in fks) {
                var fk = fks[i];
                if(fk.foreign_table == table_name) {
                    var values = {};
                    values[fk.foreign_column] = this[fk.column];
                    return model_klass.first(values);
                }
            }

            throw new Error("No foreign key found from table " + klass.table_name + " to table " + table_name + "!");
        },

        /* Returns the associated records in a many-to-many relationship.
         * For example:
         *      author.associated_set('Book');
         * where book has a foreign key for one author
         * */
        associated_set: function(table_name) {

            table_name = table_name.capitalize();
            var fks = klass.db.fk_table[table_name];
            var model_klass = eval(table_name);

            for(var i in fks) {
                var fk = fks[i];
                if(fk.foreign_table == klass.table_name) {
                    var values = {};
                    values[fk.column] = this[fk.foreign_column];
                    return model_klass.all(values);
                }
            }

            //throw new Error("No foreign key found from table " + klass.table_name + " to table " + table_name + "!");
        },

        /* For debugging */
        toString: function() {
            var str = Object.prototype.toString.call(this, '(', ')');
            return klass.table_name + str;
        },

        html_link: function(content) {
            var params = [];
            var pk_attrs = this.pk_attributes();

            for(var k in pk_attrs) {
                params.push( k + '=' + pk_attrs[k] );
            }

            return new Tag('a', {href: klass.table_name + '.asp?' + params.join('&')}, content || this.name || this);
        }

    };

    // class methods
    klass.query = function(sql) {
        return DB.getRows(klass.db.execute(sql), klass);
    };

    klass.nonquery = function(sql) {
        klass.db.execute(sql);
    };

    klass.all = function(filter, opts) {
        opts = opts || {};
        var sql = 'select * from ' + DB.quote_table(klass.table_name) + ' ';

        if (filter) {
            sql += DB.where_clause(filter, opts);
        }

        if (opts.limit) {
            //limit=0 doesn't make any sense
            sql += " LIMIT " + opts.limit;
        }

        opts.order  = opts.order || klass.order;
        if(opts.order) {
            sql += " ORDER BY " + DB.quote_column(opts.order);
        }

        return klass.query(sql);
    };

    klass.first = function(filter, opts) {
        opts = opts || {};
        opts.limit = 1;
        return klass.all(filter, opts)[0];
    };

    klass.get = function(pks) {
        fst = klass.first(pks);
        if(!fst)
            throw new Error(klass.table_name + " does not exist!");
        return fst;
    };

    klass.create = function(attributes) {
        var model = new klass(attributes);
        model.save();
    };

    klass.count = function(filter, opts) {
        opts = opts || {};
        var sql = 'select count(*) from ' + DB.quote_table(klass.table_name);
        sql += DB.where_clause(filter, opts);
        var rows = klass.query(sql);
        return rows[0]['count'];
    };

    return klass;
};

%>
