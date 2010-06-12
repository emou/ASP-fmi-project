<!-- #include FILE="init.inc" -->
<!-- #include FILE="form.inc" -->
<%
if(false) {
    var session_email = Session("user_email");
    if (session_email) {
        Session("flash") = "Вече сте се вписали!";
    }
    else {
        try {
            user = User.authenticate(Form('email'), Form('password'));
        }
        catch(error)
        {
            //Session("flash") = "Грешна парола или потребителски email!");
            //Invalid syntax
        }

        session_email = email; // User logged in!
    }
}

form = new LoginForm();
%>

<%= form %>
