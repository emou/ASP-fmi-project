<%@ CODEPAGE=65001 %>
<!-- #include FILE="db.js" -->
<!-- #include FILE="util.js" -->
<%
  var results = Server.createObject("ADODB.Recordset");
  var method = Request.ServerVariables("REQUEST_METHOD");

  if(method=="POST") {
	  print("baba: " + Request.form("value"));
  }
  else if(method=="GET") {
	  print("Това кирилица ли е?");
	  var contacts = DBConnection._connection.execute("select * from contacts;");
	  var rows = DB.getRows(contacts);
	  var rows_html = array_to_html(rows);
	  var server_vars = array_to_html(rsToArray(Request.ServerVariables));
  }

  print('<form action="default.asp" method="POST"><input type="text" name="value"/><input type="button" value="Submit"/></form></body>');
%>

