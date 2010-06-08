<!-- #include FILE="db.inc" -->
<%
  db          		= new DB.Connection("PostgreSQL35W", "SET search_path TO tickets;");
  Cart 			= db.model("Cart");
  CartItem 	        = db.model("CartItem");
  Order 		= db.model("Order");
  Courier 	        = db.model("Courier");
  User 			= db.model("User");
  Administrator 	= db.model("Administrator");
  Sport 		= db.model("Sport");
  Event 		= db.model("Event");
  Client 		= db.model("Client");
  TicketCategory      	= db.model("TicketCategory");
  Place 		= db.model("Place");
  Address 	        = db.model("Address");
  OrderItem 	        = db.model("OrderItem");
%>
