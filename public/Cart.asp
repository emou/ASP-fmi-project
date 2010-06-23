<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/client_required.inc" -->
<%
    var content = "";
    var ticket = TicketCategory.get({id: Request.QueryString("id")});

    if(!ticket || method!="POST") {
        Response.redirect("/default.asp");
    }

    var count = new Number( Request.Form("count") );

    cart.add_item(ticket, count);

    if( !ticket.can_buy(count) ) {
        Session("flash") = "Не можете да купите толкова билети";
        Response.redirect("Event.asp?id=" + ticket.Event_id);
    }


    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
