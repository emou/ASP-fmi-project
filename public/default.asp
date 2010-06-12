<!-- #include FILE="init.inc" -->
<!-- #include FILE="md5.inc" -->
<%

  if(method=="POST") {
     var user = User.authenticate('estanchev@mail.ru', 'estanchev');
     debug(user);
  }
  else if(method=="GET") {
//        debug("Users count: " + User.count() + "<br/>");
//        debug("First user's email: " + User.first().email + "<br/>");
//        var user = User.first();
//        user.first_name = 'Emo';
//        debug("Primary keys of CartItem: " + CartItem.primary_keys + "<br/>");
//        user.save();
//        debug("User exists?: " + user.exists() + "<br/>");
//        debug("Authenticate user: " + User.authenticate('estanchev@mail.ru', '').email);
//      user.email = 'Fucker';
//      debug("User exists?: " + user.exists() + "<br/>");
//        all_users = User.all();
//        debug("First user's first name: " + User.first().first_name + "<br/>");
//        debug("hash of foo: " + MD5('foo'));
  }

  debug('<form action="default.asp" method="POST"><input type="text" name="value"/><input type="button" value="Submit"/></form></body>');
%>

<%= flash %>
