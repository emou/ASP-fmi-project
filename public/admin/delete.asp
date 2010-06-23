<!-- #include FILE="includes/init.inc" -->
<!-- #include VIRTUAL="/includes/form.inc" -->
<%
    if(method=="POST") {
        var model_name = new String ( Request.QueryString("object_type") ).toString();
        var Model = DB.Connection.model_index[ model_name ];

        if(!Model) {
            Response.Redirect("/admin");
        }

        var filter = {};
        var pks = Model.primary_keys;
        for(var i in pks) {
            var k = Request.Form(pks[i]);
            if(!k) {
                Response.Redirect("/admin");
            } else {
                filter[ pks[i] ] = k;
            }
        }

        var obj = Model.get(filter);
        obj.remove();
        Session("flash") = "Успешно изтриване на " + Model.singular;
        Response.redirect("/admin");
    }

%>

<!-- #include FILE="includes/template.inc" -->
