/*
 * @file lighting.h
 * @brief Lights in the alert system.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef LIGHTING_H_
#define LIGHTING_H_

#include "utils/hahaUtils.h"
#include "atmel_start_pins.h"

int getLightingStatus();
bool setLighting(int priority, bool active, int lightingPattern);

#endif /* LIGHTING_H_ */