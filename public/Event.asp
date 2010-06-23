<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/form.inc" -->
<%
    var ev = Event.get({id: Request.QueryString("id")});
    var place = ev.associated("Place");
    var city = place.associated("Address").city;
    var tickets = ev.associated_set("TicketCategory");
    var sport = ev.associated("Sport");


    blocks['title'] = ev.Sport_name + " | " + ev.name;

    content = new Tag('h2', {}, ev.name);
    content += sport;
    content += new Tag('div', {'class': 'place'}, format_date(ev.start, {'date_only': true}) + ", " + city + ", " + place.name );

    if(tickets.length) {
        var prices_table =  new Tag('table', {'class': 'prices'});
        var head_row = new Tag('tr');
        head_row.append(new Tag('th', {}, 'бр.'));
        head_row.append(new Tag('th', {}, 'вид'));
        head_row.append(new Tag('th', {}, 'цена'));
        head_row.append(new Tag('th', {}, ''));
        prices_table.append(head_row);

        for(var i in tickets) {
            var row = new Tag('tr', {});

            row.append( new Tag('td', {}, tickets[i].count) );
            row.append( new Tag('td', {}, tickets[i].name) );
            row.append( new Tag('td', {}, format_price(tickets[i].price)) );
            row.append( new Tag('td', {}, new Form({action: 'Cart.asp?id=' + tickets[i].id, submit: 'Добави', 'class': 'buy_button'},
            {'count': {type: 'text', size: 3, id: 'count_' + i, label: null, value: 1}}) ) );

            prices_table.append(row);
        }
        content += "<h3>Цени на билетите</h3>" + prices_table;
    }
    else {
        content += "<h3>Няма билети</h3>";
    }
    content += sport.html_link("Още " + sport);
    content += '<br/>';
    content += new Tag('a', {href: 'city.asp?city=' + city}, "Още в " + city);
    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
