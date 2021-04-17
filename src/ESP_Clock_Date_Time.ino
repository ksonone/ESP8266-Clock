/**********************************************************
*
*          ESP Clock using API and ESP8266
*             Author : Kapil H. Sonone
***********************************************************/

#include <ESP8266WiFi.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

//Library for JSON Response
#include <ArduinoJson.h>


String payload; //To store the JSON object as string
const char* host = "worldtimeapi.org";
const int httpPort = 80;
String urlGetData = "GET /api/timezone/Asia/Kolkata";
WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected. :)");
}

void loop() {
  // put your main code here, to run repeatedly:
  getAllData();
}

void getAllData() {
  if (client.connect(host, httpPort)) {

    // This will send the request to the server
    client.print(urlGetData);
    client.println(" HTTP/1.1");
    client.println("Host: worldtimeapi.org");
    client.println("User-Agent: Kapil-ESP/1.0");
    client.println("Connection: close");
    client.println();
    while (!client.available());
    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String line = client.readString();
      //Serial.println(line.length());
      //Serial.println(line);
      //String head = line.substring(0, 329);
      //String data = line.substring(332, 679);
      //Serial.println(head);
      payload = line.substring(332, 679);
    }
    const size_t capacity = JSON_OBJECT_SIZE(15) + 340;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject& root = jsonBuffer.parseObject(payload);
    
    //Address the value sin to desired variables
    const char* datetime = root["datetime"]; // "2019-10-24T22:28:28.848935+05:30"
    String k_datetime = String(datetime);
    int dates = (k_datetime.substring(8,10)).toInt();
    int months = (k_datetime.substring(5,7)).toInt();
    int years = (k_datetime.substring(0,4)).toInt();

    int hrs = (k_datetime.substring(11,13)).toInt();
    int mins = (k_datetime.substring(14,16)).toInt();
    int secs = (k_datetime.substring(17,19)).toInt();

    String k_date = "Date : " + String(dates) + "/" + String(months) + "/" + String(years);
    String k_time = "Time : " + String(hrs) + ":" + String(mins) + ":" + String(secs);
    Serial.println(k_date);
    Serial.println(k_time);
    //Serial.print(dates);
    //Serial.print(months);
    //Serial.println(years);

    //Serial.print(hrs);
    //Serial.print(mins);
    //Serial.println(secs);
  }
  client.flush();
  client.stop();
}
