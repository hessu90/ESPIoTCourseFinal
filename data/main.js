var intervalTime = 1000 * 60 * 60 * 12;

var tempo = setInterval(function () {
    loadMoisture();
}, intervalTime);


document.getElementById('btn').addEventListener('click', () => {
    loadMoisture();
    
})


function loadMoisture() {
    document.getElementById("loading").innerHTML = "Loading..."
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("moisture").innerHTML = this.responseText;
            document.getElementById("loading").innerHTML = ""
        }
    };
    xhttp.open("GET", "/moisture", true);
    xhttp.send();
    
}