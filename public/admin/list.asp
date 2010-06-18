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

    var content = new Tag('ul', {id: 'object_list'});

    for(var i in objects) {
        var li = new Tag('li', undefined, objects[i].admin_buttons());
        content.append(li);
    }

    blocks['content'] += content;
%>

<!-- #include FILE="includes/template.inc" -->
