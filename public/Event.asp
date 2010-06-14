<!-- #include FILE="init.inc" -->
<%
    var ev = Event.get({id: Request.QueryString("id")});
    var place = ev.associated("Place");
    var tickets = ev.associated_set("TicketCategory");
    var sport = ev.associated("Sport");

    blocks['title'] = ev.Sport_name + " | " + ev.name;

    content = new Tag('h2', {}, ev.name);
    content += new Tag('h3', {}, sport.html_link());
    content += new Tag('small', {}, new Date(ev.start));
    content += new Tag('div', {'class': 'place'}, place.html_link());

    var prices_list =  new Tag('ul', {'class': 'prices'});
    for(var i in tickets) {
        prices_list.append( new Tag('li', {}, tickets[i].name + " " + format_price(tickets[i].price)) );
    }
    content += new Tag('h3', {}, "Цени на билетите") + prices_list;

    blocks['content'] = content;
%>

<!-- #include FILE="template.inc" -->
