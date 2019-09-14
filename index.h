const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<body>

  <div>
    <button type="button" onclick="turnLed(1)">LED ON</button>
    <button type="button" onclick="turnLed(0)">LED OFF</button><br>
    <input id="slide" type="range" min="1" max="255" step="1" value="10" oninput="setLedBrightness(this.value)">
  </div>

  <div>
    LED State is : <span id="ledState">NA</span>
    Brightness is : <span id="ledBrightness">NA</span>
  </div>


  <script>

    function turnLed(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ledState").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "turnLed?ledState=" + value, true);
      xhttp.send();
    }

    function setLedBrightness(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ledBrightness").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "setLedBrightness?ledBrightness="+value, true);
      xhttp.send();
    }

  </script>
</body>

</html>
)=====";