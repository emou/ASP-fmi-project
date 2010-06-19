<!-- #include FILE="includes/init.inc" -->
<%
    blocks['title'] = "Администраторска част";

    var model_name = new String ( Request.QueryString("object_type") ).toString();

    var Model = DB.Connection.model_index[ model_name ];

    if(!Model) {
        Response.Redirect("/admin");
    }

    var objects = Model.all();

    blocks['content'] = "<h2>" + Model.plural + "</h2>";

    var content = new Tag('table', {id: 'object_list'});

    for(var i in objects) {
        var row = new Tag('tr');
        row.append( new Tag('td', undefined, objects[i]) );
        row.append( new Tag('td', undefined, objects[i].admin_buttons()) );
        content.append( row );
    }

    blocks['content'] += content;
%>

<!-- #include FILE="includes/template.inc" -->
