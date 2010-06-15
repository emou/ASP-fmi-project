<!-- #include FILE="init.inc" -->
<!-- #include FILE="md5.inc" -->
<%
    blocks['content'] = "<h2>Предстоящи прояви</h2>";
    var events = Event.all(null, {limit: 4});
    var events_by_sport = group_objects(events, "start");

    var wrapper = new Tag('div');
    for(var s in events_by_sport) {
        wrapper.append(new Tag('h4', {}, format_date(s, {date_only: true, week_day: true})));
        var event_list = new Tag('ul', {'class': 'events'});
        for(var i in events_by_sport[s]) {
            event_list.append(new Tag('li', {}, events_by_sport[s][i].html_link()));
        }

        wrapper.append(event_list);
    }

    blocks['content'] += wrapper;
%>

<!-- #include FILE="template.inc" -->
