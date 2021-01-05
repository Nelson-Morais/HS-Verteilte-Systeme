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