#include "lemlib/api.hpp"
#include "main.h"
#include "devices.hpp"
#include "drivercontrol.hpp"

pros::ADIDigitalOut wing1('E');
pros::ADIDigitalOut wing2('D');
pros::ADIDigitalOut stick ('B');
pros::ADIDigitalOut lift1('F');
pros::ADIDigitalOut lift2('C');
pros::ADIDigitalOut hang1('G');

pros::Motor intake (-16, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor slapper1 (10, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor slapper2 (2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::MotorGroup slapper ({slapper1, slapper2});

pros::Motor leftFront(5, pros::E_MOTOR_GEARSET_06, true); 
pros::Motor leftBack(4, pros::E_MOTOR_GEARSET_06, true); 
pros::Motor leftTop(3, pros::E_MOTOR_GEARSET_06, false); 
pros::Motor rightFront(14, pros::E_MOTOR_GEARSET_06, false); 
pros::Motor rightBack(11, pros::E_MOTOR_GEARSET_06, false); 
pros::Motor rightTop(12, pros::E_MOTOR_GEARSET_06, true); 

// this is for the controller screen to alert you if a motor is unplugged
// and tell you the temperature of the drivetrain
std::vector<pros::Motor> motors = { 
        leftFront, leftBack, leftTop, rightFront, rightBack, rightTop,
        // add more motors here
    };

pros::MotorGroup left_side_motors({leftFront, leftBack, leftTop});
pros::MotorGroup right_side_motors({rightFront, rightBack, rightTop});

pros::Imu inertial_sensor(17); 

pros::Controller controller(pros::E_CONTROLLER_MASTER);

Drive EzTempChassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-4, -5, 3}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{11, 14, -12}

  // IMU Port
  ,17

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.75

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,48.0/36.0


  // Uncomment if using tracking wheels
  /*
  // Left Tracking Wheel Ports (negative port will reverse it!)
  // ,{1, 2} // 3 wire encoder
  // ,8 // Rotation sensor

  // Right Tracking Wheel Ports (negative port will reverse it!)
  // ,{-3, -4} // 3 wire encoder
  // ,-9 // Rotation sensor
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);


lemlib::Drivetrain drivetrain (
    &left_side_motors, // left drivetrain motors
    &right_side_motors, // right drivetrain motors
    11.5, // track width
    2.75, // wheel diameter
    450, // wheel rpm
    2 // chase power
);

lemlib::OdomSensors sensors (
    nullptr, 
    nullptr, 
    nullptr, 
    nullptr,
    &inertial_sensor // inertial sensor
);

// forward/backward PID
lemlib::ControllerSettings lateralController (
    0.45, // kP
    5, // kD
    1, // smallErrorRange
    80, // smallErrorTimeout
    3, // largeErrorRange
    300, // largeErrorTimeout
    7 // slew rate
);
 
// turning PID
lemlib::ControllerSettings angularController (
    5, // kP
    35, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    0 // slew rate
);

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

void checkMotorsAndReturnTemperature() {
    while (true) {
        double totalTemp = 0.0;
        int count = 0;

        for (auto& motor : motors) {
            double temp = motor.get_temperature();
            if (temp == PROS_ERR_F) { // PROS_ERR_F is returned when the motor is unplugged
                master.set_text(0, 0, "Motor " + std::to_string(motor.get_port()) + " unplugged.");
                pros::delay(250);
                master.rumble("---");
            }

            if (count < 6) {
                totalTemp += temp;
            }
            ++count;
        }

        if (count == 0) master.set_text(0, 0, "No motors found.");

        double averageTempCelsius = totalTemp / count;
        double averageTempFahrenheit = averageTempCelsius * 9.0 / 5.0 + 32.0;
        master.set_text(0, 0, "Avg Temp: " + std::to_string(averageTempFahrenheit));

        pros::delay(250);
    }
}




void calibrateBothChassis() {
    chassis.calibrate(false); // calibrate the lem lib chassis without imu

    // calibrate the chassis with imu
}

void ezTempChassisInits() {
    // EZ temp inits
    //EzTempChassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
    EzTempChassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
    EzTempChassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
    
}