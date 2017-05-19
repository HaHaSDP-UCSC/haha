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
bool allowSound = false;
bool allowLight = false;

/**
 * @brief      Sets the alarm.
 *
 * @param[in]  active      The active
 * @param[in]  allowSound  The allow sound
 * @param[in]  allowLight  The allow light
 *
 * @return     Success or failure.
 */
void setAlarm(bool active) {
    if(active) {
	    printd("ALARM ON!!!!!!!!!\n");
	    setLighting(PRI_ALERT, allowLight, 0x1);
	    setSound(PRI_ALERT, allowSound, 0x1);
	} else {
		printd("Alarm Off");
		setLighting(PRI_ALERT, false, 0x1);
		setSound(PRI_ALERT, false, 0x1);
	}
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