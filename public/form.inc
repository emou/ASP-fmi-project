/* A class representing a form.
 * (Django rules) */
function Form(opts) {
  this.fields={};
};

Form.prototype = {
  add_field: function(f) {
	       if !(f instanceof Field) {
		 throw Error();
	       }
	       else {
		 this.fields.push(f);
		}
	     }
  to_html: function() {
	     res="<form>";
	     for(v in this.fields) {
	       this.fields[v].to_html();
	     }
	     res+="</form>";
	     return res;
	   }
};


function Field(opts) {
};

TextField.prototype = {
  to_html: function() {
	     for(v in fields) {
	     }
	   }
}
