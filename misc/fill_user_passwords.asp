<%
    /* This code was used to fill the password values of all users with the
     * first part of their email address (before the @ sign).
     * */
    users = User.all();
    for(var i in users) {
        user = users[i];
        user.set_password(user.email.split('@')[0]);
        debug("setting " + user.email + " password to : " + user.email.split('@')[0] + " with hash: " + user.password)
        user.save();
    }
%>
