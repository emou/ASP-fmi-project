<!-- #include FILE="includes/init.inc" -->
<!-- #include VIRTUAL="/includes/form.inc" -->
<%
    blocks['title'] = "Редакция";
    var model_name = new String ( Request.QueryString("object_type") ).toString();
    var Model = DB.Connection.model_index[ model_name ];

    if(!Model) {
        Response.Redirect("/admin");
    }

    var form_opts = {action: "/admin/edit.asp?object_type=" + Model.table_name, name: Model.singular}

    if(method == "POST") {
        var form = make_model_form(Model, form_opts, collToArray(Request.Form));

        if(form.valid()) {
            form.save();
        }
    }
    else {
        var pks = Model.primary_keys;
        var filter = {};

        for(var i in pks) {
            var k = Request.QueryString(pks[i]);
            if(!k) {
                Session("flash") = "Недостатъчно параметри при търсене на обект";
                Response.Redirect("/admin");
            } else {
                filter[ pks[i] ] = decodeURIComponent( k );
            }
        }

        var obj = Model.get(filter);

        var form = make_model_form(Model, form_opts, obj);

        blocks['content'] = "<h2>" + Model.singular + ": редактиране</h2>";

        var content = form; 

        blocks['content'] += content;
    }
%>

<!-- #include FILE="includes/template.inc" -->
