<%
    /* Various variable initializations */
    
    /* GET/POST request */
    var method = Request.ServerVariables("REQUEST_METHOD");

    /* If this is set, then we have a logged in user */
    var session_email = Session("email");

    var flash = new String( Session("flash") || '' ); // A copy
    if(flash) Session.Contents.Remove("flash");

%>
