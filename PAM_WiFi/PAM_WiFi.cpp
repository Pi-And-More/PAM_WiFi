#include <PAM_WiFi.h>
#include <ESP8266WiFi.h>

String WiFiConnect (char* ssid, char* password) {
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  return WiFi.localIP().toString();
}

void doNothing (String webLine) {
}

String getURLParse (char* host, char* url, bool fullContent, void (*g)(String l), int port=80) {
  String lastline;
  boolean inPage;
  inPage = false;
  WiFiClient client;
  if (!client.connect(host,port)) {
    return "ERR:noconnect";
  }
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return "ERR:timeout";
    }
  }
  lastline = "";
  while(client.available()){
    String line = client.readStringUntil('\r');
    line.replace("\n","");
    if (line.length()>0) {
      lastline = line;
    }
    if (fullContent || inPage) {
      g(line);
    }
    if (line.length()==0) {
      inPage = true;
    }
  }
  return lastline;
}

String getURLFullParse (char* host, char* url, void (*g)(String l), int port=80) {
  return getURLParse(host,url,true,g,port);
}

String getURLBodyParse (char* host, char* url, void (*g)(String l), int port=80) {
  return getURLParse(host,url,false,g,port);
}

String getURL (char* host, char* url, int port=80) {
  return getURLParse(host,url,false,doNothing,port);
}
