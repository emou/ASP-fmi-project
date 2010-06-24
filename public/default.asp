<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/md5.inc" -->
<%
    var events;
    if(new String(Request.QueryString('past'))!='undefined') {
        blocks['content'] = '<h2>Минали прояви</h2><a href="default.asp">Предстоящи прояви</a>';
        events = Event.past();
    }
    else {
        blocks['content'] = '<h2>Предстоящи прояви</h2><a href="default.asp?past=1">Минали прояви</a>';
        events = Event.upcoming();
    }

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

<!-- #include FILE="includes/template.inc" -->
