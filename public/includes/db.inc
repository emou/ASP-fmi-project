<!-- #include FILE="util.inc" -->
<!-- #include FILE="html.inc" -->
<!-- #include FILE="form.inc" -->

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
        attributes[field.name] = collValue( field.value );
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
        if(k && val) {
            params.push(DB.quote_column(k) + '=' + DB.quote_value(val));
        }
        else {
            throw new Error("Empty parameters while building conditions for query!");
        }
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
    if(typeof(val) == 'string' || val instanceof String || typeof(val) == 'object') {
        return "'" + val + "'";
    }
    return val;
  },

  quote_column: function(col) {
      return DB.quote_table(col);
  },

  select_statement: function(table_name, what, filter, opts) {
        var sql = 'select ' + what + ' from ' + DB.quote_table(table_name) + ' ';
        opts = opts || {};

        if (filter) {
            sql += DB.where_clause(filter, opts);
        }

        if(opts.group_by) {
            sql += " GROUP BY " + DB.quote_column(opts.group_by);
        }

        if(opts.order_by) {
            sql += " ORDER BY " + DB.quote_column(opts.order_by);
            if(opts.order)
                sql += opts.order;
        }

        if (opts.limit) {
            //limit=0 doesn't make any sense
            sql += " LIMIT " + opts.limit;
        }

        return sql;
    },

    delete_statement: function(table_name, filter, opts) {
        var opts = opts || {};
        var sql = 'delete from ' + DB.quote_table(table_name) + ' ';
        sql += DB.where_clause(filter, opts);

        return sql;
    }

};

DB.Column = function(options) {
    this.name = options['COLUMN_NAME'];
    delete options['COLUMN_NAME'];

    for(var k in options)
        this[k] = options[k];

};

DB.Column.prototype.form_field = function(initial_data) {
    var db_type = this.DATA_TYPE;
    var field_opts = {name: this.name, value: initial_data};

    switch( db_type ) {
        case adBoolean:
        case adChar:
            field_opts['type'] = "checkbox";
            break;
        case adDBTimeStamp:
            field_opts['type'] = "timestamp";
            break;
        default:
            field_opts['type'] = "text";
            break;
    }

    return new Field(field_opts);
};

DB.Column.prototype.get_value = function(val) {
        var db_type = this.DATA_TYPE;
        switch( db_type ) {
            case adChar:
                val = val=="1";
                break;
        }
        return val;
};

/* A factory for connections */
DB.make_connection = function(dsn, exec_on_connect) {
    function klass() {}

    /* A table for quick access to the models.
     * Every created model should be added to the index
     * by the model factory function DB.model
     * using the register_model method.
     * */
    klass.model_index = {};

    /* A table for quick access to the primary keys */
    klass.fk_table = {};

    /* The name of the ODBC source */
    klass.dsn = dsn;

    /* A command that is executed on every connect.
     * Usualy setting the schema */
    klass.exec_on_connect = exec_on_connect;

    klass.prototype = {
        connect: function() {
            try {
                this._conn = Server.createObject("ADODB.Connection");
                this._conn.open("DSN=" + klass.dsn);
            }
            catch(e) {
                // Hide sensitive info
                throw new Error("Could not connect to the DB");
            }

            if(this.exec_on_connect) {
                this.execute(klass.exec_on_connect);
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
        },

        register_model: function(model_name, model_class) {
            klass.model_index[model_name] = model_class;
        },

        get_fk_table: function() {
            return klass.fk_table;
        },

        get_model_class: function(name) {
            return klass.model_index[name];
        }
     
    };

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

    /* Well, yeah. It IS perfectly possible! */
    var c = new klass();
    c.connect();

    for(var rs = c.execute(sql); rs.EOF != true; rs.MoveNext()) {
        var t = rs('table_name').value;
        var a = klass.fk_table[t] || [];
        a.push({
                column: rs('column_name').value,
                foreign_column: rs('foreign_column_name').value,
                foreign_table: rs('foreign_table_name').value
              });

        klass.fk_table[t] = a;
    }

    return klass;
}

// A meta class, representing a database table
DB.model = function(db, table_name, opts) {

    if(!table_name) {
        throw new Error("Cannot create model without table_name option!");
    }

    klass.columns    = db.columns(table_name);

    function klass(attributes) {
        for(var i in klass.columns) {
            var c   = klass.columns[i].name;
            this[c] = klass.columns[i].get_value(attributes[c]);
        }
    };

    /* Very permissive */
    for(var i in opts) {
        klass[i] = opts[i];
    }

    klass.db         = db;
    klass.table_name = table_name;
    rs               = klass.db._conn.OpenSchema(28, [null, null, klass.table_name]); // 28 means: return primary keys
    klass.primary_keys = [];
    while (rs.EOF != true) {

      klass.primary_keys.push(rs('column_name').value); // "cast" to string
      rs.MoveNext();
    }
    rs.close();

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

            var cols = new Array();
            var vals = new Array();

            var db_attrs = this.db_attributes();

            for(var k in db_attrs) {
                cols.push(k);
                vals.push(db_attrs[k]); //DB.quote_value(db_attrs[k]));
            }

            var sql;

            if(update || this.exists()) {

                /* This is so much cleaner than using a recordset! */
                sql  = 'update ' + quoted_table_name;
                sql += DB.set_clause(db_attrs);
                sql += DB.where_clause(this.pk_attributes());
                db.execute(sql);
            }
            else {
                var rs = Server.CreateObject("ADODB.Recordset");
                rs.CursorLocation = 3; /* adUseClient. Without this, resync() does not work! */
                rs.Open(quoted_table_name, db._conn, 3, 3, 512);

                rs.AddNew(cols, vals);
                rs.Update();
                this._refresh(rs);

                rs.Close();
            }


        },

        /* Refresh values in the object from a recordset
         * Changes argument rs
         * For internal use only
         *
         * - If the model has a serial (autoincrement, AI) field,
         *   that was specified in the creation of the model,
         *   to refresh the row, we use absolutePosition and Requery().
         *   - If there is a column named 'id', we assume it to be AI
         *     and do the same as above.
         *
         * - If klass.serial is null, we do not make the assumption about
         *   the 'ID' column
         * 
         */
        _refresh: function(rs) { 

            if(klass.serial || (this.id === null && klass.serial === undefined)) {
                var pos = rs.absolutePosition;
                rs.Requery();
                rs.absolutePosition = pos;
            }
            else {
                rs.Resync();
            }

            var cols = klass.columns;
            for(var i in cols) {
                 //debug("Refreshing " + klass.table_name + "(" + cols[i].name + ") to ");
                 this[cols[i].name] = rs.Fields(cols[i].name).value;
                 //debug(this[cols[i].name]);
            }

        },

        /* delete is a keyword in Javascript */
        remove: function() {
            if(!this.exists())
                throw new Error("Object does not exist!");
            
            var sql = DB.delete_statement(klass.table_name, this.pk_attributes());

            klass.db.execute(sql);

        },

        exists: function() {
            /* Get the values for the primary-key attributes */
            var filter=this.pk_attributes();

            /* Check if there are rows with those primary-key values */
            return klass.exists(filter);
        },

        /* Returns the associated records in a many-to-one relationship for the many side.
         * For example:
         *      book.associated('Author');
         * Does *not* support different roles (or multiple foreign keys for a single role) at the moment,
         * but it would be easy to add this just by returning a 2D array of rows: one array per role.
         * WARNING: This function is potentionally dangerous, as it uses eval.
         * */
        associated: function(table_name) {

            table_name = table_name.capitalize();
            var fks = klass.db.get_fk_table()[klass.table_name];
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
            var fks = klass.db.get_fk_table()[table_name];
            var model_klass = eval(table_name);

            for(var i in fks) {
                var fk = fks[i];
                if(fk.foreign_table == klass.table_name) {
                    var values = {};
                    values[fk.column] = this[fk.foreign_column];
                    return model_klass.all(values);
                }
            }

            throw new Error("No foreign key found from table " + table_name + " to table " + klass.table_name + "!");
        },

        toString: function() {
            return this.name;
        },

        debug: function() {
            var str = Object.prototype.toString.call(this, '(', ')');
            return klass.table_name + str;
        },

        html_link: function(content) {
            return new Tag('a', { href: klass.table_name + '.asp?' + this.http_key() }, content || this.name || this);
        },

        http_key: function() {
            var params = [];
            var pk_attrs = this.pk_attributes();

            for(var k in pk_attrs) {
                params.push(k + '=' + encodeURIComponent(pk_attrs[k]) );
            }

            return params.join('&amp;');

        },

        admin_buttons: function() {
            var http_key = this.http_key();
            var buttons = "";

            buttons+=( new Tag('a', {href: '/admin/edit.asp?object_type=' + klass.table_name + '&amp;' + http_key, 'class': 'edit'}, 'Редакция') );

            var pks = this.pk_attributes();
            var field_defs = {};
            for(var k in pks) {
                field_defs[k] = {name: k, type: 'hidden', value: pks[k], label: null, id: null};
            }

            var delete_form = new Form({action: '/admin/delete.asp?object_type=' + klass.table_name, submit: 'Изтриване', 'class': 'delete'},
                                    field_defs);

            buttons+=( delete_form );

            return buttons;
        }

    };

    klass.query = function(sql) {
        return DB.getRows(klass.db.execute(sql));
    };

    // class methods
    klass.model_query = function(sql) {
        return DB.getRows(klass.db.execute(sql), klass);
    };

    klass.nonquery = function(sql) {
        klass.db.execute(sql);
    };

    klass.all = function(filter, opts) {
        opts = opts || {};
        opts.order_by  = opts.order_by || klass.order_by;
        opts.order  = opts.order || klass.order;

        var what = opts.fetch_only || '*';

        if(what.join) {
            what = what.join(',');
        }

        var sql = DB.select_statement(klass.table_name, what, filter, opts);

        return klass.model_query(sql);
    };

    klass.first = function(filter, opts) {
        if(empty(filter)) {
            throw new Error(klass.table_name + ": empty filter!");
        }
        opts = opts || {};
        opts.limit = 1;
        return klass.all(filter, opts)[0];
    };

    klass.get = function(pks) {
        fst = klass.first(pks);
        return fst;
    };

    klass.create = function(attributes) {
        var model = new klass(attributes);
        if(klass.table_name == "CartItem") {
            var debug = [];
            for(var i in attributes) {
                debug.push("\n" + i.length);
                debug.push("\n" + ("TicketCategory_id" == i));
                debug.push(i + " = " + attributes[i]);
            }
            throw new Error(new Number(attributes["TicketCategory_id"]));
        }
        model.save();
        return model;
    };

    klass.count = function(filter, opts) {
        opts = opts || {};
        if(empty(filter))
            return 0;
        var sql = 'select count(*) from ' + DB.quote_table(klass.table_name);
        sql += DB.where_clause(filter, opts);

        var rows = klass.query(sql);

        return rows[0]['count'];
    };

    klass.exists = function(filter) {
        return klass.count(filter)>0;
    };

    /* Register the model on the index */
    db.register_model(klass.table_name, klass);

    return klass;
};

%>
