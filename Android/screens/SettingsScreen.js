import React, {Component} from 'react';
import { ExpoConfigView } from '@expo/samples';
import { TextInput } from 'react-native-gesture-handler';
import { Dimensions, ScrollView, StyleSheet, View } from 'react-native';

export default class SettingsScreen extends Component {
  /**
   * Go ahead and delete ExpoConfigView and replace it with your content;
   * we just wanted to give you a quick view of your config.
   */
  a = function (text)
  {
    console.log(text);
  }
  render() {
    return (

      <ScrollView>
        <View>
          <TextInput style={
            { height: 80 }}  
            onChangeText={text => this.a(text)}> 
             
            </TextInput>
          <TextInput style={{ height: 40 }}> Second input</TextInput>
        </View>

      </ScrollView>
    );
  }
}

// SettingsScreen.navigationOptions = {
//   title: 'app.json',
// };
