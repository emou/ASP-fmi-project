<%
function print(arg) {
    Response.write(arg);
}

function debug(arg) {
    Response.write(arg + "<br/>");
}

/* Casts a value of an ASP collection */
function collValue(v) {
    var res;
    switch( typeof(v) ) {
        case 'date':
            res = new Date(v);
            break;
        case 'number':
            res = new Number(v).valueOf();
            break;
        case 'object':
            res = new Object(v);
            break;
        default:
            res = new String(v).toString();
            break;
    }

    return res;
}

function collToArray(shit) {
    var reqEnumerator = new Enumerator(shit);
    var res={}
    while(!reqEnumerator.atEnd())
    {
        var item = reqEnumerator.item();
        var v = shit(item);
        res[item] = collValue(v);
        reqEnumerator.moveNext();
    }

    return res;
}

function array_to_html(arr) {
    res = "<ul>";
    for(v in arr) {
        res += "<li>" + v + ": " + arr[v] + "</li>";
    }
    res += "</ul>";
    return res;
}


function each(collection, block, include_functions) {
    for (var key in collection) {
        if (typeof(collection[key]) === "function") {
            if (include_functions === true) {
                block(key, collection[key]);
            }
        }
        else {
            block(key, collection[key]);
        }
    }
}

function map(collection, block) {
    var to_return = [];
    each(collection, function(key, value){
        to_return[to_return.length] = block(key, value);
    });
    return to_return;
}

function select(collection, block) {
    var to_return = [];
    each(collection, function(key, value) {
        if (block(value) == true) to_return.push(value);
    });
    return to_return;
}

Object.prototype.toString = function(open_brace, close_brace) {
    str=open_brace || "{";
    for(var i in this) {
        a=this[i];
        if(a && typeof(a)!="function") {
            str+= i + ": ";
            if(typeof(a)=="string" || a instanceof String) {
                str+="'" + a + "'";
            }
            else {
                str+=a;
            }
            str+= ", \n";
        }
    }
    str+=close_brace || "}";
    return str;
};

function update_attributes(dest, src) {
    for(var i in src) {
        dest[i] = src[i];
    }
}

function format_price(p) {
    return p.toFixed(2) + " лв.";
}

Date.WEEKDAYS = ["Понеделник", "Вторник", "Сряда", "Четвъртък",  "Петък", "Събота", "Неделя"];
Date.MONTHS = ["Януари", "Февруари", "Март", "Април", "Май", "Юни", "Юли", "Август", "Септември","Октомври", "Ноември", "Декември"];

Date.prototype.getDayName = function() {
    return Date.WEEKDAYS[this.getDay()];
}

Date.prototype.getMonthName = function() {

    return Date.MONTHS[this.getMonth()];
}

Date.daysInMonth = function(month,year) {
    var dd = new Date(year, month, 0);
    return dd.getDate();
} 

function format_date(s, opts) {
    var d = new Date(s);
    if(opts && opts.date_only) {
        return (opts.week_day?d.getDayName() + ", ":"") + [d.getDate(), d.getMonthName(), d.getYear() + "г."].join(' ');
    }
    return d.toLocaleString();
}

function format_date_for_sql(d) {
    return 'timestamp ' + DB.quote_value(d.getFullYear() + '-' + (d.getMonth()+1) + '-' + d.getDate());
}


String.prototype.capitalize = function() {
    return this.charAt(0).toUpperCase() + this.slice(1);
};

String.prototype.strip = function() {
    return this.replace(/^\W+/,'').replace(/\W+$/,'');
};


function group_objects(objs, field_name) {
    if(objs[0][field_name] == null)
        throw new Error("Cannot group by undefined field " + field_name + "!");
    
    var res = {};
    for(var k in objs) {
        res[objs[k][field_name]] = res[objs[k][field_name]] || [];
        res[objs[k][field_name]].push(objs[k]);
    }

    return res;
}

function empty(ar) {
    for(var k in ar) {
        return false;
    }
    return true;
}

%>
