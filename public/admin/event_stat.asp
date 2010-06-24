<!-- #include file="includes/init.inc" -->
<%
    blocks['title'] = "Най-популярни прояви";

    var content = "";

    var sql = DB.select_statement("Event", 'count(*)');

    var sql = "select ev.\"name\",sum(oi.\"count\") from \"OrderItem\" oi JOIN \"TicketCategory\" tc on oi.\"TicketCategory_id\"=tc.\"id\" JOIN \"Event\" ev on tc.\"Event_id\"=ev.\"id\" group by ev.\"name\" order by sum(oi.\"count\");"

    var top_events = DB.getRows(db.execute(sql));

    var table = new Tag('table', {'class': 'top_events'});
    table.append("<tr><th>Проява</th><th>Бр. продадени билети</th></tr>");
    for(var k in top_events) {
        table.append("<tr><td>" + top_events[k].name + "</td><td>" + top_events[k].sum + "</td></tr>");
    }

    content += table;

    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
