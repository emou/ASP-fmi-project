<!-- #include FILE="includes/init.inc" -->
<%
    var city = decodeURIComponent(Request.QueryString("city"));
    blocks['title'] = city;

    /* Get the info from a dedicated SQL View with 'where city = ' + city */
    blocks['content'] = "";
%>

<!-- #include FILE="includes/template.inc" -->
