<!-- #include FILE="util.inc" -->
<% 
var DB = {
  getRows: function(recordset) {
    var rows = [];
    if (recordset.EOF == true) return rows;

    recordset.MoveFirst()
    while (recordset.EOF != true) {
      var attributes = {};

      for (var i = 0; i<recordset.Fields.Count; i++){
        var field = recordset.Fields(i);
        attributes[field.name] = field.value;
      }

      rows.push(attributes);
      recordset.MoveNext();

    }
    return rows;
  },

  _conditions: function(values) {
      var params=[];
      for(var k in values) {
        var val=values[k];
        if(k==null || val==null) {
            throw new Error("Empty parameters while building conditions for query!");
        }
        params.push(k + '=' + DB.quote_value(val));
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
  }

};

DB.Column = function(options) {
  for (var key in options) {
      this[key.toLowerCase().replace(/^column_/, '')] = options[key];
  }
};

DB.Connection = function(dsn, exec_on_start) {
    try {
      this._conn = Server.createObject("ADODB.Connection");
      this._conn.open("DSN=" + dsn);
      if(exec_on_start) {
        this.execute(exec_on_start);
      }
    }
    catch(expection) {
      print("Could not connect to the DB");
      Response.end;
    }
};

DB.Connection.prototype = {
  model: function(table_name) {
    return DB.model(this, table_name);
  },

  columns: function(table_name) {
    var rs      = this._conn.OpenSchema(4, [null, null, table_name]); // 4 means: return columns
    var rows    = DB.getRows(rs);
    var columns = map(rows, function(i,row){ return new DB.Column(row); });
    return columns;
  },

  execute: function(sql) {
    return this._conn.execute(sql);
  }

};

//Returns a new meta class, representing a database table
DB.model = function(db, table_name) {
    function klass(attributes) {
      this.update_attributes(attributes);
    };

    klass.db         = db;
    klass.table_name = table_name;
    klass.columns    = db.columns(table_name);

    rs               = klass.db._conn.OpenSchema(28, [null, null, klass.table_name]); // 28 means: return primary keys
    klass.primary_keys = []
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
            var sql = '';
            var quoted_table_name = DB.quote_table(klass.table_name);

            db_attrs = this.db_attributes();
            if(db_attrs == {})
                return null;

            cols = [];
            vals = [];

            for(var k in db_attrs) {
                cols.push(k);
                vals.push(DB.quote_value(db_attrs[k]));
            }

            if(update || this.exists()) {
                sql = 'update ' + quoted_table_name;
                sql+= DB.set_clause(db_attrs);
                sql+= DB.where_clause(this.pk_attributes());
            }
            else {
                sql  = 'insert into ' + quoted_table_name;
                sql += DB.tuple(cols) + ' values ' + DB.tuple(vals);
            }

            db.execute(sql);
        },

        exists: function() {
            /* Get the values for the primary-key attributes */
            filter=this.pk_attributes();

            /* Check if there are rows with those primary-key values */
            return klass.count(filter)>0;
        },

        toString: function() {
            str=Object.prototype.toString.call(this, '(', ')');
            return klass.table_name + str;
        }


    };

    // class methods
    klass.query = function(sql) {
        return DB.getRows(klass.db.execute(sql));
    };

    klass.nonquery = function(sql) {
        klass.db.execute(sql);
    };

    klass.all = function(filter, opts) {
        var sql = 'select * from ' + DB.quote_table(klass.table_name) + ' ';

        if (filter) {
            sql += DB.where_clause(filter, opts);
        }

        if (opts && opts.limit) {
            //limit=0 doesn't make any sense
            sql += " LIMIT " + opts.limit;
        }

        return map(klass.query(sql), function(i,row) { return new klass(row); });
    };

    klass.first = function(filter, opts) {
        opts = opts || {};
        opts.limit = 1;
        return klass.all(filter, opts)[0];
    };

    klass.get = function(pks) {
        fst = klass.first(pks);
        if(!fst)
            throw Error("User does not exist!");
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
