<!-- #include file="includes/init.inc" -->
<%
    blocks['title'] = "Месечна статистика";

    var content = "";

    content += new Form({submit: 'Виж', action: 'monthly_stat.asp'},
                        {month: {type: 'select', choices: Date.MONTHS, numbered: true} });

    if(method=="POST") {
        var month = 1*Request.Form('month');
        content += "Месец " + Date.MONTHS[month];

        var mo = db.execute('select monthly_orders(' + (month + 1) + ')');
        content += "<p>Общо поръчки за месеца: " + 1*mo("monthly_orders").value + "</p>";

        var mt = db.execute('select monthly_tickets(' + (month + 1) + ')');
        content += "<p>Общо закупени билети за месеца: " + 1*mt("monthly_tickets").value + "</p>";
    }
    else {

    }

    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
