import React, { Component } from 'react';
import { ExpoConfigView } from '@expo/samples';
import { TextInput } from 'react-native-gesture-handler';
import { Platform, Text, Dimensions, ScrollView, StyleSheet, View } from 'react-native';
import Constants from 'expo-constants';
import PropTypes from 'prop-types';

export default class SettingsScreen extends Component {

  constructor(props) {
    super(props);
    this.state = { ipAddress: "" }
  }

  setIpAddress(objIpAddress) {
    this.setState(objIpAddress);                                   // Sets state value
    this.props.screenProps.setIpAddress(objIpAddress.ipAddress);   // Calls parent's callback and sets parent's state value
  }

  render() {
    return (
      <ScrollView style={styles.container} >
   
         {/* Pass setIpAddress from App.js to child component */}
        <MyList callback={this.props.screenProps.setIpAddress}> </MyList> 
      </ScrollView>
    );
  }
}

class MyList extends Component {
  render() {
    return (
      <View>
        {/* Pass setIpAddress from App.js to child's child component */}
        <MyItem name='IP Address' text="" keyboard="numeric" callback={this.props.callback}> </MyItem> 
        <MyItem name='Username'> </MyItem>
        <MyItem name='Password'> </MyItem>
      </View>
    );
  }
}

class MyItem extends Component {

  constructor(props) {
    super(props);
    this.state = { textInput: props.text }
  }

  render() {
    let onChangeTextCall = (objText) => {       
      this.setState(objText)
      if (this.props.callback != undefined) {     // If callback prop was defined, call it
        this.props.callback(objText.textInput);   // Sets ipAdress prop visible in all Tabs
      }
    };

    return (
      <View>
        <Text>
          {this.props.name}
        </Text>
        <TextInput
          value={this.state.textInput}
          placeholder={this.props.name}
          keyboardType={this.props.keyboard}
          onChangeText={text => onChangeTextCall({ textInput: text })}/>
        <ColoredLine color="black" />
      </View>
    );
  }
}

MyItem.propTypes = {
  name: PropTypes.string,
  text: PropTypes.string,
  keyboard: PropTypes.string,
}

MyItem.defaultProps = {
  keyboard: "default",
  callback: undefined
}

const ColoredLine = ({ color }) => (
  <View
    style={{
      borderBottomColor: color,
      borderBottomWidth: StyleSheet.hairlineWidth,
    }}
  />
);

SettingsScreen.navigationOptions = {
  title: 'Settings',
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    marginTop: Constants.statusBarHeight,
    marginHorizontal: 16,
  },
  title: {
    textAlign: 'center',
    marginVertical: 8,
  },
  fixToText: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center'
  },
  separator: {
    marginVertical: 8,
    borderBottomColor: '#737373',
    borderBottomWidth: StyleSheet.hairlineWidth,
  },
  developmentModeText: {
    marginBottom: 20,
    color: 'rgba(0,0,0,0.4)',
    fontSize: 14,
    lineHeight: 19,
    textAlign: 'center',
  },
  contentContainer: {
    paddingTop: 30,
  },
  welcomeContainer: {
    alignItems: 'center',
    marginTop: 10,
    marginBottom: 20,
  },
  welcomeImage: {
    width: 100,
    height: 80,
    resizeMode: 'contain',
    marginTop: 3,
    marginLeft: -10,
  },
  getStartedContainer: {
    alignItems: 'center',
    marginHorizontal: 50,
  },
  homeScreenFilename: {
    marginVertical: 7,
  },
  codeHighlightText: {
    color: 'rgba(96,100,109, 0.8)',
  },
  codeHighlightContainer: {
    backgroundColor: 'rgba(0,0,0,0.05)',
    borderRadius: 3,
    paddingHorizontal: 4,
  },
  getStartedText: {
    fontSize: 17,
    color: 'rgba(96,100,109, 1)',
    lineHeight: 24,
    textAlign: 'center',
  },
  tabBarInfoContainer: {
    position: 'absolute',
    bottom: 0,
    left: 0,
    right: 0,
    ...Platform.select({
      ios: {
        shadowColor: 'black',
        shadowOffset: { width: 0, height: -3 },
        shadowOpacity: 0.1,
        shadowRadius: 3,
      },
      android: {
        elevation: 20,
      },
    }),
    alignItems: 'center',
    backgroundColor: '#fbfbfb',
    paddingVertical: 20,
  },
  tabBarInfoText: {
    fontSize: 17,
    color: 'rgba(96,100,109, 1)',
    textAlign: 'center',
  },
  navigationFilename: {
    marginTop: 5,
  },
  helpContainer: {
    marginTop: 15,
    alignItems: 'center',
  },
  helpLink: {
    paddingVertical: 15,
  },
  helpLinkText: {
    fontSize: 14,
    color: '#2e78b7',
  },
});
