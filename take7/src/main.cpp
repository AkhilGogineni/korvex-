#include "main.h"
#include "lemlib/api.hpp"
#include "drivercontrol.hpp"
#include "devices.hpp"
#include "autons.hpp"
#include "gif-pros/gifclass.hpp"
#include "screen.hpp"
#include "display/lvgl.h" 

enum class autonStates { // the possible auton selections
	off,
	Descore,
	ThreeBall,
	SixBallSafe,
	SixBallRisky,
    Elims,
	Skills,
	test
};

autonStates autonSelection = autonStates::off;

static lv_res_t DescoreBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Descore;
	std::cout << pros::millis() << "Descore" << std::endl;
	return LV_RES_OK;
}

static lv_res_t ThreeBallBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::ThreeBall;
	std::cout << pros::millis() << "ThreeBall" << std::endl;
	return LV_RES_OK;
}

static lv_res_t SixBallRiskyBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::SixBallRisky;
	std::cout << pros::millis() << "SixBallRisky" << std::endl;
	return LV_RES_OK;
}

static lv_res_t SixBallSafeBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::SixBallSafe;
	std::cout << pros::millis() << "SixBallSafe" << std::endl;
	return LV_RES_OK;
}

static lv_res_t ElimsBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Elims;
	std::cout << pros::millis() << "Elims" << std::endl;
	return LV_RES_OK;
}

static lv_res_t SkillsBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Skills;
	std::cout << pros::millis() << "Skills" << std::endl;
	return LV_RES_OK;
}

static lv_res_t ResetBtnAction(lv_obj_t *btn) {
	inertial_sensor.reset();


	
	if (pros::millis() < 5000) std::cout << pros::millis() << ": finished calibrating!" << std::endl;
	return LV_RES_OK;
}

static lv_res_t noAutonBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::off;
	std::cout << pros::millis() << "None" << std::endl;
	return LV_RES_OK;
}


void initialize() {

    pros::delay(500); // Stop the user from doing anything while legacy ports configure.
 

    // calibrate the lem lib x ez temp chassis
    //calibrateBothChassis();


    // pid and curve inits

	
	
	EzTempChassis.set_active_brake(0.1); // Sets the active brake kP. We recommend 0.1.

	default_constants(); // Set the drive to your own constants from autons.cpp!
  	modified_exit_condition(); // Set the exit conditions to your own constants from autons.cpp!
	EzTempChassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
    //EzTempChassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  

	EzTempChassis.initialize();
	
    //chassis.calibrate();



     lv_theme_t *th = lv_theme_alien_init(360, NULL); //Set a HUE value and keep font default RED
	lv_theme_set_current(th);

	// create a tab view object
	std::cout << pros::millis() << ": creating gui..." << std::endl;
	lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), NULL);

	//button layout
	lv_obj_t *MatchTab = lv_tabview_add_tab(tabview, "Match");
	lv_obj_t *SkillsTab = lv_tabview_add_tab(tabview, "Skills");
	lv_obj_t *OffTab = lv_tabview_add_tab(tabview, "Turn Off");

	

	// Match tab
	lv_obj_t *DescoreBtn = lv_btn_create(MatchTab, NULL);
	lv_obj_t *labelDescore = lv_label_create(DescoreBtn, NULL);

	lv_obj_t *ThreeBallBtn = lv_btn_create(MatchTab, NULL);
	lv_obj_t *labelThreeBall = lv_label_create(ThreeBallBtn, NULL);

	lv_obj_t *SixBallSafeBtn = lv_btn_create(MatchTab, NULL);
	lv_obj_t *labelSixBallSafe = lv_label_create(SixBallSafeBtn, NULL);

	lv_obj_t *SixBallRiskyBtn = lv_btn_create(MatchTab, NULL);
	lv_obj_t *labelSixBallRisky = lv_label_create(SixBallRiskyBtn, NULL);

	lv_obj_t *ElimsBtn = lv_btn_create(MatchTab, NULL);
	lv_obj_t *labelElims = lv_label_create(ElimsBtn, NULL);

	lv_label_set_text(labelDescore, "Descore");
	lv_btn_set_action(DescoreBtn, LV_BTN_ACTION_CLICK, DescoreBtnAction);
	lv_obj_set_size(DescoreBtn, 90, 50);
	lv_btnm_set_toggle(DescoreBtn, true, 1);
	lv_obj_set_pos(DescoreBtn, 0, 0);
	lv_obj_align(DescoreBtn, NULL, LV_ALIGN_CENTER, -180, -20);

	lv_label_set_text(labelThreeBall, "3 Ball");
	lv_btn_set_action(ThreeBallBtn, LV_BTN_ACTION_CLICK, ThreeBallBtnAction);
	lv_obj_set_size(ThreeBallBtn, 90, 50);
	lv_btnm_set_toggle(ThreeBallBtn, true, 1);
	lv_obj_set_pos(ThreeBallBtn, 0, 0);
	lv_obj_align(ThreeBallBtn, NULL, LV_ALIGN_CENTER, -90, -15);

	lv_label_set_text(labelSixBallSafe, "6 Ball - S");
	lv_btn_set_action(SixBallSafeBtn, LV_BTN_ACTION_CLICK, SixBallSafeBtnAction);
	lv_obj_set_size(SixBallSafeBtn, 90, 50);
	lv_btnm_set_toggle(SixBallSafeBtn, true, 1);
	lv_obj_set_pos(SixBallSafeBtn, 0, 0);
	lv_obj_align(SixBallSafeBtn, NULL, LV_ALIGN_CENTER, 0, -10);

	lv_label_set_text(labelSixBallRisky, "6 Ball - R");
	lv_btn_set_action(SixBallRiskyBtn, LV_BTN_ACTION_CLICK, SixBallRiskyBtnAction);
	lv_obj_set_size(SixBallRiskyBtn, 90, 50);
	lv_btnm_set_toggle(SixBallRiskyBtn, true, 1);
	lv_obj_set_pos(SixBallRiskyBtn, 0, 0);
	lv_obj_align(SixBallRiskyBtn, NULL, LV_ALIGN_CENTER, 90, -5);

	lv_label_set_text(labelElims, "Elims");
	lv_btn_set_action(ElimsBtn, LV_BTN_ACTION_CLICK, ElimsBtnAction);
	lv_obj_set_size(ElimsBtn, 90, 50);
	lv_btnm_set_toggle(ElimsBtn, true, 1);
	lv_obj_set_pos(ElimsBtn, 0, 0);
	lv_obj_align(ElimsBtn, NULL, LV_ALIGN_CENTER, 180, 12.5);

	//off Tab	
	lv_obj_t *OffBtn = lv_btn_create(OffTab, NULL);
	lv_obj_t *Offlabel = lv_label_create(OffBtn, NULL);
	lv_label_set_text(Offlabel, "Turn Off");
	lv_btn_set_action(OffBtn, LV_BTN_ACTION_CLICK, noAutonBtnAction);
	lv_obj_set_size(OffBtn, 450, 50);
	lv_btnm_set_toggle(OffBtn, true, 1);
	lv_obj_set_pos(OffBtn, 0, 100);
	lv_obj_align(OffBtn, NULL, LV_ALIGN_CENTER, 0, 0);


	// skills tab
	lv_obj_t *SkillsBtn = lv_btn_create(SkillsTab, NULL);
	lv_obj_t *skillslabel = lv_label_create(SkillsBtn, NULL);

	lv_label_set_text(skillslabel, "Skills");
	lv_btn_set_action(SkillsBtn, LV_BTN_ACTION_CLICK, SkillsBtnAction);
	lv_obj_set_size(SkillsBtn, 450, 50);
	lv_btnm_set_toggle(SkillsBtn, true, 1);
	lv_obj_set_pos(SkillsBtn, 0, 100);
	lv_obj_align(SkillsBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	//reset btn
	lv_obj_t *ResetBtn = lv_btn_create(tabview, NULL);
	lv_obj_t *ResetLabel = lv_label_create(ResetBtn, NULL);

	lv_label_set_text(ResetLabel, "Reset");
	lv_btn_set_action(ResetBtn, LV_BTN_ACTION_CLICK, ResetBtnAction);
	lv_obj_set_size(ResetBtn, 250, 50);
	lv_btnm_set_toggle(ResetBtn, true, 1);
	lv_obj_set_pos(ResetBtn, 0, 100);
	lv_obj_align(ResetBtn, NULL, LV_ALIGN_CENTER, 0, 50);

	
	lift1.set_value(false);
	lift2.set_value(false);
	wing1.set_value(false);
	wing1.set_value(false);
	stick.set_value(false);
	hang1.set_value(false);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {} 

/**
 * Runs after initialize(), and before autonomous when connected to the Field

 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 */
void autonomous() {
    EzTempChassis.reset_pid_targets(); // Resets PID targets to 0
    EzTempChassis.reset_gyro(); // Reset gyro position to 0
    EzTempChassis.reset_drive_sensor(); // Reset drive sensors to 0
	EzTempChassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD);

    if(autonSelection == autonStates::off) {
		autonSelection = autonStates::Skills	;
	}	

	switch(autonSelection) {
		case autonStates::Descore:
			Descore();
			break;
		case autonStates::ThreeBall:
			ThreeBall();
			break;
		case autonStates::SixBallRisky:
			SixBallRisky();
			break;
		case autonStates::SixBallSafe:
			SixBallSafe();
			break;
		case autonStates::Elims:
			Elims();
			break;
		case autonStates::Skills:
			skills();
			break;
		case autonStates::test:
			test();
			break;
		default:
			break;
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 */

bool liftState = false;
bool leftWingState = false;
bool rightWingState = false;
bool stickState = false;
bool hangState = false;

void opcontrol() {
    EzTempChassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);

    // reset all pistons to false

    // task to make sure all motors are plugged in and check the temperature of the drivetrain

    bool flywheelOn = false;
	while (true) {
        while (true) {
            chassis.tank(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y), 3);
			//EzTempChassis.tank();

			if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
				intake.move_voltage(12000);
			} else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
				intake.move_voltage(-12000);
			} else {
				intake.move_voltage(0);
			}

			if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
				liftState = !liftState;
			}

			if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
				slapper1.move_voltage(-12000);
                slapper2.move_voltage(-12000);
			} else {
				slapper1.move_voltage(0);	
                slapper2.move_voltage(0);			
			}
				
			if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
				leftWingState = !leftWingState;
			}

			if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
				rightWingState = !rightWingState;
			}

			if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
				stickState = !stickState;
			}

			if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
				hangState = !hangState;
			}
			
			if(liftState) {
				lift1.set_value(true);
				lift2.set_value(true);
			} else if(!liftState) {
				lift1.set_value(false);
				lift2.set_value(false);
			}

			
			if(leftWingState) {
				wing1.set_value(true);
			} else if(!leftWingState) {
				wing1.set_value(false);
			}

			if(rightWingState) {
				wing2.set_value(true);
			} else if(!rightWingState) {
				wing2.set_value(false);
			}


			if(stickState) {
				stick.set_value(true);
			} else if(!stickState) {
				stick.set_value(false);
			}

            if(hangState) {
				hang1.set_value(true);
			} else if(!hangState) {
				hang1.set_value(false);
			}

        
		pros::delay(10);
	}
	}
}

