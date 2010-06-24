<!-- #include FILE="includes/init.inc" -->
<!-- #include VIRTUAL="/includes/form.inc" -->
<%
    blocks['title'] = "Редакция / добавяне";
    var model_name = new String ( Request.QueryString("object_type") ).toString();
    var Model = DB.Connection.model_index[ model_name ];

    if(!Model) {
        Response.Redirect("/admin");
    }

    var filter = {};
    var pks = Model.primary_keys;
    for(var i in pks) {
        var k = Request.QueryString(pks[i]);
        if(!k) {
            filter = {};
            break;
        } else {
            filter[ pks[i] ] = decodeURIComponent( k );
        }
    }

    if(!empty(filter))
        var obj = Model.get(filter);

    var form;
    var form_opts = {action: Request.ServerVariables("PATH_INFO") + "?" + Request.QueryString, name: Model.singular}


    if(method == "POST") {
        form = make_model_form(Model, form_opts, collToArray(Request.Form));

        if(form.valid()) {
            flash.set_content( "Успешно редактирахте " + Model.singular.toLowerCase() );
            form.save();
        }
    }
    else {
        form = make_model_form(Model, form_opts, obj);
    }
    

    blocks['content'] = "<h2>" + Model.singular + "</h2>";

    var content = form; 

    blocks['content'] += content;
%>

<!-- #include FILE="includes/template.inc" -->
