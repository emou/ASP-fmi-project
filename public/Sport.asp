<!-- #include FILE="includes/init.inc" -->
<%
    var sport = Sport.get({name: decodeURIComponent( Request.QueryString("name") )});
    var events = sport.associated_set("Event");

    blocks['title'] = sport.name;

    var ul = new Tag('ul', {'class': 'events'});
    for(var i in events) {
    ul += new Tag('li', {'class': 'sport_events'},
                        new Tag('span', {'class':'small_date'}, format_date(events[i].start, {'date_only': true}) )
                        + events[i].html_link());
    }

    blocks['content'] = "<h2>" + sport.name + "</h2>" + ul;
%>

<!-- #include FILE="includes/template.inc" -->
