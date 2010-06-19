<!-- #include FILE="html.inc" -->
<%
    function Flash(session) {
        this._content = new String ( session("flash") || "" ); //A copy
        session.Contents.Remove("flash");
    }

    Flash.prototype = {
        toString: function() {
            if(this._content != "")
                return (new Tag('div', {id: 'flash'}, this._content)).toString();
        },

        set_content: function(c) {
            this._content = c;
        }
    };
%>
