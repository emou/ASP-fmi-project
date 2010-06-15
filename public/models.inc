<!-- #include FILE="settings.inc" -->
<!-- #include FILE="md5.inc" -->
<%

  var db             = new DB.Connection("PostgreSQL35W",
                                   "SET search_path TO tickets;");
  db.connect();
  var Cart           = db.model("Cart");
  var CartItem       = db.model("CartItem");
  var Order          = db.model("Order");
  var Courier        = db.model("Courier");
  var User           = db.model("User");
  var Administrator  = db.model("Administrator");
  var Sport          = db.model("Sport");
  var Event          = db.model("Event", {order_by: 'start', order: 'ASC'});
  var Client         = db.model("Client");
  var TicketCategory = db.model("TicketCategory", {order_by: 'price'});
  var Place          = db.model("Place");
  var Address        = db.model("Address");
  var OrderItem      = db.model("OrderItem");


  /* Returns a hashed password.
   * Adds a string for better security (even if someone knows the secret, it won't help them)
  */
  User._hash_pass = function(pass) {
    return MD5(Settings.secret + pass + 'bubble' + Settings.secret);
  };

  /* Checks if an email and password combination is a valid login.
   * Returns the corresponding user on success.
   */
  User.authenticate = function(email, pass) {
    var user = User.get({'email': email});
    if( !user.exists() ) {
        throw Error("A user with that email address does not exist!");
    }
    else {
        if ( User._hash_pass(pass) != user.password ) {
            throw Error("Wrong password!"); 
        }
    }
    return user;
  };

  /* Sets the user's password. Does not save the user! */
  User.prototype.set_password = function(pass) {
    this.password = User._hash_pass(pass);
  };

  User.prototype.toString = function() {
      return this.first_name + ' ' + this.family_name;
  };

  User.current = function() {
      if(Session("email")) {
          return User.get({email: Session("email")});
      }
  };

  Event.prototype.toString = function() {
      return this.name;
  };

  Place.prototype.toString = function() {
      return place.name;
  };

%>
