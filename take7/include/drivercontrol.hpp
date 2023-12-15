#pragma once

#include "pros/rtos.hpp"
#include "lemlib/api.hpp"

// . . .
// 
//  DRIVER CONTROL
//
// . . .


void set_drive_to_coast();
void set_drive_to_hold();

void spinIntake();
void outtake();
void stopIntake();