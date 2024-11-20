#include "globalInstances.h"

cliHandler espcli(Serial, BAUDRATE);
wifiManager wifiMan(1);
irrigationControl irrigationCtrl(DEFAULT_WATERING_TIME);
mqttControl mqttCtrl(1);
timeControl timeCtrl("south-america.pool.ntp.org", -10800, 0);
