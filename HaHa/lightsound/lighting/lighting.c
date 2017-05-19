/*
 * @file lighting.c
 * @brief Lights in the alert system implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "lighting.h"

int lightingPriority = PRI_OFF; /* Higher number indicates higher priority. */

int getLightingStatus() {
	return lightingPriority;
}

bool setLighting(int priority, bool active, int lightingPattern) {
	if (priority >= lightingPriority) {
		if (active) {
			//Activate lighting TODO
			lightingPriority = priority;
			gpio_set_pin_level(TOGGLE_LIGHT, true);
			return true;
		} else {
			//Deactivate lighting TODO
			lightingPriority = PRI_OFF;
			gpio_set_pin_level(TOGGLE_LIGHT, false);
			return true; //TODO implement
		}
	}
	return false;
}