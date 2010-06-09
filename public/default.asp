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
	print("First user's email: " + User.first().email + "<br/>");
	var user = User.first();
	user.first_name = 'Emo';
	print("Primary keys of CartItem: " + CartItem.primary_keys + "<br/>");
	user.save();
	print("First user's first name: " + User.first().first_name + "<br/>");
  }

  print('<form action="default.asp" method="POST"><input type="text" name="value"/><input type="button" value="Submit"/></form></body>');
%>
