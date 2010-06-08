<%@ CODEPAGE=65001 %>
<!-- #include FILE="models.inc" -->
<%
  var results = Server.createObject("ADODB.Recordset");
  var method = Request.ServerVariables("REQUEST_METHOD");

  if(method=="POST") {
	  print("baba: " + Request.form("value"));
  }
  else if(method=="GET") {
	print("Users count: " + User.count() + "<br/>");
	print("Users first: " + User.first().email + "<br/>");
  }

  print('<form action="default.asp" method="POST"><input type="text" name="value"/><input type="button" value="Submit"/></form></body>');
%>

