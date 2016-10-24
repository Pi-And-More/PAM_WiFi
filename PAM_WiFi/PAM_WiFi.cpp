//
// Include the PAM_WiFi header
//
// created by PI and more, piandmore@j-tools.net
//
#include <PAM_WiFi.h>

//
// Include the ESP8266WiFi library
//
#include <ESP8266WiFi.h>

//
// wifiConnect will connect you to wifi network using
// the supplied credentials. It will than wait to connect
// for up to 5 times 10 seconds. 
//
void wifiConnect (String ssid, String password) {
  //
  // Convert ssid and password from String to a Char array
  //
  char xssid[50];
  ssid.toCharArray(xssid,50);
  char xpassword[50];
  password.toCharArray(xpassword,50);

  bool connected = false;
  byte cnt2 = 0;
  //
  // Wait in the loop while you are not connected to a wifi, boolean connected
  // accesspoint but a maximum of 5 iterations, byte cnt2.
  //
  while (!connected && cnt2<5) {
    //
    // Try to connect to a wifi accesspoint
    //
    WiFi.begin(xssid,xpassword);
    byte cnt = 0;

    //
    // Wait in the loop while we are not connected, WiFi.status()
    // but a maximum of 20 iterations, byte 20 after which we
    // disconnect from the wifi and try again.
    //
    while (WiFi.status()!=WL_CONNECTED && cnt<20) {
      delay(500);
      Serial.print(".");
      cnt++;
    }
    if (WiFi.status()!=WL_CONNECTED) {
      WiFi.disconnect();
      Serial.println();
    } else {
      connected = true;
    }
    cnt2++;
  }
}

//
// The doNothing function which is used as a parameter if we do not care about
// the webpage result, aka fire and forget
//
void doNothing (String webLine) {
}

//
// There are four members in the family of functions for getURL:
// - getURLParse which is the main function that the others use
// - getURL which is a fire and forget request type. It does return the last line for error messages, if needed
// - getURLBodyParse which sends all the body lines through a supplied function
// - getURLFullParse which sends all lines, header and body, through a supplied function
//
// Each of these functions exists in two variations, one with a supplied port and one without
// in which case port 80 is assumed.
//

//
// getURLParse, get the url url from the host host on port port and send each line 
// through the function g. The boolean fullContent determines whether only the header lines
// or all ilnes are send through to the function g
//
String getURLParse (char* host, String url, bool fullContent, void (*g)(String l), int port) {
  String lastline;
  boolean inPage;
  inPage = false;
  WiFiClient client;
  //
  // Try to connect to the host. If unsuccesful, quit the function with the error URLNOCONNECT
  //
  if (!client.connect(host,port)) {
    return URLNOCONNECT;
  }
  //
  // Send the HTTP GET request
  //
  client.print("GET " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  //
  // Wait for an answer but if none is given within 5 seconds
  // quite the function with the error URLTIMEOUT
  //
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return URLTIMEOUT;
    }
  }
  //
  // Go through all the lines. When we encounter an empty line
  // we know that the header is done and the remaining lines
  // are the body of the page. The boolean inPage, which initially
  // is set to false, will turn true. The function g is called if
  // if want the full content to be send to the function,
  // boolean fullContent, or if we are in the main page.
  //
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

String getURLParse (char* host, String url, bool fullContent, void (*g)(String l)) {
  return getURLParse(host,url,fullContent,g,80);
}

String getURLFullParse (char* host, String url, void (*g)(String l), int port) {
  return getURLParse(host,url,true,g,port);
}

String getURLFullParse (char* host, String url, void (*g)(String l)) {
  return getURLParse(host,url,true,g,80);
}

String getURLBodyParse (char* host, String url, void (*g)(String l), int port) {
  return getURLParse(host,url,false,g,port);
}

String getURLBodyParse (char* host, String url, void (*g)(String l)) {
  return getURLParse(host,url,false,g,80);
}

String getURL (char* host, String url, int port) {
  return getURLParse(host,url,false,doNothing,port);
}

String getURL (char* host, String url) {
  return getURLParse(host,url,false,doNothing,80);
}
