<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/client_required.inc" -->
<%

    if(method=="POST" && cart_items.length) {
       var courier_name = Request.Form("courier");
       var order = new Order({ User_email: client.User_email, Courier_name: courier_name });
       order.save(); //expect refreshed

       for(var i in cart_items) {
          var tc = cart_items[i].associated("TicketCategory");
           if(tc.count-cart_items[i].count < 0) {
               Session("flash") = "Междувременно са изкупени билети за " + tc.associated("Event")
                                  + ". Бройката в количката ви беше намалена или билетите премахнати напълно.";
               order.remove();
               if(tc.count>0) {
                   cart_items[i].count = tc.count;
               }
               else {
                   cart_items[i].remove();
               }
           }

           OrderItem.create({
                                    Order_id: order.id,
                                    TicketCategory_id: cart_items[i].TicketCategory_id,
                                    count: cart_items[i].count
                            });


       }

       cart.remove(); // ON DELETE CASCADE

       Session("flash") = "Поръчката ви беше приета успешно!";

       debug(order.debug());
    }

    Response.Redirect("/default.asp");
%>

<!-- #include FILE="includes/template.inc" -->
