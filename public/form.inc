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
        return '<' + this.name + Tag.stringify_attributes(this.attributes) + '/>\n';
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

/* A class representing a validation error in a form */
function FormError(msg) {
    this.msg = msg;
}

/* A class representing a form.
 * Inspired by Django.
 * req should be an ASP Request object
 * */
function Form(opts, req, field_defs) {

    /* Add fields and set their values if request is given */
    this.fields={};

    Form.prototype.add_fields.call(this, field_defs);

    if(req) {
        for(n in this.fields) {
            this.fields[n].set_value( new String(req.Form(n)) );
        }
    }

    var opts = opts || {};

    if(!opts['action']) {
        throw new Error("You need to provide an action for the form.");
    }

    this.attributes = {
        method: opts['method'] || 'POST',
        action: opts['action']
    };

    this.submit = opts['submit'] || 'Изпрати';

    this.errors = {};
}

Form.prototype = {

    add_fields: function(field_defs) {
        for(i in field_defs) {
            var f = new Field(field_defs[i]);
            this.fields[ f.get_name() ] = f;
        }
    },

    toString: function() {
        var res=new Tag('form', this.attributes);
        if(this.errors) {
            var err_tag=new Tag('ul', {'class': 'form_errors'});
            for(i in this.errors) {
                err_tag.append(new Tag('li', {}, this.errors[i]));
            }
            res.append(err_tag);
        }
        for(v in this.fields) {
            var f=this.fields[v];
            if(this.errors[v])
                f.mark_invalid();
            res.append(f);
        }
        res.append(new Tag('input', {type: 'submit', value: this.submit}));
        return res.toString();
    },

    valid: function() {
        var is_valid = true;
        for(var i in this.fields) {
            var f = this.fields[i];
            var cleaner = this.constructor.checks[i];

            if(cleaner) {
                try {
                    var cleaned_value = cleaner(f.get_value());
                    f.set_value(cleaned_value);
                }
                catch(e) {
                    is_valid = false;
                    this.errors[i] = e.msg;
                }
            }
        }

        var global_cleaner = this.constructor.checks['all'];
        if(global_cleaner) {
            try {
                global_cleaner(this.fields);
            }
            catch(e) {
                is_valid = false;
                this.errors['all'] = e.msg;
            }
        }
        return is_valid;
    },

    save: function() {

    }

};

function make_form(field_defs, opts) {
    function FormClass(local_opts, req) {
        update_attributes(opts, local_opts);
        Form.call(this, opts, req, field_defs);
    }
    FormClass.prototype=Form.prototype;
    FormClass.prototype.constructor=FormClass;
    return FormClass;
}

function make_model_form(model, opts) {
    var field_defs = {};
    return make_form(field_defs);
}

/* A class representing a field.
 * Again, inspired by Django.
 */
function Field(attrs) {
    /* Modifies argument! */
    if(!attrs || !attrs['name'] || !attrs['type'])
        throw new Error("Field construction error: you need to provide at least a name and a type!");

    this.valid = true;

    attrs['id'] = (attrs['id']) || (attrs['name'] + '_id');

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
    toString: function(error) {
        var p_attrs = this.valid ? {} : {'class': 'invalid_field'};
        var p=new Tag('p', p_attrs);
        label = new Tag('label', {'for': this.attributes['id']}, this.attributes['name']);
        p.append(label);
        p.append(new EmptyTag('input', this.attributes));
        return p.toString();
    },

    set_value: function(val) {
        this.attributes['value']=val;
    },

    get_value: function() {
        return this.attributes['value'];
    },

    set_name: function(name) {
        this.attributes['name']=name;
    },

    get_name: function() {
        return this.attributes['name'];
    },

    mark_invalid: function() {
        this.valid = false;
    }
};

LoginForm = make_form(
    [{type: 'text', name: 'email', label: 'e-mail адрес'},
     {type: 'password', name: 'password', label: 'Парола'}
    ],

    {action: 'login.asp', submit: 'Вход'}
);

LoginForm.checks = {

    all: function(fields) {
        try {
            User.authenticate(fields['email'].get_value(), fields['password'].get_value());
        }
        catch(e) {
            throw new FormError("Грешна парола или e-mail адрес.");
        }

    }

};

LoginForm.prototype.save = function() {
    return User.authenticate(this.fields['email'].get_value(), this.fields['password'].get_value());
};

%>
