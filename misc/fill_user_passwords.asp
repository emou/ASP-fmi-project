<%
    /* This code was used to fill the password values of all users to
     * first part of their email address (before the @ sign).
     * */
    users = User.all();
    for(var i in users) {
        user = users[i];
        user.set_password(user.email.split('@')[0]);
        user.save();
    }
%>
