#include "devices.hpp"
#include "autons.hpp"
#include "drivercontrol.hpp"

void doNothing() {}

ASSET(leftsidequalsv3_txt)
ASSET(elimsLeftPath1_txt)
ASSET(elimsLeftPath2_txt)
ASSET(elimsLeftPath3_txt)
ASSET(elimsLeftPath4_txt)
ASSET(elimsLeftPath5_txt)
ASSET(elimsLeftPath6_txt)
ASSET(skillsPath1_txt)
ASSET(skillsPath2_txt)
ASSET(skillsPath3_txt)

// function definitions

//       chassis.setPose(x, y, theta); 
//       chassis.turnTo(x, y, timeout);    
//       chassis.follow(path file, timeout, lookahead distance, isAsynchronous, forwards (false makes the robot run the path backwards));
//
//       chassis.waitUntil(inches); use this to have your bot wait until the path is finished
//       a really big number means just wait until the path has finished


const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;

// inits
void default_constants() {
  EzTempChassis.set_slew_min_power(80, 80);
  EzTempChassis.set_slew_distance(7, 7);
  EzTempChassis.set_pid_constants(&EzTempChassis.headingPID, 11, 9, 20, 0);
  EzTempChassis.set_pid_constants(&EzTempChassis.forward_drivePID, 0.8, 0.7, 5, 0);
  EzTempChassis.set_pid_constants(&EzTempChassis.backward_drivePID, 0.8, 0.7, 5, 0);
  EzTempChassis.set_pid_constants(&EzTempChassis.turnPID, 5, 0.003, 35, 15);
  EzTempChassis.set_pid_constants(&EzTempChassis.swingPID, 7, 0, 45, 0);
}


// void modified_exit_condition() {
//   EzTempChassis.set_exit_condition(EzTempChassis.turn_exit, 10, 3, 10, 7, 10, 10);
//   EzTempChassis.set_exit_condition(EzTempChassis.swing_exit, 10, 3, 10, 7, 10, 10);
//   EzTempChassis.set_exit_condition(EzTempChassis.drive_exit, 10, 50, 10, 150, 10, 10);
// }

void modified_exit_condition() {
  EzTempChassis.set_exit_condition(EzTempChassis.turn_exit, 100, 3, 500, 7, 500, 500);
  EzTempChassis.set_exit_condition(EzTempChassis.swing_exit, 100, 3, 500, 7, 500, 500);
  EzTempChassis.set_exit_condition(EzTempChassis.drive_exit, 80, 50, 300, 150, 500, 500);
}



// example auto for clearing matchload area and touching pole
void leftSideQual() {
    chassis.setPose(-48.428, -55.617, 135); // X: 0, Y: 0, Heading: 0

    chassis.follow(elimsLeftPath5_txt, 15, 1500, true, false);
    chassis.waitUntil(1000000);

    intake = 127;
    wing1.set_value(true);  
    wing2.set_value(true);
    chassis.follow(elimsLeftPath6_txt, 15, 2000, true);
    chassis.waitUntil(1000000);
    intake = 0;

    
    chassis.turnTo(-11.918, -55.484, 500);    
    chassis.follow(leftsidequalsv3_txt, 15, 2000, true);
    chassis.waitUntil(3);
    wing1.set_value(false);
    wing2.set_value(false);
    chassis.waitUntil(5000);
    chassis.turnTo(999999, -55.484, 500);    

}

// example auto for matchload elims
// for video check: https://www.youtube.com/live/bE180c_xft8?si=eTFYuLUq1aH28cv-&t=37856
void leftSideElims() {

    // chassis.setPose(-37.194, -53.076, 15); // X: 0, Y: 0, Heading: 0

    // chassis.follow(elimsLeftPath1_txt, 10, 2000, true);
    // intake = 127;
    // chassis.waitUntilDone();

    // EzTempChassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD);
    // EzTempChassis.set_drive_pid(24, DRIVE_SPEED, false);
    // EzTempChassis.wait_drive();

    // chassis.follow(elimsLeftPath2_txt, 15, 2000, false);
    // chassis.waitUntil(5);
    // intake = 0;
    // chassis.waitUntilDone();

    // chassis.turnTo(9999999, -15, 500, true);  
    // chassis.waitUntil(1000000);
    // pros::delay(250); 
    // pros::delay(750); 

    // chassis.follow(elimsLeftPath3_txt, 15, 1000, true);
    // chassis.waitUntil(1000000);

    // intake = 0;

    // chassis.follow(elimsLeftPath4_txt, 15, 2000, true, false);
    // chassis.waitUntil(1000000);

    // chassis.turnTo(9999999, -60, 1000, true);  
    // chassis.waitUntil(1000000);

    // chassis.follow(elimsLeftPath5_txt, 15, 1500, true, false);
    // chassis.waitUntil(1000000);

    // chassis.setPose(-59.662, -30.609, 180);

    // wing1.set_value(true);
    // wing2.set_value(true);
    // chassis.follow(elimsLeftPath6_txt, 15, 2000, true);
    // chassis.waitUntil(1000000);

    // wing1.set_value(false);
    // wing2.set_value(false);

    // chassis.turnTo(9999999, -60, 500, true);  

    // intake = -127;
}


void Descore() {
    slapper1.move_voltage(-12000);
    slapper2.move_voltage(-12000);
    pros::delay(600);
    slapper1.move_voltage(0);
    slapper2.move_voltage(0);
    pros::delay(100);
    spinIntake();
    
    EzTempChassis.set_drive_pid(30, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_turn_pid(45, TURN_SPEED);
    EzTempChassis.wait_drive();

    outtake();

    EzTempChassis.set_drive_pid(16, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_drive_pid(-12, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(180, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(true);

    EzTempChassis.set_drive_pid(36, DRIVE_SPEED);
    EzTempChassis.wait_until(32);
    wing2.set_value(false);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-8, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    pros::delay(200);

    EzTempChassis.set_drive_pid(8, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(135, TURN_SPEED);
    EzTempChassis.wait_drive();

  EzTempChassis.set_drive_pid(23, DRIVE_SPEED);
    EzTempChassis.wait_drive();

}

void ThreeBall() {
    slapper1.move_voltage(-12000);
    slapper2.move_voltage(-12000);
    pros::delay(600);
    slapper1.move_voltage(0);
    slapper2.move_voltage(0);
    pros::delay(100);
    spinIntake();
    EzTempChassis.set_drive_pid(4, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-29, 127);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_drive_pid(2, 127);
    EzTempChassis.wait_drive();


    EzTempChassis.set_turn_pid(135, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(true);
    pros::delay(300);

    EzTempChassis.set_drive_pid(20, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(115, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(false);

    EzTempChassis.set_drive_pid(-3, 127);
    EzTempChassis.wait_drive();
    
    EzTempChassis.set_turn_pid(125, TURN_SPEED);
    EzTempChassis.wait_drive();
    pros::delay(300);

    outtake();

    pros::delay(300);

    EzTempChassis.set_drive_pid(25, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-15, 127);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_turn_pid(-45, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(22, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(0, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(32, 127);
    EzTempChassis.wait_drive();
}
void FourBall() {

}
void SixBallSafe() {
    slapper1.move_voltage(-12000);
    slapper2.move_voltage(-12000);
    pros::delay(600);
    slapper1.move_voltage(0);
    slapper2.move_voltage(0);
    pros::delay(100);
    spinIntake();
    EzTempChassis.set_drive_pid(4, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-29, 127);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_drive_pid(2, 127);
    EzTempChassis.wait_drive();


    EzTempChassis.set_turn_pid(135, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(true);
    pros::delay(300);

    EzTempChassis.set_drive_pid(20, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(115, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(false);

    EzTempChassis.set_drive_pid(-3, 127);
    EzTempChassis.wait_drive();
    
    EzTempChassis.set_turn_pid(125, TURN_SPEED);
    EzTempChassis.wait_drive();
    pros::delay(300);

    outtake();

    pros::delay(300);

    EzTempChassis.set_drive_pid(25, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-5, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_swing_pid(ez::RIGHT_SWING, 100, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(10, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-15, 127);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_turn_pid(45, TURN_SPEED);
    EzTempChassis.wait_drive();

    spinIntake();
    EzTempChassis.set_drive_pid(60, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(180, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing1.set_value(true);
    wing2.set_value(true);

    outtake();
    EzTempChassis.set_drive_pid(35, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-20, 127);
    EzTempChassis.wait_drive();


}
void SixBallRisky() {
    EzTempChassis.set_angle(-40);
    EzTempChassis.set_drive_pid(60, 127);
    slapper1.move_voltage(-12000);
    slapper2.move_voltage(-12000);
    pros::delay(600);
    wing1.set_value(false);
    slapper1.move_voltage(0);
    slapper2.move_voltage(0);
    pros::delay(100);
    spinIntake();
    EzTempChassis.wait_drive();
    
    EzTempChassis.set_turn_pid(90, TURN_SPEED);
    EzTempChassis.wait_drive();
    stopIntake();
    wing1.set_value(true);
    //wing2.set_value(true);

    outtake();
    EzTempChassis.set_drive_pid(30, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-15, 127);
    EzTempChassis.wait_drive();

    wing1.set_value(false);
    wing2.set_value(false);

    EzTempChassis.set_turn_pid(-135, TURN_SPEED);
    EzTempChassis.wait_drive();

    spinIntake();
    EzTempChassis.set_drive_pid(20, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-10, 127);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_turn_pid(125, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(24, 127);
    outtake();
    EzTempChassis.wait_drive();


    EzTempChassis.set_turn_pid(180, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(30, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(270, TURN_SPEED);
    EzTempChassis.wait_drive();

    spinIntake();

    EzTempChassis.set_drive_pid(30, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-29, 127);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_drive_pid(2, 127);
    EzTempChassis.wait_drive();


    EzTempChassis.set_turn_pid(45, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(true);
    pros::delay(300);

    EzTempChassis.set_drive_pid(20, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(25, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing2.set_value(false);

    EzTempChassis.set_drive_pid(-3, 127);
    EzTempChassis.wait_drive();
    
    EzTempChassis.set_turn_pid(35, TURN_SPEED);
    EzTempChassis.wait_drive();
    pros::delay(300);

    outtake();

    pros::delay(300);

    EzTempChassis.set_drive_pid(25, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-5, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_swing_pid(ez::RIGHT_SWING, 10, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(10, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-15, 127);
    EzTempChassis.wait_drive();

    stopIntake();

}

void Elims() {
    EzTempChassis.set_angle(15);
    EzTempChassis.set_drive_pid(51, 127);
    slapper1.move_voltage(-12000);
    slapper2.move_voltage(-12000);
    pros::delay(600);
    wing1.set_value(false);
    slapper1.move_voltage(0);
    slapper2.move_voltage(0);
    pros::delay(100);
    spinIntake();
    EzTempChassis.wait_drive();
    stopIntake();

    EzTempChassis.set_turn_pid(90, TURN_SPEED);
    EzTempChassis.wait_drive();

    outtake();
    wing1.set_value(true);

    pros::delay(400);

    EzTempChassis.set_drive_pid(30, 127);
    EzTempChassis.wait_drive();

    wing1.set_value(false);
    stopIntake();
    EzTempChassis.set_drive_pid(-33, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(0, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-45, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-90, TURN_SPEED);
    EzTempChassis.wait_drive();

    spinIntake();

    EzTempChassis.set_drive_pid(10, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-45, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(20, 127);
    EzTempChassis.wait_drive();


    EzTempChassis.set_turn_pid(0, TURN_SPEED);
    EzTempChassis.wait_drive();

    outtake();

    pros::delay(400);

    EzTempChassis.set_drive_pid(15, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-10, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(15, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-10, 127);
    EzTempChassis.wait_drive();
}

void test() {


    pros::delay(200);

    // EzTempChassis.set_drive_pid(-24, DRIVE_SPEED);
    // EzTempChassis.wait_drive();

}

void skills() {
    EzTempChassis.set_angle(45);
    EzTempChassis.set_drive_pid(-32, 127);
    // slapper1.move_voltage(-12000);
    // slapper2.move_voltage(-12000);
    // pros::delay(600);
    // wing1.set_value(false);
    // slapper1.move_voltage(0);
    // slapper2.move_voltage(0);
    pros::delay(100);
    //spinIntake();
    EzTempChassis.wait_drive();
    
    EzTempChassis.set_turn_pid(90, TURN_SPEED);
    EzTempChassis.wait_drive();

    outtake();

    EzTempChassis.set_drive_pid(-16, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    stopIntake();

    EzTempChassis.set_drive_pid(13, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-15, TURN_SPEED);
    EzTempChassis.wait_drive();

    stick.set_value(true);

    // slapper1.move_voltage(-12000);
    // slapper2.move_voltage(-12000);

    // pros::delay(5000);

    // slapper1.move_voltage(0);
    // slapper2.move_voltage(0);

    stick.set_value(false);

    EzTempChassis.set_turn_pid(45, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(28, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(0, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(80, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-45, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(26, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    outtake();

    EzTempChassis.set_swing_pid(ez::RIGHT_SWING, -90, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(15, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    pros::delay(200);

    EzTempChassis.set_drive_pid(-8, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-180, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(48, DRIVE_SPEED);
    EzTempChassis.wait_drive();


    EzTempChassis.set_turn_pid(-90, TURN_SPEED);
    EzTempChassis.wait_drive();

    // EzTempChassis.set_swing_pid(ez::RIGHT_SWING, -90, DRIVE_SPEED);
    // EzTempChassis.wait_drive();


    EzTempChassis.set_drive_pid(15, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-20, TURN_SPEED);
    EzTempChassis.wait_drive();

    wing1.set_value(true);
    wing2.set_value(true);


    EzTempChassis.set_drive_pid(33, DRIVE_SPEED);
    EzTempChassis.wait_drive();
    wing2.set_value(false);

    pros::delay(200);

    wing1.set_value(false);

    pros::delay(200);

    EzTempChassis.set_drive_pid(-33, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(-90, TURN_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(50, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(20, TURN_SPEED);
    EzTempChassis.wait_drive();


    wing1.set_value(true);
    wing2.set_value(true);


    EzTempChassis.set_drive_pid(33, DRIVE_SPEED);
    EzTempChassis.wait_drive();
    wing1.set_value(false);

    pros::delay(200);

    wing2.set_value(false);

    pros::delay(200);


    EzTempChassis.set_drive_pid(-10, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(90, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-48, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_turn_pid(225, TURN_SPEED);
    EzTempChassis.wait_drive(); 

    EzTempChassis.set_drive_pid(-30, 127);
    EzTempChassis.wait_drive();

    EzTempChassis.set_swing_pid(ez::RIGHT_SWING, 90, DRIVE_SPEED);
    EzTempChassis.wait_drive();

    EzTempChassis.set_drive_pid(-15, DRIVE_SPEED);
    EzTempChassis.wait_drive();
}

