/*
 * AJAX library and helper functions for CSCI-170. 
 * Boilerplate by Randy, etc by Shaun.
 * Mar, 2010
 */

var INFO    = 0;
var WARNING = 1;
var ERROR   = 2;
var SUCCESS = 3;

var myMessages = ['info','warning','error','success']; // define the messages types 

function ge(id) {
	return document.getElementById(id);
}

function getXMLHttpReq()
{ 
    var req = null;

    if (window.XMLHttpRequest)
		req = new XMLHttpRequest();
    else if (window.ActiveXObject)
		req = new ActiveXObject(Microsoft.XMLHTTP);
    return req;
}

function hideAllMessages()
{
    var messagesHeights = new Array(); // this array will store height for each
    
    for (i=0; i<myMessages.length; i++)
    {
	messagesHeights[i] = $('.' + myMessages[i]).outerHeight();
	$('.' + myMessages[i]).css('top', -messagesHeights[i]); //move element outside viewport	  
    }
}

function showMessage(type)
{
    $('.'+ type +'-trigger').click(function(){
	hideAllMessages();				  
	$('.'+type).animate({top:"0"}, 500);
    });
}

function programmaticShowMessage(type)
{
	hideAllMessages();
	$('.'+type).animate({top:"0"}, 500);
}


/*
 * I think I'll need to be urlEncoding my output from here on out since
 * I want to be able to do more advanced things and not worry about my =
 * signs or html or etc...
 * thank you http://www.albionresearch.com/
 */

function urlDecode(encoded)
{
    if (encoded == null)
		return null;

    var plaintext = "";
    var HEXCHARS = "0123456789ABCDEFabcdef";
    var i = 0;
    for (i=0; i < encoded.length; i++)
    {
		var ch = encoded.charAt(i);
		if (ch == "+") {
			plaintext += " ";
			//i++;
		}
		else if (ch == "%") {
			if (i < (encoded.length-2) 
				&& HEXCHARS.indexOf(encoded.charAt(i+1)) != -1
				&& HEXCHARS.indexOf(encoded.charAt(i+2)) != -1 ) {
				plaintext += unescape( encoded.substr(i,3) );
				i += 2;
			}
		}
		else {
			plaintext += ch;
			//i++;
		}
		
    } // while
    return plaintext;
}

/*
 * updateLog(html) - append some nice data to the log area
 * (default: responseSpot) and keep the div scrolled to the 
 * bottom or latest text.
 */

function updateLog(html, outputDivId, append)
{
    /* These are our default values: */

    /* default div Id: 'responseSpot' */
    var outputDivId = (outputDivId == null) ? 'responseSpot' : outputDivId;
    /* default: should we append? TRUE */
    var append = (append == null) ? true : false;

    divObj = document.getElementById(outputDivId);
    if (append)
    {
		divObj.innerHTML += html;
		divObj.scrollTop = divObj.scrollHeight;
    }
    else
    { 
		divObj.innerHTML = html;
    }
}

function raiseNotice ( message )
{
    var divObj = document.getElementById("notice");
    divObj.className = "system-message";
    divObj.innerHTML = message;
}

function lowerNotice()
{
    var divObj = document.getElementById("notice");
    if (divObj)
    {
		divObj.className = "";
		divObj.innerHTML = "";
    }
}


function raiseError( message)
{
    var divObj = document.getElementById("error");
    divObj.className = "errornote";
    divObj.innerHTML = message;
}

function lowerError()
{
    var divObj = document.getElementById("error");
    if (divObj) 
    {
        divObj.className = "";
        divObj.innerHTML = "";
    }
}

/* 
 * I'm trying something new, returning all my values as 
 * key=value which I intend to parse with string.split
 * method and set in the appropriate divs... Here goes.
 */

function parseData(data) 
{
    var temp = new Array();
    var lines = new Array();

 	lines = data.split('\n');
	var message = lines[0];
	
	if (message == "ERROR_CAUSED_SHUTDOWN") {
		ge('error').innerHTML = "<h3>ERROR_CAUSED_SHUTDOWN:</h3>\n<p>" + lines[1] + "</p>";
		programmaticShowMessage('error');
	}
}

function sendData(dataStr, url, method) { 
    var req = getXMLHttpReq();

    if (req == null)
		return 1;

    /* if we are using the GET method we need to append the data
       to the URL after a ?-mark. */

    if ( method == "GET" ) 
		url += '?' + dataStr.replace(/\n/g, '&');

    req.open(method, url, true);
    
    req.onreadystatechange = function()
    {
		if(req.readyState == 4)
		{
			if(req.status == 200)
			{
				//updateLog(req.responseText, 'output', false); /* where do we dump the output? */
				parseData(req.responseText);
				updateLog("<br>Data sent/recieved from server!<br>--");
			}
			else
				updateLog("Error Status: " 
						  + req.status 
						  + "<br />Error Description: " 
						  + req.statusText);
		}
    }


    req.setRequestHeader("Content-Type", "text/plain");

    if (method == "POST")
		req.send(dataStr);
    else if (method == "GET")
		req.send(dataStr);

    return 0;
}

/* 
 * Shaun's "clear the box if it's at the default" function...
 * extra awesomeness, if it's blank reset the box to its default
 * "boxObj" is gotten from `this` in the onFocus="" method.
 * "setfocus" is 1 for onFocus and 0 for onBlur so we know if 
 * a textbox has focus or no (so we know if we're coming or going)
 */

function checkDefault(boxObj, setfocus)
{
    boxObj.hasFocus = setfocus;

    if (boxObj.hasFocus && boxObj.value ==  boxObj.defaultValue) 
        boxObj.value = "";
    else if (!boxObj.hasFocus && boxObj.value == "")
        boxObj.value = boxObj.defaultValue;
}

/*
 * A bit of a useless function, if t (object with value) has or doesn't
 * have the correct value, does something. 
 * The idea sort of fizzled out when I realized how difficult this is to
 * check with onKeyUp or onEvent stuffies.
 */

function isDigit( t )
{
    if (isNaN(parseInt(t.value)) && t.value != "") {
        alert("Integers only! Bad Human, Bad!");
        t.value = 0;
    }
}

function exchangeData( boxObj )
{
    var uploadstring = boxObj.value;
    sendData(uploadstring, 'http://gimli.morningside.edu/~meyersh/cgi-bin/S8.E1.1.cgi');
}

/*
 * Allows two submit buttons that can change a Forms submit method
 */

function modSubmit(formid, method) 
{
    formid = document.getElementById(formid);
    formid.method = method;
    //document.getElementById('inputFormSubmit').value = 'Submit ' + method;
    formid.submit();
}

function Submit(formid) 
{
    var frm = document.getElementById(formid);
    var method = frm.method;
    var url = frm.action;
    var inpt = frm.input.value;
    sendData(inpt, url, method);
}
//
// EOF
//
