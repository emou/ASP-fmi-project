<!-- #include FILE="includes/init.inc" -->
<%
    var content = "";
    var ticket = TicketCategory.get({id: Request.QueryString("id")});
    if(method == "POST") {
        var count = Request.Form("count");

        if( count > ticket.count ) {
            Session("flash") = "Не можете да купите толкова билети";
            Response.redirect("Event.asp?id=" + ticket.Event_id);
        }
    }
    else {
    }

    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
