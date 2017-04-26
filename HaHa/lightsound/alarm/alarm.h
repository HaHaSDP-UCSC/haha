/*
 * @file alarm.h
 * @brief Alarm system that controls the lights and sounds.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef ALARM_H_
#define ALARM_H_

#include "utils/hahaUtils.h"
#include "lightsound/lighting/lighting.h"
#include "lightsound/sound/sound.h"

bool setAlarm(bool active, bool allowSound, bool allowLight);
bool getAlarmStatus();

#endif /* ALARM_H_ */