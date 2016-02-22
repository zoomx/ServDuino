#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  192,168,1, 177 };

Server server(80);

int LED = 19; //Analog pin 5 (yes it works as a digital pin too)

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(LED, OUTPUT);
}
#define BUFSIZ 100  //Buffer size for getting data
char clientline[BUFSIZ];  //string that will contain command data
int index = 0;  //clientline index
void loop()
{
  index=0;  //reset the clientline index
  Client client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(index<BUFSIZ)  //Only add data if the buffer isn't full.
        {
          clientline[index]=c;
          index++;
        }
        if (c == '\n' && currentLineIsBlank)
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<h1><center>LED Control</h1></center><br><center><form method=get action=/?><input type=radio name=L1 value=1>On<br><input type=radio name=L1 value=0>Off<br><input type=submit value=submit></form></center>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
        if(strstr(clientline,"/?L1=1")!=0) {  //look for the command to turn the led on
          digitalWrite(LED, 1);  //turn the led on
        } else if(strstr(clientline,"/?L1=0")!=0) {  //look for command to turn led off. Note: If led is on, it will stay on until command is given to turn it off, even if multiple computers are on the site
          digitalWrite(LED, 0);  //turn led off
        }
      }
    }
    delay(1);
    client.stop();
  }
}