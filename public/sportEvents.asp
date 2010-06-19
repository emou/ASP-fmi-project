<%
	var events = SportEvents.get_all_objects('foobar, foobar, foobar');
	var sports = Sports.get_all() 


	Server.execute(../templates/sportEvents.asp);
%>


	<% for(e in events) { %>
	    <%= e.name %> at <%= e.place %>
	<% } %>
