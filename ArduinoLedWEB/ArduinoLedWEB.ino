
#include <SPI.h>

#include <Ethernet.h> 
 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

byte ip[] = { 192, 168, 0, 17 }; // Define o endereço IP.


EthernetServer server(3214);  
 
String readString;
int Pin = 13; 
 
void setup(){
 
  pinMode(Pin, OUTPUT); 
  Ethernet.begin(mac, ip); 
 
  server.begin();  
}
 
void loop(){
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        if (readString.length() < 100) {
          readString += c;             
        }

        if (c == '\n') {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
    
       
    
          client.println("<HTML>");
          client.println("<BODY>");
          client.println("<H1>Acende LED</H1>");
          client.println("<H1>Projeto basico para demonstracao com Shield Ethernet</H1>");
          client.println("<hr />");
          client.println("<br />");
          
          client.println("<a href=\"/facacomarduino/LedOn\"\">Acender led</a>");
          client.println("<a href=\"/facacomarduino/LedOff\"\">Apagar led</a><br />");   
          
          client.println("</BODY>");
          client.println("</HTML>");
          
          delay(1);
          client.stop();
          
          if(readString.indexOf("facacomarduino/LedOn") > 0)
          {
            digitalWrite(Pin, HIGH); 
          }
          else {
            if(readString.indexOf("facacomarduino/LedOff") > 0)
            {
              digitalWrite(Pin, LOW); 
            }
          }
          readString="";    
        }
      }
    }
  }
}
