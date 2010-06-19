<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/form.inc" -->
<%
    if(current_user) {
        Session("flash") = "Вие вече сте регистрирани и сте влезли";
        Response.redirect("default.asp");
    }

    var form;
    if(method=="POST") {
        form = new RegisterForm({}, collToArray(Request.Form));
        if(form.valid()) {
            form.save();
            Session("flash") = "Вие се регистрирахте успешно с " + form.fields.email.get_value() + "!";
            Response.Redirect("default.asp");
        }
    }
    else {
        form = new RegisterForm();
    }

    blocks['content'] = form;
    blocks['title'] = "Регистрация";
%>

<!-- #include FILE="includes/template.inc" -->
