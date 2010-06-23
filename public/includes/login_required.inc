<%
    if(!current_user) {
        Session("flash") = "Трябва да сте влезли, за да видите тази страница";
        Response.Redirect("/login.asp");
    }
%>
