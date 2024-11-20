#ifndef GLOBALINSTANCES_H
#define GLOBALINSTANCES_H

#include <Preferences.h>

#include "config.h"
#include "cliHandler.h"
#include "wifiManager.h"
#include "irrigationControl.h"
#include "mqttControl.h"
#include "timeControl.h"

extern cliHandler espcli;
extern wifiManager wifiMan;
extern irrigationControl irrigationCtrl;
extern mqttControl mqttCtrl;
extern timeControl timeCtrl;

#endif