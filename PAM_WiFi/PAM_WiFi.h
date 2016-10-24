#ifndef PAM_WiFi_H
#define PAM_WiFi_H

//
// URLNOCONNECT is returned if the connection to the host could not be made
//
#define URLNOCONNECT "ERR:noconnect"
//
// URLTIMEOUT is returned if a connection was made but the retrieving the
// page timed out
//
#define URLTIMEOUT "ERR:timeout"

#include <Arduino.h>

void wifiConnect (String, String);
String getURL (char*, String);
String getURL (char*, String, int);
String getURLFullParse (char*, String, void (*)(String));
String getURLFullParse (char*, String, void (*)(String), int);
String getURLBodyParse (char*, String, void (*)(String));
String getURLBodyParse (char*, String, void (*)(String), int);
String getURLParse (char*, String, bool, void (*)(String));
String getURLParse (char*, String, bool, void (*)(String), int);
void doNothing (String);

#endif
