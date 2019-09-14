const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<body>

  <div>
    <button type="button" onclick="turnLed(1)">LED ON</button>
    <button type="button" onclick="turnLed(0)">LED OFF</button><br>
    <input id="slideBrightness" type="range" min="1" max="255" step="1" value="10"
      oninput="setLedBrightness(this.value)"> <br>
    <input id="slideRedValue" type="range" min="1" max="255" step="1" value="10"
      oninput="setLedChannelValue(this.id, this.value)"><br>
    <input id="slideGreenValue" type="range" min="1" max="255" step="1" value="10"
      oninput="setLedChannelValue(this.id, this.value)"><br>
    <input id="slideBlueValue" type="range" min="1" max="255" step="1" value="10"
      oninput="setLedChannelValue(this.id, this.value)"><br>
  </div>

  <div>
    LED State is : <span id="ledState">NA</span><br>
    Brightness is : <span id="ledBrightness">NA</span><br>
    Red Value is : <span id="ledRedValue">NA</span><br>
    Green Value is : <span id="ledGreenValue">NA</span><br>
    Blue Value is : <span id="ledBlueValue">NA</span><br>
  </div>


  <script>

    function turnLed(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ledState").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "turnLed?ledState=" + value, true);
      xhttp.send();
    }

    function setLedBrightness(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ledBrightness").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "setLedBrightness?ledBrightness=" + value, true);
      xhttp.send();
    }

    function setLedChannelValue(id, value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          if (id == "slideRedValue")
            document.getElementById("ledRedValue").innerHTML = this.responseText;
          else if (id == "slideGreenValue")
            document.getElementById("ledGreenValue").innerHTML = this.responseText;
          else if (id == "slideBlueValue")
            document.getElementById("ledBlueValue").innerHTML = this.responseText;
        }
      };
      var argColor = "";
      if (id == "slideRedValue")
        argColor = "ledRedValue=";
      else if (id == "slideGreenValue")
        argColor = "ledGreenValue=";
      else if (id == "slideBlueValue")
        argColor = "ledBlueValue=";

      xhttp.open("GET", "setLedChannelValue?" + argColor + value, true);
      xhttp.send();
    }

  </script>
</body>

</html>
)=====";
