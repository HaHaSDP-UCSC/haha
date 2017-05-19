/*
 * @file sound.c
 * @brief Sound System in the alert system implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "sound.h"

int soundPriority = PRI_OFF; /* Higher number indicates higher priority. */

int getSoundStatus() {
	return soundPriority;
}

bool setSound(int priority, bool active, int soundPattern) {
	if (priority >= soundPriority) {
		if (active) {
			//Activate sound TODO
			return true;
			} else {
			//Deactivate sound TODO
			return true; //TODO implement
		}
	}
	return false;
}