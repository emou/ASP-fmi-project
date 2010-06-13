<!-- #include FILE="init.inc" -->
<!-- #include FILE="form.inc" -->
<%
    var form;
    if(method=="POST") {
        form = new LoginForm({}, Request);
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

<!-- #include FILE="template.inc" -->
