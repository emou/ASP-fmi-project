<!-- #include FILE="init.inc" -->
<%
    var ev = TicketCategory.get({id: Request.QueryString("id")});

    var content = "";
    blocks['content'] = content;
%>

<!-- #include FILE="template.inc" -->
