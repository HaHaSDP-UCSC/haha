/*
 * @file sound.h
 * @brief Sound System in the alert system.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef SOUND_H_
#define SOUND_H_

#include "utils/hahaUtils.h"

int getSoundStatus();
bool setSound(int priority, bool active, int lightingPattern);


#endif /* SOUND_H_ */