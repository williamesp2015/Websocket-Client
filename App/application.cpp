#include <user_config.h>
#include <SmingCore/SmingCore.h>
//#include <ESP8266WiFi.h>
#include <Libraries/Websocket2/WifiClient.h>
#include <Libraries/Websocket2/WebSocketClient.h>
// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
	#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
	#define WIFI_PWD "PleaseEnterPass"
#endif

char path[] = "/";
char host[] = "echo.websocket.org";

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

#define LED_PIN 2 // GPIO2

Timer procTimer;
bool state = true;
///////////////////////
void loop() {
  String data;
	digitalWrite(LED_PIN, state);
	state = !state;
  if (client.connected()) {

    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }

    // capture the value of analog 1, send it along
    pinMode(1, INPUT);
    data = String(analogRead(1));

    webSocketClient.sendData(data);

  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }

  // wait to fully let the client disconnect
  delay(3000);

}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Enable debug output to serial
	pinMode(LED_PIN, OUTPUT);//LED

	WifiStation.enable(true);
	WifiStation.config(WIFI_SSID, WIFI_PWD);
	WifiAccessPoint.enable(false);
	 delay(5000);


	  // Connect to the websocket server
	  if (client.connect("echo.websocket.org", 80)) {
	    Serial.println("Connected");
	  } else {
	    Serial.println("Connection failed.");
	    while(1) {
	      // Hang on failure
    			    digitalWrite(LED_PIN, 0x1);//LED ON
    			    delay(500);
    			    digitalWrite(LED_PIN, 0x0);//LED OFF
	              }
	  }

	  // Handshake with the server
	  webSocketClient.path = path;
	  webSocketClient.host = host;
	  if (webSocketClient.handshake(client)) {
	    Serial.println("Handshake successful");
	  } else {
	    Serial.println("Handshake failed.");
	    while(1) {
	      // Hang on failure
	    		digitalWrite(LED_PIN, 0x1);//LED ON
	    		delay(100);
	    		digitalWrite(LED_PIN, 0x0);//LED OFF
	    }
    }
////loop every 10 msec
		procTimer.initializeMs(10, loop).start();//10 msec

}
