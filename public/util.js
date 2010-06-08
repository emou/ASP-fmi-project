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

  Response.Charset = "UTF-8";
  Response.Codepage = 65001;
%>
