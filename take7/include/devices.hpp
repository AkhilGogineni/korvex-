#pragma once

#include "main.h"
#include "lemlib/api.hpp"

extern pros::Controller controller;

extern pros::Motor intake;
extern pros::Motor slapper1;
extern pros::Motor slapper2;

extern pros::MotorGroup slapper;

extern pros::Motor rightFront;
extern pros::Motor rightBack;
extern pros::Motor rightTop;

extern pros::Motor leftFront;
extern pros::Motor leftBack;
extern pros::Motor leftTop;

extern pros::MotorGroup left_side_motors;
extern pros::MotorGroup right_side_motors;

extern pros::Imu inertial_sensor;

extern pros::ADIDigitalOut wing1;
extern pros::ADIDigitalOut wing2;

extern pros::ADIDigitalOut stick;

extern pros::ADIDigitalOut lift1;
extern pros::ADIDigitalOut lift2;

extern pros::ADIDigitalOut hang1;


extern lemlib::Chassis chassis;

extern Drive EzTempChassis;

extern lemlib::Chassis chassis;

extern Drive EzTempChassis;

void checkMotorsAndReturnTemperature();

