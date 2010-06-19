<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/form.inc" -->
<%
    if(current_user) {
        Session("flash") = "Вие вече сте влезли";
        Response.redirect("default.asp");
    }

    var form;
    if(method=="POST") {
        form = new LoginForm({}, collToArray(Request.Form));
        if(form.valid()) {
            Session("email") = form.save().email;
            Session("flash") = "Вие се вписахте успешно с " + Session("email") + "!";
            Response.Redirect("default.asp");      
        }
    }
    else {
        form = new LoginForm();
    }

    blocks['content'] = form;
    blocks['title'] = "Вписване";
%>

<!-- #include FILE="includes/template.inc" -->
