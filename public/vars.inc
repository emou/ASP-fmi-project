<!-- #include FILE="flash.inc" -->
<!-- #include FILE="models.inc" -->
<!-- #include FILE="html.inc" -->
<%
    /* Various variable initializations */
    
    /* GET/POST request */
    var method = Request.ServerVariables("REQUEST_METHOD");

    /* If this is set, then we have a logged in user */
    var session_email = Session("email");

    /* Blocks for the pseudo-template */
    var blocks = { };

    /* Flash notice for messages to the user */
    var flash = new Flash ( Session );

    /* Currently logged in user or null */
    var current_user = User.current();

    /* Log in / Log out div */
    var logged;

    if(current_user) {
        logged = "Влезли сте като " + current_user.email + " ";
        logged+= new Tag("a", {href: "/logout.asp", id: "logout_link"}, "Изход");
    }
    else {
        logged = "Не сте влезли " + " ";
        logged+= new Tag("a", {href: "/login.asp", id: "login_link"}, "Вход");
    }

%>
