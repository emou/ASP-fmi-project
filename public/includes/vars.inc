<!-- #include FILE="flash.inc" -->
<!-- #include FILE="html.inc" -->
<%
    /* Various variable initializations */
    
    /* GET/POST request */
    var method = Request.ServerVariables("REQUEST_METHOD");

    /* Blocks for the pseudo-template */
    var blocks = { };

    /* Flash notice for messages to the user */
    var flash = new Flash ( Session );

    /* Currently logged in user or null */
    var current_user = User.current();

    /* Log in / Log out div */
    var logged;

    var cart;
    if(current_user) {
        logged = "Влезли сте като " + current_user.email + " ";
        logged+= new Tag("a", {href: "/logout.asp", id: "logout_link"}, "Изход");

        var client = Client.get({User_email: current_user.email});
        if(client) {
            var cart      = client.find_cart();
            var cart_items = cart.items();
            if(cart_items.length>0) {
                blocks['cart'] = "Имате " + cart_items.length + " артикул" + (cart_items.length>1?"а":"")
                                 + ' в <a href="/Cart.asp">количката</a>';
            }
            else {
                blocks['cart'] = "Количката ви е празна";
            }

        }
    }
    else {
        logged = "Не сте влезли " + " ";
        logged+= new Tag("a", {href: "/login.asp", id: "login_link"}, "Вход");
        logged+= "&nbsp;|&nbsp;";
        logged+= new Tag("a", {href: "/register.asp", id: "register_link"}, "Регистрация");
    }

    var sports = Sport.all();
    
    blocks['sports_menu'] = new Tag('ul', {id: 'sports_menu'} );

    for(i in sports) {
        blocks['sports_menu'].append(new Tag('li', {}, sports[i].html_link()));
    }

%>
