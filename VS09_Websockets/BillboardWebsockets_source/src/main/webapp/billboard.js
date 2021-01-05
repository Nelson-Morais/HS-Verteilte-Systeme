var websocket = new WebSocket("ws://" + location.host + "/BillboardWebsockets_war_exploded/billboard");
let billboard
function $(id) {
    return document.getElementById(id);
}

websocket.onmessage = function (event) {
    $('posters').innerHTML = buildEntries(event.data);
}

websocket.onerror = function (event) {
    $('posters').innerText = "An error occurred.";
    console.log("Error");
}

function postEntry(url) {
    var content = document.getElementById('contents').value
    console.log("POST: " + content)

    if(websocket.readyState === websocket.OPEN) {
        websocket.send("*P*" + content);
        console.log("POST: " + content)
    }

}

function buildEntries(responseText) {
    let newElement
    let context = "";
    let result = "";
    let id = "";
    let ip = "";
    let text = "";
    let disable_edits="";

    if(billboard == null) {
        billboard = JSON.parse(responseText);
    } else {
        newElement =JSON.parse(responseText);
            context = newElement.context;
            id = newElement.data["id"];
            text = newElement.data["input"];
            ip = newElement.data["ip"];
            billboard[id].data["input"] =newElement.data["input"];
            billboard[id].data["ip"] =newElement.data["ip"];
            billboard[id].context =newElement.context;
    }

    result += ("<table border=\"1\" rules=\"none\" cellspacing=\"4\" cellpadding=\"5\">\n");
    for(let i = 0; i < 10; i++)
    {
        if(typeof billboard[i]["data"] != 'undefined')
        {
            context = billboard[i].context;
            id = billboard[i].data["id"];
            text = billboard[i].data["input"];
            ip = billboard[i].data["ip"];
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
                result += ("<td><button onClick=\"updateEntry('" + context + "'," + id + ")\">Update</button></td>");
                result += ("<td><button onClick=\"deleteEntry('" + context + "'," + id + ")\">Delete</button></td>");
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

function updateEntry(url, id) {
    var content = document.getElementById("input_field_" + id).value;
    console.log("UPDATE " + id + "to " + content);
    const json = {"text": content, "id": id.toString()};
    if(websocket.readyState === websocket.OPEN) {
        websocket.send("*U*" + JSON.stringify(json));
    }

}
function deleteEntry(url, id) {
    console.log("DELETE Entry No. " + id);
    if(websocket.readyState === websocket.OPEN) {
        websocket.send("*D*" +id);
    }
}