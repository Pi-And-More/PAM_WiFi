#ifndef PAM_WiFi_H
#define PAM_WiFi_H

#include <Arduino.h>

String WiFiConnect (char*, char*);
String getURL (char*, char*, int);
String getURLFullParse (char*, char*, void (*)(String), int);
String getURLBodyParse (char*, char*, void (*)(String), int);
String getURLParse (char*, char*, bool, void (*)(String), int);
void doNothing (String);

#endif
