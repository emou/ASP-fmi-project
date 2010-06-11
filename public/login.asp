<%
    var session_email = Session("user_email");
    if (session_email) {
        Session("flash") = "Вече сте се вписали!";
    }
    else {
        try {
                user = User.authenticate(Form('email'), Form('password'));
            }
        except(error)
            {
                Session("flash") = "Грешна парола или потребителски email!");
            }

        session_email = email; // User logged in!
    }
%>
