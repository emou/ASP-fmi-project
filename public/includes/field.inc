<%
/* A class representing a field.
 * Again, inspired by Django.
 * Modifies argument!
 */
function Field(attrs) {

    if(attrs && (!attrs['name'] || !attrs['type']))
    throw new Error("Field construction error: you need to provide at least a name and a type!");

    this.valid = true;

    if(attrs['id']===undefined) {
        attrs['id'] = attrs['name'] + '_id';
    }

    var label = attrs['label'];

    if(label === undefined) {
        this.label = attrs['name'];
    }
    else if(label) {
        delete attrs['label'];
        this.label = label;
    }

    if(this.label) {
        this.label = Settings.field_names[this.label] || this.label;
        this.label = this.label.capitalize();
    }

    var required = attrs['required'];
    if(required != false) {
        required = true;
    }
    this.required = required;
    delete attrs['required'];

    this.value = attrs['value'];
    this.attributes = attrs;


    /* Why, oh, why didn't I make subclasses??!?!
     * No time to do it now (deadline).
     * */
    switch( attrs['type'] ) {
        case 'textarea':
            this.core_html = new Tag('textarea', this.attributes, this.attributes['value']);
            delete this.attributes['type'];
            delete this.attributes['value'];
            break;
        case 'timestamp':
            this.core_html = new DateWidget( this.attributes['value'] );

            delete this.attributes['type'];
            delete this.attributes['value'];
            delete this.attributes['name'];
            break;
        case 'select':
            var choices  = this.attributes['choices'];
            var values   = this.attributes['values'] || choices;
            var selected = this.attributes['selected'];
            var num      = this.attributes['numbered'];
            var is_selected = {};
            is_selected[selected]="selected";

            delete this.attributes['choices'];
            delete this.attributes['type'];
            delete this.attributes['selected'];
            delete this.attributes['values'];
            delete this.attributes['numbered'];

            this.core_html = new Tag('select', this.attributes);

            if(choices instanceof Range) {
                for(var i=choices.from; i<=choices.to; i++) {
                    this.core_html.append( new Tag('option', {value: i, 'selected': is_selected[i]}, i) );
                }
            }
            else {
                for(var i=0; i<choices.length; i++) {
                    this.core_html.append( new Tag('option',
                                            { value: num?i:values[i], 'selected': is_selected[choices[i]] },
                                            choices[i])
                                        );
                }
            }

            break;

        case 'checkbox':
            this.value = this.value=="0" || this.value===undefined ? 'f': 't'; /* PostgreSQL specific */

           /* This is what will be sent when the form is
            * submitted if the checkbox is checked, so we set
            * this to always be 't' */
           /* PosgreSQL specific */
           this.attributes['value'] = 't';

           if(this.value == 't') {
               this.attributes['checked'] = 'checked';
           }

       default:
           this.core_html = new EmptyTag('input', this.attributes);
    }
       /* this.core_html should be something with toString() method that represents the field in HTML.
        * I currently use a Tag object, and a DateWidget object */
};


Field.prototype = {
    toString: function() {
        var p_attrs = this.valid ? {} : {'class': 'invalid_field'};

        var p=new Tag('p', p_attrs);
        if(this.label) {
            label = new Tag('label', {'for': this.attributes['id']}, this.label);
            p.append(label);
        }
        p.append(this.core_html);

        return p.toString();
   },

   set_value: function(val) {
       this.value=val;
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

function DateWidget(date) {
    // var days_in_month = Date.daysInMonth(date.getYear(), date.getMonth());
    /* XXX: Until I make client-side scripting to partially submit the date */
    var days_in_month = 31;
    var selection = {};

    if(date) {
        date = new Date( date );

        selection['day']    = date.getDate();
        selection['month']  = date.getMonthName();
        selection['year']   = date.getYear();
        selection['hour']   = date.getHours();
        selection['minute'] = date.getMinutes();
    }

    this.day = new Field({
        name: 'day',
        label: null,
        type: 'select',
        choices: new Range(1,days_in_month),
        selected: selection['day']
    });

    this.month = new Field({
        name: 'month',
        label: null,
        type: 'select',
        choices: Date.MONTHS,
        selected: selection['month']
    });

    this.year = new Field({
        name: 'year', label: null,  type: 'select',
        choices: new Range(Settings.min_year, Settings.min_year + Settings.year_offset),
        selected: selection['year']
    });

    this.hour = new Field({
        name: 'hour', label: null, type: 'select', choices: new Range(0,23), 
        selected: selection['hour']
    });

    this.minute = new Field({
        name: 'minute',type: 'select',
        label: null,
        choices: new Range(0,60),
        selected: selection['hour']});

    }

    DateWidget.prototype.toString = function() {
        return  this.day.toString() + this.month.toString() +
        this.year.toString() + this.hour.toString() +
        this.minute.toString();
    }

    /* A Range 'struct'.
     * This should be used by the Field constructor,
     * hence the Field constructor and the Range constructor
     * are coupled.
     * */
    function Range(from, to) {
        this.from = from;
        this.to = to;
    }
%>
