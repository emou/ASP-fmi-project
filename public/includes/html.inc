<%
/* A class representing an HTML tag.
 * Really simple.
 */
function Tag(name, attributes, content) {
    this.name = name;
    this.attributes = attributes;
    this.content = content!=null?[content]:[];
}

Tag.prototype = {
    append: function(piece) {
        this.content.push(piece);
    },

    toString: function() {
        return '<' + this.name + Tag.stringify_attributes(this.attributes) + '>\n'
                   + this.content.join('') + '\n</' + this.name + '>';
    }
};

/* A class representing an empty HTML tag.
 * For example: <input name="..." type="..." />
 * as opposed to <input name="..." type=".."></input>.
 * Inherits Tag.
 */
function EmptyTag(name, attributes) {
    Tag.call(this, name, attributes);
}

EmptyTag.prototype = {
    toString: function() {
        return '<' + this.name + Tag.stringify_attributes(this.attributes) + '/>';
    }
};

Tag.stringify_attributes = function(attributes) {
    var str = '';
    for(var k in attributes) {
        if(attributes[k]!=null) {
            str += ' ' + k + '="' + attributes[k] + '"';
        }
    }
    return str;
};

Tag.model_link = function(model_obj) {
    return new Tag('a', {href: (model_obj.constructor.table_name + '.asp')});
};

%>
