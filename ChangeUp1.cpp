/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Wyatt Lipscomb                                            */
/*    Created:      Mon Feb 22 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Vision               vision        16              
// LeftMotor            motor         11              
// RightIntake          motor         13              
// SlapMotor            motor         14              
// LeftIntake           motor         17              
// BackUp               motor         18              
// FrontUp              motor         19              
// RightMotor           motor         20              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

bool SLAPL = false;
bool SLAPR = false;

void IntakeIn(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  LeftIntake.spin(forward);
  RightIntake.spin(forward);  
};

void IntakeIn() {
  if (MYTURN) return;
  IntakeIn(false);
}

void IntakeOut(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  LeftIntake.spin(reverse);
  RightIntake.spin(reverse);
}

void IntakeOut() {
  if (MYTURN) return;
  IntakeOut(false);
}

void IntakeStop(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (!Man && (Controller1.ButtonL1.pressing() || Controller1.ButtonL2.pressing())) return;
  LeftIntake.stop(vex::brakeType::brake);
  RightIntake.stop(vex::brakeType::brake);
}

void IntakeStop() {
  if (MYTURN) return;
  IntakeStop(false);
}

int GearsStatus() {
  if (FrontUp.isSpinning() && BackUp.isSpinning()) {
    if (FrontUp.direction() == forward && BackUp.direction() == forward) {
      return 1; // going up
    } else if (FrontUp.direction() == reverse && BackUp.direction() == reverse) {
      return 2; // going down
    } else {
      return 3; // something went horribly wrong
    }
  } else if (FrontUp.isSpinning() || BackUp.isSpinning()) {
    return 3; // jammed?
  } else {
    return 0; // off
  }
}

void GearsUp(bool Man, double time=0) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (time > 0) {
    FrontUp.spinFor(forward, time, seconds);
    BackUp.spinFor(forward, time, seconds);
    return;
  }
  FrontUp.spin(forward);
  BackUp.spin(forward);  
};

void GearsUp() {
  if (MYTURN) return;
  GearsUp(false);
}

void GearsDown(bool Man, int time=0) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (time > 0) {
    FrontUp.spinFor(forward, time, seconds);
    BackUp.spinFor(forward, time, seconds);
    return;
  }
  FrontUp.spin(reverse);
  BackUp.spin(reverse);
}

void GearsDown() {
  if (MYTURN) return;
  GearsDown(false);
}

void GearsStop(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (!Man && (Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing())) return;
  FrontUp.stop(vex::brakeType::brake);
  BackUp.stop(vex::brakeType::brake);
}

void GearsStop() {
  if (MYTURN) return;
  GearsStop(false);
}

void SlapL() {
  if (!SLAPR) {
    SLAPL = true;
    LeftSlapMotor.setVelocity(65,percent);
    LeftSlapMotor.setMaxTorque(50, percent);
    LeftSlapMotor.spinToPosition(SlapMotor.position(deg)+360, deg);
    while (!LeftSlapMotor.isDone()) {}
    LeftSlapMotor.setPosition(SlapMotor.position(deg)-360, deg);
    LeftSlapMotor.stop(coast);
    SLAPL = false;
  }
}

void SlapR() {
  if (!SLAPL) {
    SLAPR = true;
    RightSlapMotor.setVelocity(65, percent);
    RightSlapMotor.spinToPosition(SlapMotor.position(deg)+360, deg);
    while (!RightSlapMotor.isDone()) {};
    RightSlapMotor.setPosition(SlapMotor.position(deg)-360, deg);
    RightSlapMotor.stop(coast);
    SLAPR = false;
  }
}
    

void A2Changed() {
  if (MYTURN) return;
  if (competition().isDriverControl() && competition().isEnabled()) {
    float a2 = Controller1.Axis2.position(percent);
    float a3 = Controller1.Axis3.position(percent);
    if (a3 < 0 && a2 > 0) {
      RightMotor.setVelocity(a2/8, percent);
    } else {
      RightMotor.setVelocity(a2/2, percent);
    }
    RightMotor.spin(forward);
    if (a2 < 5 && a2 > -5) RightMotor.stop(brake);
  }
}

void A3Changed() {
  if (MYTURN) return;
  if (competition().isDriverControl() && competition().isEnabled()) {
    float a3 = Controller1.Axis3.position(percent);
    float a2 = Controller1.Axis2.position(percent);
    if (a3 > 0 && a2 < 0) {
      LeftMotor.setVelocity(a3/8, percent);
    } else {
      LeftMotor.setVelocity(a3/2, percent);
    }
    LeftMotor.spin(forward);
    if (a3 < 5 && a3 > -5) LeftMotor.stop(brake);
  }
}

void LeftSlapLeft(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  LeftSlapMotor.setVelocity(30, percent);
  LeftSlapMotor.spin(reverse);
}

void LeftSlapLeft() { LeftSlapLeft(false); }

void SlapRight(bool Man) {
  LeftSlapMotor.setVelocity(30, percent);
  LeftSlapMotor.spin(forward);
}

void LeftSlapRight() {
  LeftSlapRight(false);
}

void LeftSlapStop(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  LeftSlapMotor.stop(hold);
}

void LeftSlapStop() { LeftSlapStop(false); }

void SlowInUp(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  FrontUp.setVelocity(30, percent);
  BackUp.setVelocity(30, percent);
  FrontUp.spin(forward);
  BackUp.spin(forward);
}

void SlowInUp() { SlowInUp(false); }

void SlowInDown(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  FrontUp.setVelocity(30, percent);
  BackUp.setVelocity(30, percent);
  FrontUp.spin(reverse);
  BackUp.spin(reverse);
}

void SlowInDown() { SlowInDown(false); }

void SlowInStop(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  if (!Man && (Controller1.ButtonUp.pressing() || Controller1.ButtonDown.pressing())) return;
  FrontUp.setVelocity(50, percent);
  BackUp.setVelocity(50, percent);
  FrontUp.stop(brake);
  BackUp.stop(brake);
}

void SlowInStop() { SlowInStop(false); }

void MLeftSlap() {
  if ((!competition().isEnabled() || !competition().isDriverControl())) return;
  Controller1.rumble(".");
  SlapL();
}

void MRightSlap() {
  Controller1.rumble(".");
  SlapR();
}

void Forward(int speed=RightMotor.velocity(pct)) {
  RightMotor.setVelocity(speed, pct);
  LeftMotor.setVelocity(speed, pct);
  RightMotor.spin(forward);
  LeftMotor.spin(forward);
}

void Reverse(int speed=RightMotor.velocity(pct)) {
  RightMotor.setVelocity(speed, pct);
  LeftMotor.setVelocity(speed, pct);
  RightMotor.spin(reverse);
  LeftMotor.spin(reverse);
}

bool Revs(vex::motor Motor, double target) { return Motor.position(rotationUnits::rev) >= target*0.35; };

void TurnRight() {
  RightMotor.spinFor(reverse, 0.95, rev);
  LeftMotor.spinFor(forward, 0.95, rev);
  while (!RightMotor.isDone()) {}
}

void TurnLeft() {
  RightMotor.spinFor(forward, 0.95, rev);
  LeftMotor.spinFor(reverse, 0.95, rev);
  while (!RightMotor.isDone()) {}
}

void autonomous() {
  SlapRight();
}

void Reset() {
  int s = Brain.timer(msec)+2500;
  while (Brain.timer(msec) < s && Controller1.ButtonX.pressing());
  if (Brain.timer(msec) < s) return;
  SLAPL = false;
  SLAPR = false;
  Controller1.rumble("..");
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  RightSlapMotor.setPosition(0,degrees);
  LeftSlapMotor.setPosition(0,degrees);

  competition().autonomous(autonomous);
  
  Controller1.ButtonL1.pressed(IntakeIn);
  Controller1.ButtonL2.pressed(IntakeOut);
  Controller1.ButtonR1.pressed(GearsUp);
  Controller1.ButtonR2.pressed(GearsDown);
  Controller1.ButtonA.pressed(MRightSlap);
  Controller1.ButtonY.pressed(MLeftSlap);
  Controller1.ButtonX.pressed(Reset);
  Controller1.ButtonRight.pressed(LeftSlapRight);
  Controller1.ButtonLeft.pressed(LeftSlapLeft);
  Controller1.ButtonUp.pressed(SlowInUp);
  Controller1.ButtonDown.pressed(SlowInDown);

  Controller1.ButtonUp.released(SlowInStop);
  Controller1.ButtonDown.released(SlowInStop);
  Controller1.ButtonRight.released(SlapStop);
  Controller1.ButtonLeft.released(SlapStop);
  Controller1.ButtonL1.released(IntakeStop);
  Controller1.ButtonL2.released(IntakeStop);
  Controller1.ButtonR1.released(GearsStop);
  Controller1.ButtonR2.released(GearsStop);

  Controller1.Axis2.changed(A2Changed);
  Controller1.Axis3.changed(A3Changed);
  FrontUp.setVelocity(75, percent);
  BackUp.setVelocity(75, percent);
  while (true) {
    Brain.Screen.newLine();
    Brain.Screen.print(Brain.timer(msec));
    /*
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    uint32_t Bat;
    Bat = (Brain.Battery.capacity());
    Controller1.Screen.print("Battery ");
    Controller1.Screen.setCursor(1, 9);
    Controller1.Screen.print(Bat);
    Controller1.Screen.setCursor(3,1);
    Controller1.Screen.clearLine(3);
    if (USEVISION == false) {
      Controller1.Screen.print("VISION OFF");
    } else {
      Controller1.Screen.print("VISION ON");
    }
    */
  }
}
