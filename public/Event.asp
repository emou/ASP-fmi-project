<!-- #include FILE="init.inc" -->
<%
    var e = Event.get({id: Request.QueryString("id")});
    blocks['title'] = e.Sport_name + " | " + e.name;

    content = new Tag('h2', {}, e.name);
    content += new Tag('h3', {}, e.Sport_name);
    content += new Tag('small', {}, new Date(e.start));
    content += new Tag('div', {'class': 'place'}, e.Place_name)

    blocks['content'] = content;
%>

<!-- #include FILE="template.inc" -->
