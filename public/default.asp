<%@ CODEPAGE=65001 %>
<!-- #include FILE="db.js" -->
<!-- #include FILE="util.js" -->
<%
  var results = Server.createObject("ADODB.Recordset");
  var method = Request.ServerVariables("REQUEST_METHOD");

  if(method=="POST") {
	  Response.write("baba: " + Request.form("value"));
  }
  else if(method=="GET") {
	  print("Това кирилица ли е?");
	  var contacts = DBConnection._connection.execute("select * from contacts;");
	  print("<ul>");
	  var rows = DB.getRows(contacts);
	  for(i in rows) {
	    print("<li>" + rows[i].address1 + "</li>");
	  }

	  var a=rsToArray(Request.ServerVariables);
	  for(i in a) {
	    print("<li>" + i + ": " + a[i] + "</li>");
	  }

	  print("</ul>");
  }


  print('<form action="default.asp" method="POST"><input type="text" name="value"/><input type="button" value="Submit"/></form></body>');
%>
