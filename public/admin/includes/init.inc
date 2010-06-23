<!-- #include virtual="/includes/init.inc" -->
<!-- #include virtual="/includes/login_required.inc" -->
<%
if(!current_user.is_admin) {
    Session("flash") = "Трябва да сте влезли с администраторския си акаунт.";
    Response.redirect("/");
}
%>
