#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"
#define light_switch_1 33
#define light_switch_2 32
#define light_switch_3 25

int j,i;
 bool led_status[8][3]={{false,false,false}, 
                        {false, false, true},
                        {false, true, true},
                        {true, true,true},
                        {true, true, false},
                        {true, false, false},
                        {true, false, true}};
void setup() {
  // put your setup code here, to run once:
  pinMode(light_switch_1, OUTPUT);
  pinMode(light_switch_2, OUTPUT);
  pinMode(light_switch_3, OUTPUT);

  Serial.begin(9600);
	
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WiFi_SSID, WiFi_PASS);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    

    http.begin(endpoint);
    
    //http.addHeader("Content-length", "76");
    
   
    for(i=0;i<8;i++)
    {
        http.addHeader("X-API-Key",apikey);
        http.addHeader("Content-type", "application/json");

        StaticJsonDocument<76>doc;
         String httpRequestData;

        digitalWrite(light_switch_1, led_status[i][0]);
        digitalWrite(light_switch_2, led_status[i][1]);
        digitalWrite(light_switch_3, led_status[i][2]);
        
         
        doc["light_switch_1"] = led_status[i][0];
        doc["light_switch_2"] = led_status[i][1];
        doc["light_switch_3"] = led_status[i][2];

        serializeJson(doc, httpRequestData);
	      int httpResponseCode= http.PUT(httpRequestData);
        String http_response;
        if(httpResponseCode>0)
    {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response from server");
      http_response=http.getString();
      Serial.println(http_response);

    }
    else{
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
     delay(2000);
    
      }
      http.end();
    }   
  }

  