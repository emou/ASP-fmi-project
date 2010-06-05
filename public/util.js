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

  Response.Charset = "UTF-8";
  Response.Codepage = 65001;
%>
