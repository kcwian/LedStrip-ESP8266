import React, { Component } from 'react';
import { Dimensions, ScrollView, StyleSheet, View, Text,   Animated, TouchableOpacity } from 'react-native';
import { ExpoLinksView } from '@expo/samples';

var HsvToRgb = require('../helper/HsvToRgb.js');


export default class AnimationsScreen extends Component {

  constructor(props)
  {
    super(props);
    this.state = { animationNumber: 0 }
  }

  nextAnimation() {
    this.setState({ animationNumber: this.state.animationNumber + 1 }, () => { this.setAnimationRequest(this.state.animationNumber) });
  }

  previousAnimation() {
    this.setState({ animationNumber: this.state.animationNumber - 1 }, () => { this.setAnimationRequest(this.state.animationNumber) });
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

      <View style={{ flex: 0.6, paddingTop: 12 }}>
        <Text style={styles.title}> Choose your Animation </Text>

        <Text style={styles.animationNumber}> {this.state.animationNumber} </Text>

        <View style={styles.rowButtons}>
          <TouchableOpacity style={styles.btn} onPress={() => this.previousAnimation()}>
            <Text style={styles.textBtn}>Previous</Text>
          </TouchableOpacity>

          <TouchableOpacity style={styles.btn} onPress={() => this.nextAnimation()}>
            <Text style={styles.textBtn}>Next</Text>
          </TouchableOpacity>
        </View>

      </View>

  );
  }
}

AnimationsScreen.navigationOptions = {
  title: 'Animations',
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    paddingTop: 55,
    backgroundColor: '#fff',
    justifyContent: 'center',
  },
  rowButtons: {
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center'
  },
  title: {
    fontSize: 17,
    color: 'rgba(96,100,109, 1)',
    lineHeight: 24,
    textAlign: 'center',
  },
  btn: {
    backgroundColor: "#480032",
    width: 100,
    height: 40,
    padding: 3,
    justifyContent: "center",
    borderRadius: 6
  },
  textBtn: {
    color: "#f4f4f4",
    fontWeight: "bold",
    textAlign: "center"
  },
  animationNumber: {
    fontSize: 54,
    textAlign: "center",
    justifyContent: 'center'
  }
});
