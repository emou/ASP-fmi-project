<!-- #include FILE="init.inc" -->
<!-- #include FILE="md5.inc" -->
<%
    blocks['content'] = new Tag('h2', {}, "Последни прояви");
    var events = Event.all();

    var event_list = new Tag('ul', {id: 'events'});

    for(var i in events) {
        event_list.append(new Tag('li', {}, events[i].link()));
    }

    blocks['content'] += event_list;
%>

<!-- #include FILE="template.inc" -->
