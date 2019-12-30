import React, { Component } from 'react';
import { Dimensions, ScrollView, StyleSheet, View } from 'react-native';
import { ExpoLinksView } from '@expo/samples';
import { ColorPicker } from 'react-native-color-picker'
import { ColorWheel } from 'react-native-color-wheel';

var HsvToRgb = require('../helper/HsvToRgb.js');


export default class ColorsScreen extends Component {

  changeColor(value) {
    this.setAnimationRequest(0);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        //document.getElementById("ledState").innerHTML = this.responseText;
      }
      if (this.status === 200) {
        console.log('success', this.responseText);
      } else {
        console.log('error', this.responseText);
      }
    };
    if (value.h < 0)
      value.h = 360 + value.h;
    var color = HsvToRgb.getRGB(value.h / 360, value.s / 100, value.v / 100);
    xhttp.open("GET", "http://" + this.props.screenProps.ipAddress + "/setLedChannelValue?ledRedValue=" + color.r + "&ledGreenValue=" + color.g + "&ledBlueValue=" + color.b, true);
    xhttp.send();
  }

  setAnimationRequest(value) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        //document.getElementById("ledState").innerHTML = this.responseText;
      }
      if (this.status === 200) {
        console.log('success', this.responseText);
      } else {
        console.log('error', this.responseText);
      }
    };
    xhttp.open("GET", "http://" + this.props.screenProps.ipAddress + "/setAnimationNumber?animationNumber=" + value, true);
    xhttp.send();
  }

  render() {
  return (

    <View style={{flex: 0.6, paddingTop: 50}}>
       <ColorWheel
      initialColor="#ee0000"
      // onColorChange={color => console.log({color})}
      onColorChangeComplete={color => this.changeColor(color)}
      style={{width: Dimensions.get('window').width}}
      thumbStyle={{ height: 30, width: 30, borderRadius: 30}}
    />
  </View>

  );
  }
}

ColorsScreen.navigationOptions = {
  title: 'Colors',
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    paddingTop: 55,
    backgroundColor: '#fff',
  },
});
