globGetMethod = 1; /* 0: html; 1: JSON */
let object;

function setGetMethod (val) {
    globGetMethod = val;
} 

function $(id) {
    return document.getElementById(id);
}

function getXMLHttpRequest() {
    // XMLHttpRequest for Firefox, Opera, Safari
    if (window.XMLHttpRequest) {
        return new XMLHttpRequest();
    }
    if (window.ActveObject) { // Internet Explorer
        try { // for IE new
            return new ActiveXObject("Msxml2.XMLHTTP");
        }
        catch (e)  {  // for IE old
            try {
                return new ActiveXObject("Microsoft.XMLHTTP");
            }
            catch (e)  {
                alert("Your browser does not support AJAX!");
                return null;
            }
        }
    }    
    return null;
} 

function getHttpRequest(url) {
    if (globGetMethod == 0)
        getHtmlHttpRequest(url);
    else
        getJSONHttpRequest(url);
}

function getHtmlHttpRequest(url) {
    var xmlhttp = getXMLHttpRequest();
    console.log("GET via HTML");
    xmlhttp.open("GET", url + "?rtype=HTML", true); //Get-Anfrage wird an Website  und in xmlhttp object gespeichert.
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState != 4) {
            $('posters').innerHTML = 'Seite wird geladen ...';
        }
        if(xmlhttp.readyState == 4 && xmlhttp.status == 200) { //readyState = DONE
            $('posters').innerHTML = xmlhttp.responseText;  //die zuvor gestartete Get-Anfrage wird innerhalb des Posters-div geschrieben.
        }
        $('timestamp').innerHTML = new Date().toString(); //die aktuelle Zeit wird ins timestamp-div geschrieben.
    };
    xmlhttp.send(null);
}

function buildEntries(responseText) {
    let newElements
    let context = "";
    let result = "";
    let id = "";
    let ip = "";
    let text = "";
    let disable_edits="";

    if(object == null) {
        object = JSON.parse(responseText);
    } else {
        newElements =JSON.parse(responseText);
        newElements.forEach(element=> {
            context = element.context;
            id = element.data["id"];
            text = element.data["input"];
            ip = element.data["ip"];
            object[id].data["input"] =element.data["input"];
            object[id].data["ip"] =element.data["ip"];
            object[id].context =element.context;
        })
    }

    result += ("<table border=\"1\" rules=\"none\" cellspacing=\"4\" cellpadding=\"5\">\n");
    for(let i = 0; i < 10; i++)
    {
        if(typeof object[i]["data"] != 'undefined')
        {
            context = object[i].context;
            id = object[i].data["id"];
            text = object[i].data["input"];
            ip = object[i].data["ip"];
            result += ("<tr><td>" + id + "</td>\n");
            result += ("<td>");
            if(ip.toString() === "false")
            {
                disable_edits = " style=\"background-color: #eeeeee;\" readonly";
            } else {
                disable_edits="";
            }

            result += ("<input type=\"text\" size=\"100\" minlength=\"1\" maxlength=\"100\"id=\"input_field_" + id + "\" value=\"" + text + "\"" + disable_edits + ">");
            result += ("</td>");

            if(ip.toString() === "true")
            {
                result += ("<td><button onClick=\"putHttpRequest('" + context + "'," + id + ")\">Update</button></td>");
                result += ("<td><button onClick=\"deleteHttpRequest('" + context + "'," + id + ")\">Delete</button></td>");
            }
        }
        else
        {
            result += ("<tr><td>" + i + "</td><td><input type=\"text\" size=\"100\" minlength=\"1\" maxlength=\"100\"id=\"input_field_" + i + "\" value=\"<empty>\"></td><td></td><td></td>");
        }
        result += ("</tr>");
    }
    result +=("</table>");

    return result;
}

function getJSONHttpRequest(url) {
    var xmlhttp = getXMLHttpRequest();
    console.log("GET via JSON");
    let type="JSON";
    if(object==null) {
        type ="newJSON"
    }
    xmlhttp.open("GET", url + "?rtype="+type, true); //Get-Anfrage wird an Website  und in xmlhttp object gespeichert.
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState != 4) {
            $('posters').innerHTML = 'Seite wird geladen ...';
        }
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) { //readyState = DONE
            $('posters').innerHTML = buildEntries(xmlhttp.responseText);  //die zuvor gestartete Get-Anfrage wird innerhalb des Posters-div geschrieben.
        }
        $('timestamp').innerHTML = new Date().toString(); //die aktuelle Zeit wird ins timestamp-div geschrieben.
    };
    xmlhttp.send(null);
}

function postHttpRequest(url) {
    var xmlhttp = getXMLHttpRequest();
    var content = document.getElementById('contents').value
    xmlhttp.open("POST", url);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    const java = {"input": content};
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState != 4) {

        }
        if(xmlhttp.readyState == 4 && xmlhttp.status == 200) { //readyState = DONE
            getHttpRequest(url)
        }
    };
    xmlhttp.send(JSON.stringify(java));
    console.log("POST: " + JSON.stringify(java));
    getHttpRequest(url);
}

function putHttpRequest(url, id) {
    var xmlhttp = getXMLHttpRequest();
    var content = document.getElementById("input_field_" + id).value
    xmlhttp.open("PUT", url);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    const java = {"text": content, "id": id.toString()};
    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState != 4) {

        }
        if(xmlhttp.readyState == 4 && xmlhttp.status == 200) { //readyState = DONE
            getHttpRequest(url)
        }
    };
    xmlhttp.send(JSON.stringify(java));
    console.log("UPDATE: " + JSON.stringify(java));
    //xmlhttp.send(content + '\n' + id);
}

function deleteHttpRequest(url, id) {
    console.log("Delete")
    let xmlhttp = getXMLHttpRequest();
    xmlhttp.open("DELETE", url /*+ "?entryid=" + id*/);
    //xmlhttp.setRequestHeader("entryid", id)
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    var content = document.getElementById("input_field_" + id.toString()).value;
    const java = {"content": content, "id": id.toString()};

    xmlhttp.onreadystatechange = function() {
        if(xmlhttp.readyState != 4) {
            console.log("Loading")
        }
        if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            getHttpRequest(url);
        }
    };
    xmlhttp.send(JSON.stringify(java));
    console.log("DELETE: " + JSON.stringify(java));
}