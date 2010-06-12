<!-- #include FILE="db.inc" -->
<!-- #include FILE="settings.inc" -->
<%
  db             = new DB.Connection("PostgreSQL35W",
                                   "SET search_path TO tickets;");
  Cart           = db.model("Cart");
  CartItem       = db.model("CartItem");
  Order          = db.model("Order");
  Courier        = db.model("Courier");
  User           = db.model("User");
  Administrator  = db.model("Administrator");
  Sport          = db.model("Sport");
  Event          = db.model("Event");
  Client         = db.model("Client");
  TicketCategory = db.model("TicketCategory");
  Place          = db.model("Place");
  Address        = db.model("Address");
  OrderItem      = db.model("OrderItem");


  /* Returns a hashed password.
   * Adds a string for better security (even if someone knows the secret, it won't help them)
  */
  User._hash_pass = function(pass) {
    return MD5(Settings.secret + pass + 'bubble' + Settings.secret);
  }

  /* Checks if an email and password combination is a valid login.
   * Returns the corresponding user on success.
   */
  User.authenticate=function(email, pass) {
    var user = User.get({'email': email});
    if( !user.exists() ) {
        throw Error("A user with that email address does not exist!");
    }
    else {
        if ( User._hash_pass(pass) != user.password )
            throw Error("Wrong password!"); 
        return user;
    }
  }

  /* Set the user's password. Does not save the user! */
  User.prototype.set_password=function(pass) {
    this.password = User._hash_pass(pass);
  }

%>
