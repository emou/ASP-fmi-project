<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/client_required.inc" -->
<%
    var content = "";
    content += "<h3>Количка</h3>";

    if(method == "POST") {
        var ticket = TicketCategory.get({id: Request.QueryString("id")});
        if(!ticket)
            Response.redirect("/default.asp");

        var count = new Number( Request.Form("count") );


        if( !ticket.can_buy(count) ) {
            Session("flash") = "Не можете да купите толкова билети";
            Response.redirect("Event.asp?id=" + ticket.Event_id);
        }

        cart.add_item(ticket, count);

        Session("flash") = "Успешно добавихте артикули в количката си!";

        Response.redirect("/Cart.asp");
    }

    var item_table  = new Tag('table', {'class': 'cart_table'});
    item_table.append("<tr><th>Проява</th><th>Цена</th><th>Брой</th></tr>");

    var total = 0;
    var cart_items = cart.items(); // We need a fresh copy

    for(var i in cart_items) {
        /* JOIN much more effective? */
        var ci  = cart_items[i];
        var tc  = ci.associated("TicketCategory");
        var ev  = tc.associated("Event");
        var remove_button = new Form({ action: 'remove.asp?id=' + tc.id, submit: 'Изтрий', 'class': 'remove' });

        item_table.append("<tr><td>"
                        + ev.html_link() + "</td><td>"
                        + format_price(tc.price) + "</td><td>"
                        + ci.count + "</td><td>"
                        + remove_button + "</td></tr>"
                    );
        total += tc.price * ci.count;
    }
    content += item_table;

    content += '<div class="total">Общо: ' + format_price(total) + '</div>';

    content += new Form({ action: 'order.asp', submit: 'Потвърди'},
                        { courier: {type: 'select', 'choices': Courier.all(), label: 'Куриер'} });


    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
