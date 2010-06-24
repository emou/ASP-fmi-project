<!-- #include FILE="includes/init.inc" -->
<!-- #include FILE="includes/md5.inc" -->
<%

    blocks['title'] = "Търсене";
    var content="<h3>Търсене</h3>";
    var y = new Date().getFullYear();

    var from = new Date(
        Request.QueryString("from_year"),
        Request.QueryString("from_month"),
        Request.QueryString("from_day")
    );

    var to = new Date(
        Request.QueryString("to_year"),
        Request.QueryString("to_month"),
        Request.QueryString("to_day")
    );

    var search_form = new Form(
        {
            action: 'search.asp', submit: 'Търси', 'class': 'date', method: 'get'
        },
        {
            from_year: {type: 'select', choices: new Range(y-1,y+Settings.year_offset), label: 'От', value: from.getFullYear()},
            from_day: {type: 'select', choices: new Range(1,31), label: null, value: from.getDate()},
            from_month: {type: 'select', choices: Date.MONTHS, numbered: true, label: null, value: from.getMonth()+1},

            to_year: {type: 'select', choices: new Range(y-1,y+Settings.year_offset), label: 'До', value: to.getFullYear()},
            to_day: {type: 'select', choices: new Range(1,31), label: null, value: to.getDate()},
            to_month: {type: 'select', choices: Date.MONTHS, numbered: true, label: null, value: to.getMonth()}
        }
    );

    content += search_form;
    if(from.getYear() && to.getYear()) {
        var sql = 'select * from "Event" where "start"<' + format_date_for_sql(to) + ' AND "start">' + format_date_for_sql(from);
        var res = Event.model_query(sql);
        var ul = new Tag('ul');
        for(var i in res) {
            ul.append(new Tag('li', {}, res[i].html_link()));
        }
        content += ul;
    }


    blocks['content'] = content;
%>

<!-- #include FILE="includes/template.inc" -->
