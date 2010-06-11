<%
    /* Various variable initializations */
    
    /* GET/POST request */
    var method = Request.ServerVariables("REQUEST_METHOD");

    /* If this is set, then we have a logged in user */
    var user_email = Session("user_email");
%>
