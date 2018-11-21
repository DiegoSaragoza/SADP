#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define REQ_BUF_SZ   20
String readString;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 100); 
EthernetServer server(80);  
File webFile;
char HTTP_req[REQ_BUF_SZ] = {0}; 
char req_index = 0; 
boolean c = false;
void Bomba(boolean bomba)
{
  if (bomba = true)
  {
    digitalWrite(2, LOW);
    delay(5000);
    digitalWrite(2, HIGH);
  }
  else if (bomba = false){
    digitalWrite(2,HIGH);
  }
}
void Ciclo(boolean Ciclo)
{
    digitalWrite(2,LOW);
    while (Ciclo = true){
    digitalWrite(3, LOW);
    delay(5000);
    digitalWrite(3, HIGH);
    delay(28800000);
    }
}
      
       
void setup()
{  
    
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);   
    Serial.begin(9600);
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;
    }
    Serial.println("SUCCESS - SD card initialized.");
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return; 
    }
    Serial.println("SUCCESS - Found index.htm file.");
    
    Ethernet.begin(mac, ip);  
    server.begin();           
}
void loop()
{
    int lumi = analogRead(5); 
    EthernetClient client = server.available();
    if (client) {  
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   
                char c = client.read(); 
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          
                    req_index++;
                }
                Serial.print(c);
                if (c == '\n' && currentLineIsBlank) {
                  
                     if (StrContains(HTTP_req, "GET / ")|| StrContains(HTTP_req, "GET /index.htm")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("index.htm");       
                    }
                    else if (StrContains(HTTP_req, "GET /lon.htm") || StrContains(HTTP_req, "GET /loff.htm") || StrContains(HTTP_req, "GET /gerencia.htm") || StrContains(HTTP_req, "GET /home.htm") || StrContains(HTTP_req, "GET /boff.htm") || StrContains(HTTP_req, "GET /bon.htm") || StrContains(HTTP_req, "GET /son.htm")) {
                      if (StrContains(HTTP_req, "GET /lon.htm")){
                        digitalWrite(2, LOW);
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("lon.htm");
                      }else if (StrContains(HTTP_req, "GET /loff.htm")){
                        digitalWrite(2, HIGH);
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("loff.htm");
                      }
                    else if (StrContains(HTTP_req, "GET /home.htm")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("home.htm");        
                    }
                    else if (StrContains(HTTP_req, "GET /gerencia.htm")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("gerencia.htm");        
                        client.print("<p hidden id='dado'>" + String(lumi) + "</p>"); 
                    }
                    else if (StrContains(HTTP_req, "GET /boff.htm")){
                        boolean bomba = false;
                        Bomba(bomba);
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("boff.htm");
                      }
                      else if (StrContains(HTTP_req, "GET /bon.htm")){
                        boolean bomba = true;
                        Bomba(bomba);
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("bon.htm");
                      }   
                      else if (StrContains(HTTP_req, "GET /son.htm")){
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("son.htm");
                        c = true;
                        Ciclo(c);
                      }                         
                   }
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read()); 
                        }
                        webFile.close();
                    }

                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        } 
        delay(1);      
        client.stop(); 
    }
}
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;
    len = strlen(str);    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }
    return 0;
}
