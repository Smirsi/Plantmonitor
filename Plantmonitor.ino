/* Temperaturanzeige 
 * Version 1.0 vom 15.05.2021
 * by Der Bastelbruder
 * https://www.youtube.com/channel/UCMGqOyxbKXgnbDSe7DfkLow
 * Code kann mit Quellenangabe frei verwendet werden;)
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

const char* ssid = "FRITZ!Box 7530 DG";
const char* password = "Ace-2468";
#define DHTPin D1
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);

ESP8266WebServer server(80);

void indexHTML() {    
  server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Temperaturanzeige</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <style>\
  body { background-color: #585858; font-size: 50px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 40px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  #temp { width: 230px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>Raumtemperatur</h1>\
  <h2>(by Smirsi)</h2>\
  <table><tr><td>Aktuelle Temperatur:&nbsp;</td>\
  <td id='temp'><span id='TempWert'>-</span>°C</td></tr></table>\
  <script>\
  setInterval(function() {\
    getData();\
  }, 1000);\
  function getData() {\
    var xhttp = new XMLHttpRequest();\
    xhttp.onreadystatechange = function() {\
      if (this.readyState == 4 && this.status == 200) {\
        document.getElementById('TempWert').innerHTML = this.responseText;\
      }\
    };\
    xhttp.open('GET', 'TempWeb', true);\
    xhttp.send();\
  }\
  </script>\
  </body>\
  </html>\
  ");
}

void TempMessung() {
  String TempWert = String(dht.readTemperature(),1); 
  server.send(200, "text/plane", TempWert);
}

void setup(void){
  dht.begin();
  Serial.begin(9600);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Verbunden mit ");
  Serial.println(ssid);
  Serial.print("IP Adresse: ");
  Serial.println(WiFi.localIP());
 
  server.on("/", indexHTML);
  server.on("/TempWeb", TempMessung);

  server.begin(); 
  Serial.println("Webserver ist gestartet");
}

void loop(void){
  server.handleClient();
}
