#include "config.h"
// if you are seeing this, sorry, all sensitive info is stored locally

// WiFi Config
#define WIFI_SSID HIDDEN_WIFI_SSID
#define WIFI_PASSWORD HIDDEN_WIFI_PWD
#define HOSTNAME "smart-blinds"

// Total revs to open/close blinds (experimentally determined)
#define TOTAL_ROTATIONS 1

// Speed to open/close blinds
// delay between steps in ms
#define STEPS_DELAY_FAST 600
#define STEPS_DELAY_SLOW 1000

// max time to open/close in seconds
#define MAX_SPIN_TIME 30