/*
 * @file lighting.c
 * @brief Lights in the alert system implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "lighting.h"

bool lightingStatus = false;

bool getLightingStatus() {
	return lightingStatus;
}

bool setLighting(bool active, int lightingPattern) {
	return false; //TODO program this.
}