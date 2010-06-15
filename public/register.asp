<!-- #include FILE="init.inc" -->
<!-- #include FILE="form.inc" -->
<%
    if(current_user) {
        Session("flash") = "Вие вече сте регистрирани и сте влезли";
        Response.redirect("default.asp");
    }

    var form;
    if(method=="POST") {
        form = new RegisterForm({}, Request);
        if(form.valid()) {
            form.save();
            Session("flash") = "Вие се регистрирахте успешно с " + form.fields.email.get_value() + "!";
            Response.Redirect("default.asp");      
        }
    }
    else {
        form = new ClientForm();
    }

    blocks['content'] = form;
    blocks['title'] = "Регистрация";
%>

<!-- #include FILE="template.inc" -->
