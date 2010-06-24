<!-- #include file="includes/init.inc" -->
<%
    blocks['title'] = "Администраторска част";
    blocks['content'] = "<h2>Обекти</h2><p>Изберете обекти за редактиране от списъка по-долу:</p>";

    var content = new Tag('ul', {id: 'object_type_list'});
    var model_index = DB.Connection.model_index;

    for(var i in model_index) {
        var model = model_index[i];
        var li = new Tag('li', undefined, new Tag('a', {href: 'list.asp?object_type=' + i}, model.plural));
        content.append( li );
    }

    content += "<h2>Статистика</h2>";

    content += '<ul class="stat"> \
                    <li><a href = "monthly_stat.asp">За месец</a></li> \
                    <li><a href = "event_stat.asp">По прояви</a></li> \
               </ul>'

    blocks['content'] += content;
%>

<!-- #include FILE="includes/template.inc" -->
