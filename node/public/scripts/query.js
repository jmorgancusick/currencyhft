
function getTable(){
    document.getElementById("stockTable").innerHTML="";

    name = document.getElementById("tableName").value;
    console.log("getTable called with name: "+name);
    ajax("db/"+name, populateTable);

}

function populateTable(data){
    console.log("in function!");
    console.log(data);

    tableStr = "<tr><th>ID</th><th>Val</th></tr>"

    for(i = 0; i < data.length; i++){
	tableStr+="<tr><th>"+data[i]["id"]+"</th><th>"+data[i]["close"]+"</th></tr>";
    }
    
    document.getElementById("stockTable").innerHTML=tableStr;
}


function ajax(url, callback) {
    var xmlhttp = new XMLHttpRequest();
	
    xmlhttp.onreadystatechange = function() {
	if (xmlhttp.readyState == XMLHttpRequest.DONE ) {
	    if (xmlhttp.status == 200) {
		console.log(JSON.parse(xmlhttp.responseText));
		callback(JSON.parse(xmlhttp.responseText));
	    }
	    else if (xmlhttp.status == 400) {
		alert('There was an error 400');
	    }
	    else {
		alert('something else other than 200 was returned');
	    }
	}
    };
    
    xmlhttp.open("GET", url, true);
    xmlhttp.send();
}
