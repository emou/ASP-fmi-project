<%
  function print(args) {
    Response.write(args);
  }

  function rsToArray(shit) {
	  var reqEnumerator = new Enumerator(shit);
	  var res={}
	  while(!reqEnumerator.atEnd())
	  {
	      res[reqEnumerator.item()]=shit(reqEnumerator.item());
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

 
  function each( collection, block, include_functions ){
    for (var key in collection)
      if (typeof(collection[key]) === "function"){
	if (include_functions === true) block(key, collection[key]);
      } else
	block(key, collection[key]);
  };

  function map( collection, block ){
    var to_return = [];
    each(collection, function(key, value){
      to_return[to_return.length] = block(key, value);
    });
    return to_return;
  }

  function filter( collection, block ){
    var to_return = [];
    each(collection, function(key, value){
      if (block(value) == true) to_return[to_return.length] = value;
    });
    return to_return;
  }

  Response.Charset = "UTF-8";
  Response.Codepage = 65001;
%>
