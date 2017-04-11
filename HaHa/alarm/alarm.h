/*
 * alarm.h
 *
 * Created: 4/10/2017 7:59:34 PM
 *  Author: kevin
 */ 



#ifndef ALARM_H_
#define ALARM_H_

#include "utils/hahaUtils.h"
#include "lighting/lighting.h"
#include "sound/sound.h"

bool setAlarm(bool active, bool allowSound, bool allowLight);
bool getAlarmStatus();

#endif /* ALARM_H_ */