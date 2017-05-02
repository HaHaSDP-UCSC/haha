/*
 * @file alarm.c
 * @brief Alarm system that controls the lights and sounds implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */

#include "alarm.h"
#include "../lighting/lighting.h"
#include "../sound/sound.h"


bool alarmstatus = false;

/**
 * @brief      Sets the alarm.
 *
 * @param[in]  active      The active
 * @param[in]  allowSound  The allow sound
 * @param[in]  allowLight  The allow light
 *
 * @return     Success or failure.
 */
bool setAlarm(bool active, bool allowSound, bool allowLight) {
    if(active)
        printd("ALARM ON!!!!!!!!!\n");
    else
        printd("Alarm Off");
    setLighting(true, 0x1);
    setSound(true, 0x1);
}

/**
 * @brief      Gets the alarm status.
 *
 * @return     The alarm status.
 */
bool getAlarmStatus() {
    char* t = alarmstatus ? "ON" : "OFF";
    printd("Alarm is: %s", t);
	return alarmstatus;
}