<!-- #include FILE="settings.inc" -->
<!-- #include FILE="md5.inc" -->
<%

  DB.Connection = DB.make_connection("PostgreSQL35W", "SET search_path TO tickets;");

  var db = new DB.Connection();
  db.connect();


  var Cart              = db.model( "Cart",              { singular: "Количка",                plural: "Колички"              });
  var CartItem          = db.model( "CartItem",          { singular: "Артикул в количка",      plural: "Артикули в количка"   });
  var Order             = db.model( "Order",             { singular: "Поръчка",                plural: "Поръчки"              });
  var OrderItem         = db.model( "OrderItem",         { singular: "Артикул в поръчка",      plural: "Артикули в поръчка"   });
  var Courier           = db.model( "Courier",           { singular: "Куриер",                 plural: "Куриери"              });
  var User              = db.model( "User",              { singular: "Потребител",             plural: "Потребители"          });
  var Administrator     = db.model( "Administrator",     { singular: "Администратор",          plural: "Администратори"       });
  var Sport             = db.model( "Sport",             { singular: "Спорт",                  plural: "Спортове"             });
  var Event             = db.model( "Event",             { singular: "Проявa",                 plural: "Прояви"               });
  var Client            = db.model( "Client",            { singular: "Клиент",                 plural: "Клиенти"              });
  var TicketCategory    = db.model( "TicketCategory",    { singular: "Билет",                  plural: "Билети"               });
  var Place             = db.model( "Place",             { singular: "Спортно съоръжение",     plural: "Спортни съоръжения"   });
  var Address           = db.model( "Address",           { singular: "Адрес",                  plural: "Адреси"               });

  /* options */
  Event.order_by = 'start';
  Event.order    = 'ASC';

  TicketCategory.order_by = 'price';


  /* Returns a hashed password.
   * Adds a string for better security (even if someone knows the secret);
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

  Client.prototype.find_cart = function() {
       var filter = {Client_User_email: this.User_email};
       return Cart.get(filter) || Cart.create(filter);
  };

  User.prototype.toString = function() {
      return this.first_name + ' ' + this.last_name;
  };

  User.prototype.is_client = function() {
      return Client.exists({User_email: this.email}); //!this.is_admin;
  };

  User.current = function() {
      if(Session("email")) {
          return User.get({email: Session("email")});
      }
  };

  Administrator.prototype.toString = function() {
      return this.User_email;
  };

  Event.prototype.toString = function() {
      return this.name;
  };

  Place.prototype.toString = function() {
      return this.name;
  };

  TicketCategory.prototype.can_buy = function(n) {
      return n<=this.count;
  };

  Cart.prototype.items = function() {
      return this.associated_set("CartItem");
  };

  Cart.prototype._cart_item_filter = function(ticket) {
      return { Cart_Client_User_email: this.Client_User_email, TicketCategory_id: ticket.id };
  };

  Cart.prototype.find_item = function(ticket) {
      var ci = CartItem.create( this._cart_item_filter(ticket) );
      return ci;
  };

  Cart.prototype.add_item = function(ticket, cnt) {
      var ci = this.find_item(ticket);
      if(ci.associated("TicketCategory").can_buy(ci.count + cnt)) {
          ci.count += cnt;
          ci.save();
      }
  };

  Cart.prototype.remove_item = function(ticket) {
      var ci = CartItem.get( this._cart_item_filter(ticket) );
  };


%>
