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

      rows[rows.length] = attributes;
      recordset.MoveNext();

    }
    return rows;
  },

  where_clause: function(values, opts) {
      op=(opts && opts['or'])?' OR ':' AND ';
      sql='WHERE ';
      params=[];

      for(var k in values) {
        var val=values[k];
        if(k==null || val==null) {
            throw new Error("Empty parameters for a where clause!");
        }
        params.push(k + '=' + DB.quote_value(val));
      }

      return sql + params.join(op);
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
	this._conn = Server.createObject("ADODB.Connection");
	if (this._conn.errors.count==0) {
	  /* throws an exception if cannot connect */
	  this._conn.open("DSN=" + dsn);
	  if(exec_on_start) {
	    this.execute(exec_on_start);
	  }
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

    klass.db  	     = db;
    klass.table_name = table_name;
    klass.columns    = db.columns(table_name);

    rs               = klass.db._conn.OpenSchema(28, [null, null, klass.table_name]); // 28 means: return primary keys
    klass.primary_keys = []
    while (rs.EOF != true) {
      var attributes = {};

      klass.primary_keys.push(' ' + rs('column_name')); // "cast" to string
      rs.MoveNext();
    }

    // instance methods
    klass.prototype = {
        init: function(attributes) {
        },

        update_attributes: function(attributes) {
            for (var key in attributes) this[key] = attributes[key];
        },

        db_attributes: function() {
            var attrs = {};
            for (var i in klass.columns) {
                var name = klass.columns[i].name;
                if (this[name] != null)
                    attrs[name] = this[name];
            }
            return attrs;
        },

        save: function(update) {
            var sql = '';
            var quoted_table_name = DB.quote_table(klass.table_name);
            db_attrs = this.db_attributes();
            cols = [];
            vals = [];

            for(var k in db_attrs) {
                cols.push(k);
                vals.push(DB.quote_value(db_attrs[k]));
            }

            if(update) {
                sql = 'update ' + quoted_table_name + ' where ';// + this.values_for(this.primary_keys);
                throw Error("Update not yet implemented");
            }
            else {
                sql  = 'insert into ' + quoted_table_name;
                sql += DB.tuple(cols) + ' values ' + DB.tuple(vals);
                print(sql);
                //db.execute(sql);
            }
        }

    };

    // class functions
    klass.query = function(sql) {
        return DB.getRows(klass.db.execute(sql));
    };

    klass.nonquery = function(sql) {
        klass.db.execute(sql);
    };

    klass.all = function(filter, opts) {
        var sql = 'select * from "' + klass.table_name + '" ';

        if (filter) {
            sql += DB.where_clause(filter, opts);
        }

        if (opts && opts.limit) {
            //limit=0 doesn't make any sense
            sql += " LIMIT " + opts.limit;
        }

        return map(klass.query(sql), function(i,row) { return new klass(row); });
    };

    klass.first = function(filter, options) {
        options = options || {};
        options.limit = 1;
        return klass.all(filter, options)[0];
    };

    klass.get = function(id) {
        return klass.first({ id: id });
    };

    klass.create = function(attributes) {
        var model = new klass(attributes);
        model.save();
    };

    klass.count = function(options) {
        options = options || {};
        sql = 'select count(*) from ' + DB.quote_table(klass.table_name);
        var rows = klass.query(sql);
        return rows[0]['count'];
    };
    return klass;
};

%>
