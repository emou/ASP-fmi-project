<!-- #include FILE="html.inc" -->
<%

/* A class representing a validation error in a form */
function FormError(msg) {
    this.msg = msg;
}

/* A class representing a form.
 * Inspired by Django.
 * req should be an ASP Request object
 * */
function Form(opts, field_defs, initial_data) {

    if(opts && !opts['action']) {
        /* XXX A hack: opts==undefined is permitted for inheritance */
        throw new Error("You need to provide an action for the form.");
    }

    opts = opts || {};

    this.fields={};

    this.name = opts['name'];
    delete opts['name'];

    this.submit = opts['submit'] || 'Изпрати';
    delete opts['submit'];

    opts['method'] = opts['method'] || 'post';

    this.attributes = opts;

    if(initial_data) {
        for(var k in field_defs) {
            field_defs[k]['value'] = initial_data[k] || "";
        }
    }

    Form.prototype.add_fields.call(this, field_defs);

    this.errors = {};
}

Form.prototype = {

    add_fields: function(field_defs) {
        for(var n in field_defs) {
            field_defs[n]['name'] = n;
            var f = new Field(field_defs[n]);
            this.fields[ f.get_name() ] = f;
        }
    },

    toString: function() {
        var form_tag=new Tag('form', this.attributes);

        var res;
        if(this.name) {
            //Add a fieldset
            res = new Tag('fieldset');
            res.append(new Tag('legend', {}, this.name));
            form_tag.append(res);
        }
        else {
            res = form_tag;
        }

        if( !empty(this.errors) ) {
            var err_tag = new Tag('ul', {'class': 'form_errors'});
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
        var submit_p = new Tag('p');
        submit_p.append(new EmptyTag('input', {name: 'submit', type: 'submit', value: this.submit}));
        res.append(submit_p);
        return form_tag.toString();
    },

    valid: function() {
        if(!this.constructor.checks)
            return true;

        var is_valid = true;
        for(var i in this.fields) {
            var f = this.fields[i];

            if(f.get_value()=='' && f.required) {
                this.errors[i] = "Полето " + f.get_label() + " е задължително!";
                is_valid = false;
                continue;
            }

            var cleaner = this.constructor.checks[i];

            if(cleaner) {
                try {
                    var cleaned_value = cleaner(f.get_value());
                    if(cleaned_value) {
                        f.set_value(cleaned_value);
                    }
                }
                catch(e) {
                    is_valid = false;
                    this.errors[i] = e.msg;
                }
            }
        }

        var global_cleaner = this.constructor.checks && this.constructor.checks['all'];
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

function make_form(opts, field_defs) {
    function FormClass(local_opts, initial_data) {
        update_attributes(opts, local_opts);
        Form.call(this, opts, field_defs, initial_data);
    }
    FormClass.prototype = new Form();
    FormClass.prototype.constructor = FormClass;
    return FormClass;
}

function make_model_form(Model, opts, initial_data) {
    var cols = Model.columns;
    var field_defs = {};
    
    for(var i in cols) {
        field_defs[cols[i].name] = ( cols[i].field_def() );
        field_defs[cols[i].name]['label'] = Settings.field_names[ cols[i].name ];
    }

    var ModelForm = make_form(opts, field_defs);
    ModelForm.prototype.save = function() {
        var f = {};

        for(var k in this.fields) {
            f[k] = this.fields[k].get_value();
        }

        var model = new Model(f); 

        model.save();
    };

    return new ModelForm(undefined, initial_data);
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

    if(label === undefined) {
        this.label = attrs['name'];
    }
    else if(label) {
        delete attrs['label'];
        this.label = label;
    }

    if(this.label)
        this.label = this.label.capitalize();

    var required = attrs['required'];
    if(required != false) {
        required = true;
    }
    this.required = required;
    delete attrs['required'];


    this.value = attrs['value'];
    this.attributes = attrs;

    switch( attrs['type'] ) {
        case 'textarea':
            this.tag = new Tag('textarea', this.attributes, this.attributes['value']);
            delete this.attributes['type'];
            delete this.attributes['value'];
            break;
        case 'timestamp':
            this.tag = new Tag('p', this.attributes, 'A date mada faka');
            break;
        case 'checkbox':
            if(this.value == true) {
                attrs['value'] = this.value? "on":"";
                attrs['checked'] = 'checked';
            }
        default:
            this.tag = new EmptyTag('input', this.attributes);
    }

};

Field.prototype = {
    toString: function(error) {
        var p_attrs = this.valid ? {} : {'class': 'invalid_field'};
        var p=new Tag('p', p_attrs);
        if(this.label) {
            label = new Tag('label', {'for': this.attributes['id']}, this.label);
            p.append(label);
        }
        p.append(this.tag);
        return p.toString();
    },

    set_value: function(val) {
        this.attributes['value']=val;
    },

    get_value: function() {
        return this.value;
    },

    set_name: function(name) {
        this.attributes['name']=name;
    },

    get_name: function() {
        return this.attributes['name'];
    },

    get_label: function() {
        return this.label;
    },

    mark_invalid: function() {
        this.valid = false;
    }
};

LoginForm = make_form(
    {action: 'login.asp', submit: 'Вход', name: 'Вход', id: 'login_form'},

    {
     email: {type: 'text', label: 'e-mail адрес'},
     password: {type: 'password', label: 'парола'}
    }
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


RegisterForm = make_form(
    {action: 'register.asp', submit: 'Регистрация', name: 'Регистрация', id: 'register_form'},
    {
     email: {type: 'text', label: 'e-mail адрес'},
     password: {type: 'password', label: 'парола'},
     password_confirmation: {type: 'password', label: 'потвърдете паролата'},
     first_name: {type: 'text', label: 'собствено име'},
     last_name: {type: 'text', label: 'фамилно име'},
     phone_number: {type: 'text', label: 'телефонен номер'},
     city: {type: 'text', label: 'град'},
     details: {type: 'textarea', label: 'адрес'}
    }
);

RegisterForm.checks = {
    email: function(e) {
        if ( User.exists({email: e}) )
                throw new FormError("Този Email адрес вече е зает!");
        return e;
    },

    password: function(pass) {
        if(pass.length<5)
            throw new FormError("Тази парола е твърде къса, моля въведете поне 5 символа.");
        return pass;
    },

    all: function(fields) {
        if ( fields.password_confirmation.get_value() != fields.password.get_value() ) {
            fields.password_confirmation.mark_invalid();
            throw new FormError("Паролата и потвърждението не съвпадат.");
        }
    }
};

RegisterForm.prototype.save = function() {
    var f = {};

    for(var k in this.fields) {
        f[k] = this.fields[k].get_value();
    }

    var u = new User(f);
    u.is_admin = false;
    u.set_password(f.password);
    u.save();

    var a = new Address(f);
    a.save();

    var c = new Client({User_email: u.email, phone_number: f.phone_number, Address_id: a.id});
    c.save();
};

%>
