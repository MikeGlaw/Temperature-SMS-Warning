/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiSSLClient.h>
#include <TembooSSL.h>
#include "TembooAccount.h" // Contains Temboo account information
/********************************************************************/

/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
/********************************************************************/

// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/

WiFiSSLClient client;





void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  //Starting the Library
  sensors.begin(); 
  delay(4000);
  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  Serial.println("Setup complete.\n");
}

void loop() {

  //**** The delays are set for 10 seconds, however these should be changed based upon one's needs. ****
  
  sensors.requestTemperatures();
  Serial.print("Temperature is: "); 
  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
  delay(1000); 
  
  //Insert your own temperatures in the if and else-if statement
  if (sensors.getTempCByIndex(0) <= 20.0) {

    TembooChoreoSSL SendSMSChoreo(client);

    // Invoke the Temboo client
    SendSMSChoreo.begin();

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
    SendSMSChoreo.setDeviceType(TEMBOO_DEVICE_TYPE);

    // Set Choreo inputs
    String AuthTokenValue = "YourAuthToken";
    SendSMSChoreo.addInput("AuthToken", AuthTokenValue);
    String FromValue = "YourTwilioNumber";
    SendSMSChoreo.addInput("From", FromValue);
    String ToValue = "NumberToMessage";
    SendSMSChoreo.addInput("To", ToValue);
    String BodyValue = "Your temperature is too low";
    SendSMSChoreo.addInput("Body", BodyValue);
    String AccountSIDValue = "YourSIDValue";
    SendSMSChoreo.addInput("AccountSID", AccountSIDValue);

    // Identify the Choreo to run
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");

    // Run the Choreo; when results are available, print them to serial
    SendSMSChoreo.run();

    while(SendSMSChoreo.available()) {
      char c = SendSMSChoreo.read();
      Serial.print(c);
    }
    SendSMSChoreo.close();
    delay(10000);
  }else if (sensors.getTempCByIndex(0) >= 30.0){

      TembooChoreoSSL SendSMSChoreo(client);

    // Invoke the Temboo client
      SendSMSChoreo.begin();

      // Set Temboo account credentials
      SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
      SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
      SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
      SendSMSChoreo.setDeviceType(TEMBOO_DEVICE_TYPE);
  
      // Set Choreo inputs
      String AuthTokenValue = "YourAuthToken";
      SendSMSChoreo.addInput("AuthToken", AuthTokenValue);
      String FromValue = "YourTwilioNumber";
      SendSMSChoreo.addInput("From", FromValue);
      String ToValue = "NumberToMessage";
      SendSMSChoreo.addInput("To", ToValue);
      String BodyValue = "Your Temperature is too high.";
      SendSMSChoreo.addInput("Body", BodyValue);
      String AccountSIDValue = "YourSIDValue";
      SendSMSChoreo.addInput("AccountSID", AccountSIDValue);
  
      // Identify the Choreo to run
      SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
  
      // Run the Choreo; when results are available, print them to serial
      SendSMSChoreo.run();
  
      while(SendSMSChoreo.available()) {
        char c = SendSMSChoreo.read();
        Serial.print(c);
      }
      delay(10000);
    }else{
        delay(10000);
      }
}
