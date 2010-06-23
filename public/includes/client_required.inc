<!-- #include FILE="login_required.inc" -->
<%

    if(!client) {
        Session("flash") = "Вие нямате клиентски профил!";
        Response.redirect("/default.asp");
    }

%>
