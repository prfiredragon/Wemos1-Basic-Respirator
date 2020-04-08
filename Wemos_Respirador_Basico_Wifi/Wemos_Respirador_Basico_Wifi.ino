/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "RESPIRADOR A"
#define APPSK  "password"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

void handleRoot();              // function prototypes for HTTP handlers
void onUP();
void onDOWN();
void offUP();
void offDOWN();
void handleNotFound();
int segundos_encendido = 4;
int segundos_apagado = 4;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/onUP", HTTP_POST, onUP);  // Call the 'onUP' function when a POST request is made to URI "/onUP"
  server.on("/onDOWN", HTTP_POST, onDOWN);  // Call the 'onDOWN' function when a POST request is made to URI "/onDOWN"
  server.on("/offUP", HTTP_POST, offUP);  // Call the 'offUP' function when a POST request is made to URI "/offUP"
  server.on("/offDOWN", HTTP_POST, offDOWN);  // Call the 'offDOWN' function when a POST request is made to URI "/offDOWN"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();
  Serial.println("HTTP server started");
}
int espera_apagado = (millis()+(segundos_apagado*1000)), espera_encendido=(millis()+(segundos_encendido*1000));
bool encendido = false;
void loop() {
  server.handleClient();
  if(encendido == true && millis() > espera_encendido)
  {
    digitalWrite(5, LOW);
    espera_apagado = (millis()+(segundos_apagado*1000));
    encendido = false;
  }
  if(encendido == false && millis() > espera_apagado )
  {
    digitalWrite(5, HIGH);
    espera_encendido=(millis()+(segundos_encendido*1000));
    encendido = true;
  }
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=2\"><p>ON - Time " + String(segundos_encendido) + " Seconds</p><p>OFF - Time " + String(segundos_apagado) + " Seconds</p><form action=\"/onUP\" method=\"POST\"><input type=\"submit\" value=\"Add one Sec to ON\"></form><form action=\"/onDOWN\" method=\"POST\"><input type=\"submit\" value=\"Remove one Sec to ON\"></form><form action=\"/offUP\" method=\"POST\"><input type=\"submit\" value=\"Add one Sec to OFF\"></form><form action=\"/offDOWN\" method=\"POST\"><input type=\"submit\" value=\"Remove one Sec to OFF\"></form>");
}

void onUP() {                          
  segundos_encendido++;                     // Add a second
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void onDOWN() {                          
  segundos_encendido--;                     // Remove a second
  if(segundos_encendido < 0)
     segundos_encendido = 0;
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void offUP() {                          
  segundos_apagado++;                     // Add a second
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void offDOWN() {                          
  segundos_apagado--;                     // Remove a second
  if(segundos_apagado < 0)
     segundos_apagado = 0;
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
