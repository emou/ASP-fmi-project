<!-- #include FILE="util.inc" -->
<% 
var DB = {
  getRows: function(recordset){
    var rows = [];
    if (recordset.EOF == true) return rows;
    recordset.MoveFirst()
    while (recordset.EOF != true){
      var attributes = {};
      for (var i = 0; i<recordset.Fields.Count; i++){
        var field = recordset.Fields(i);
        attributes[field.name] = field.value;
      }
      rows[rows.length] = attributes;
      recordset.MoveNext();
    }
    return rows;
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
	  this._conn.open("DSN=" + dsn);
	  if(exec_on_start) {
	    this._conn.execute(exec_on_start);
	  }
	}
	else {
	  throw new Error("Could not connect to the database");
	}
};

DB.Connection.prototype = {
  model: function(table_name) {
    return DB.model(this, table_name);
  },

  columns: function(table_name) {
    var rs      = this._conn.OpenSchema(4); // '4' means: return columns
    var rows    = DB.getRows(rs);
    var columns = map(rows, function(i,row){ return new DB.Column(row); });
    if (table_name != undefined)
    columns = filter(columns, function(column){ return column.table_name == table_name; });
    return columns;
  }

};


//Returns a new meta class, representing a database table
DB.model = function(conn, table_name) {
  function klass(attributes) {
      this.update_attributes(attributes);
  }

 klass.table_name = table_name;
 klass.columns    = db.columns(table_name);

 // instance methods
 klass.prototype = {
   init: function(attributes){
   },
   update_attributes: function(attributes){
     for (var key in attributes) this[key] = attributes[key];
   },
   db_attributes: function(){
     var attrs = {};
     for (var i in klass.columns){
       var name = klass.columns[i].name;
       if (name != 'id' && this[name] != null)
         attrs[name] = this[name];
     }
     return attrs;
   },
   save: function(){
     var sql = '';

     if (this.id != null){
       
       // UPDATE
       sql = 'UPDATE ' + klass.table_name + ' SET ';
       each(this.db_attributes(), function(key, value){
         if (key != 'id')
           sql = sql + key + '=' + JSON.stringify(value) + ' ';
       });
       sql = sql + 'WHERE id=' + this.id;

     } else {
     
       // INSERT  
       sql = 'INSERT INTO ' + klass.table_name;
       sql = sql + ' (' + map(this.db_attributes(), function(name,value){ return name; }).join(', ') + ') ';
       sql = sql + 'VALUES (' + map(this.db_attributes(), function(name,value){ return JSON.stringify(value); }).join(', ') + ')';
     
     }
     klass.nonquery(sql); 
   }
 };

 // class functions
 klass.query = function(sql){
   return DB.getRows(klass.db._conn().Execute(sql));
 };
 klass.nonquery = function(sql){
   klass.db._conn().Execute(sql);
 };

 klass.all = function(options){
   var sql = "select * from " + klass.table_name;
 
   if (options != null){
     each(options, function(key, value){
       if (value != null && key != 'limit')
         sql = sql + " WHERE " + key + " = " + JSON.stringify(value);
     });
     
     if (options.limit != null) sql = sql + " LIMIT " + options.limit;
   }

   return map(klass.query(sql), function(i,row){
     return new klass(row);
   });
 };

 klass.first = function(options){
   if (options == null) options = {};
   options.limit = 1;
   return klass.all(options)[0];
 };

 klass.get = function(id){
   return klass.first({ id: id });
 };

 klass.create = function(attributes){
   var model = new klass(attributes);
   model.save();
 };

 klass.count = function(options){
   if (options == null) options = {};
   var rows = klass.query('select count(*) from ' + klass.table_name);
   return rows[0]['count(*)'];
 };

 return klass;
};


%>
