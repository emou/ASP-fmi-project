<%
/* A class representing an HTML tag.
 * Really simple.
 */
function Tag(name, attributes, content) {
    this.name = name;
    this.attributes = attributes;
    this.content = content?[content]:[];
}

Tag.prototype = {
    append: function(piece) {
        this.content.push(piece);
    },

    toString: function() {
        return '<' + this.name + Tag.stringify_attributes(this.attributes) + '>' + this.content.join('') + '\n</' + this.name + '>';
    }
};

Tag.stringify_attributes = function(attributes) {
    var str = '';
    for(var k in attributes) {
        if(attributes[k]) {
            str += ' ' + k + '="' + attributes[k] + '"';
        }
    }
    return str;
};

/* A class representing a form.
 * Inspired by Django.
 * */
function Form(opts) {
    var opts = opts || {};
    this.attributes = {
        method: opts['method'] || 'POST'
    };
    this.submit = opts['submit'] || 'Изпрати';
    this.fields=[];
}

Form.prototype = {

    add_fields: function(field_defs) {
        for(i in field_defs) {
            this.fields.push(new Field(field_defs[i]));
        }
    },

    toString: function() {
        var res=new Tag('form', this.attributes);
        for(v in this.fields) {
            res.append(this.fields[v]);
        }
        res.append(new Tag('input', {type: 'submit', value: this.submit}));
        return res.toString();
    },

    check: function() {
        this.data={};
        for(var f in this.fields) {
            var cleaner = this['check_' + f.name];
        }
    }

};

function make_form(field_defs) {
    function FormClass(opts) {
        Form.call(this, opts);
        this.add_fields(field_defs);
    }
    FormClass.prototype=Form.prototype;
    return FormClass;
}

/* A class representing a field.
 * Again, inspired by Django.
 */
function Field(attrs) {
    /* Modifies argument! */
    if(!attrs || !attrs['name'] || !attrs['type'])
        throw new Error("Field construction error: you need to provide at least a name and a type!");

    attrs['id'] = (attrs['id'] || attrs['name']) + '_id';

    var label = attrs['label'];
    if(label) {
        delete attrs['label'];
        this.label = label;
    }
    else {
        this.label = attrs['name'];
    }
    this.label = this.label.capitalize();

    this.attributes = attrs;
};

Field.prototype = {
    toString: function() {
        var p=new Tag('p');
        label = new Tag('label', {'for': this.attributes['id']}, this.attributes['name']);
        p.append(label);
        p.append(new Tag('input', this.attributes));
        return p.toString();
    }
};

LoginForm = make_form([
        {type: 'text', name: 'email', label: 'e-mail адрес'},
        {type: 'password', name: 'password', label: 'Парола'}
    ]
);

%>
