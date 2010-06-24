<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/client_required.inc" -->
<%
    if(method == "POST") {
        var ticket = TicketCategory.get({id: Request.QueryString("id")});

        if(!ticket)
            Response.redirect("default.asp");

        var ci = cart.find_item(ticket);

        if(ci) {
            ci.remove();
            Session("flash") = "Успешно премахнахте артикул от количката си!";
            Response.redirect("Cart.asp");
        }
    }

    Response.redirect("/Cart.asp");

%>

<!-- #include FILE="includes/template.inc" -->
