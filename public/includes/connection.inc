<%
  DB.Connection = DB.make_connection("PostgreSQL35W", "SET search_path TO tickets;");

  var db = new DB.Connection();
  db.connect();
%>
