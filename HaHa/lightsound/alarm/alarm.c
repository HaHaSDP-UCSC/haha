/*
 * @file alarm.c
 * @brief Alarm system that controls the lights and sounds implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */

#include "alarm.h"

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
	return false; //TODO program this.	
}

/**
 * @brief      Gets the alarm status.
 *
 * @return     The alarm status.
 */
bool getAlarmStatus() {
	return alarmstatus;
}