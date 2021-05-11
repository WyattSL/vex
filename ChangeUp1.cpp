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
// LeftMotor            motor         19              
// RightIntake          motor         8               
// LeftSlapMotor        motor         12              
// LeftIntake           motor         20              
// BackUp               motor         11              
// FrontUp              motor         1               
// RightMotor           motor         18              
// Controller1          controller                    
// RightSlapMotor       motor         13              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

bool SLAPL = false;
bool SLAPR = false;

int TurningSpeed = 8;
int MovingSpeed = 2;

void IntakeIn(bool Man) {
  LeftIntake.spin(forward);
  RightIntake.spin(forward);  
};

void IntakeIn() {
  IntakeIn(false);
}

void IntakeOut(bool Man) {
  LeftIntake.spin(reverse);
  RightIntake.spin(reverse);
}

void IntakeOut() {
  IntakeOut(false);
}

void IntakeStop(bool Man) {
  if (!Man && (Controller1.ButtonL1.pressing() || Controller1.ButtonL2.pressing())) return;
  LeftIntake.stop(vex::brakeType::brake);
  RightIntake.stop(vex::brakeType::brake);
}

void IntakeStop() {
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
  if (time > 0) {
    FrontUp.spinFor(forward, time, seconds);
    BackUp.spinFor(forward, time, seconds);
    return;
  }
  FrontUp.spin(forward);
  BackUp.spin(forward);  
};

void GearsUp() {
  GearsUp(false);
}

void GearsDown(bool Man, int time=0) {
  if (time > 0) {
    FrontUp.spinFor(forward, time, seconds);
    BackUp.spinFor(forward, time, seconds);
    return;
  }
  FrontUp.spin(reverse);
  BackUp.spin(reverse);
}

void GearsDown() {
  GearsDown(false);
}

void GearsStop(bool Man) {
  if (!Man && (Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing())) return;
  FrontUp.stop(vex::brakeType::brake);
  BackUp.stop(vex::brakeType::brake);
}

void GearsStop() {
  GearsStop(false);
}

void SlapL() {
  if (!SLAPR && !SLAPL) {
    SLAPL = true;
    LeftSlapMotor.setVelocity(65,percent);
    LeftSlapMotor.setMaxTorque(50, percent);
    LeftSlapMotor.spinToPosition(LeftSlapMotor.position(deg)+300, deg, true);
    LeftSlapMotor.setPosition(LeftSlapMotor.position(deg)-300, deg);
    LeftSlapMotor.stop(coast);
    SLAPL = false;
  }
  return;
}

void SlapR() {
  if (!SLAPL && !SLAPR) {
    SLAPR = true;
    RightSlapMotor.setVelocity(65, percent);
    RightSlapMotor.spinToPosition(RightSlapMotor.position(deg)+300, deg, true);
    RightSlapMotor.setPosition(RightSlapMotor.position(deg)-300, deg);
    RightSlapMotor.stop(coast);
    SLAPR = false;
  }
  return;
}
    

void A2Changed() {
  float a2 = Controller1.Axis2.position(percent);
  RightMotor.setVelocity(a2/MovingSpeed, percent);
  RightMotor.spin(forward);
  if (a2 < 5 && a2 > -5) RightMotor.stop(brake);
}

void A3Changed() {
  float a3 = Controller1.Axis3.position(percent);
  LeftMotor.setVelocity(a3/MovingSpeed, percent);
  LeftMotor.spin(forward);
  if (a3 < 5 && a3 > -5) LeftMotor.stop(brake);
}

void Boost() {
  TurningSpeed = 6;
  MovingSpeed = 1;
  A3Changed();
  A2Changed();
}

void Unboost() {
  TurningSpeed = 8;
  MovingSpeed = 2;
  A3Changed();
  A2Changed();
}

void LeftSlapLeft(bool Man) {
  LeftSlapMotor.setVelocity(30, percent);
  LeftSlapMotor.spin(reverse);
}

void LeftSlapLeft() { LeftSlapLeft(false); }

void LeftSlapRight(bool Man) {
  LeftSlapMotor.setVelocity(30, percent);
  LeftSlapMotor.spin(forward);
}

void LeftSlapRight() {
  LeftSlapRight(false);
}

void LeftSlapStop(bool Man) {
  LeftSlapMotor.stop(hold);
}

void LeftSlapStop() { LeftSlapStop(false); }

void RightSlapRight() {
  RightSlapMotor.setVelocity(30, percent);
  RightSlapMotor.spin(forward);
}

void RightSlapLeft() {
  RightSlapMotor.setVelocity(30, percent);
  RightSlapMotor.spin(reverse);
}

void RightSlapStop() {
  RightSlapMotor.stop(hold);
}

void SlowInUp(bool Man) {
  FrontUp.setVelocity(30, percent);
  BackUp.setVelocity(30, percent);
  FrontUp.spin(forward);
  BackUp.spin(forward);
}

void SlowInUp() { SlowInUp(false); }

void SlowInDown(bool Man) {
  FrontUp.setVelocity(30, percent);
  BackUp.setVelocity(30, percent);
  FrontUp.spin(reverse);
  BackUp.spin(reverse);
}

void SlowInDown() { SlowInDown(false); }

void SlowInStop(bool Man) {
  if (!Man && (Controller1.ButtonUp.pressing() || Controller1.ButtonDown.pressing())) return;
  FrontUp.setVelocity(50, percent);
  BackUp.setVelocity(50, percent);
  FrontUp.stop(brake);
  BackUp.stop(brake);
}

void SlowInStop() { SlowInStop(false); }

void MLeftSlap() {
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
  FrontUp.setVelocity(75, pct);
  BackUp.setVelocity(75, pct);

  /*
  RightMotor.spinFor(-0.25, rev, false);
  LeftMotor.spinFor(0.25, rev, true);
  */
  LeftMotor.spinFor(0.15, rev, true);

  SlapR();

  RightMotor.spinFor(0.4, rev, false);
  LeftMotor.spinFor(0.4, rev, true);

  RightMotor.spinFor(-0.85, rev, false);
  LeftMotor.spinFor(0.85, rev, true);

  RightIntake.setVelocity(75, pct);
  LeftIntake.setVelocity(75, pct);
  RightIntake.spin(forward);
  LeftIntake.spin(forward);

  FrontUp.spinFor(1.5, rev, false);
  BackUp.spinFor(1.5, rev, false);
  
  RightMotor.spinFor(1.95, rev, false);
  LeftMotor.spinFor(1.95, rev);

  RightIntake.stop(coast);
  LeftIntake.stop(coast);

  RightMotor.stop(brake);
  LeftMotor.stop(brake);

  RightMotor.spinFor(-3.05, rev, false);
  LeftMotor.spinFor(-3.05, rev);

  RightMotor.spinFor(1.25, rev, false);
  LeftMotor.spinFor(-1.25, rev);

  RightMotor.spinFor(-0.75, rev, false);
  LeftMotor.spinFor(-0.75, rev, true);
  
  FrontUp.spin(forward);
  BackUp.spin(forward);
  RightIntake.spin(forward);
  LeftIntake.spin(forward);
  wait(1, sec);
  SlapL();
  wait(0.75, sec);
  FrontUp.stop(coast);
  BackUp.stop(coast);
  RightIntake.stop(coast);
  LeftIntake.stop(coast);
}

void Reset() {
  int s = Brain.timer(msec)+1500;
  while (Brain.timer(msec) < s && Controller1.ButtonX.pressing());
  if (Brain.timer(msec) < s) return;
  SLAPL = false;
  SLAPR = false;
  LeftSlapMotor.stop(coast);
  RightSlapMotor.stop(coast);
  Controller1.rumble("..");
}

void drivercontrol() {
  Controller1.ButtonL1.pressed(IntakeIn);
  Controller1.ButtonL2.pressed(IntakeOut);
  Controller1.ButtonR1.pressed(GearsUp);
  Controller1.ButtonR2.pressed(GearsDown);
  Controller1.ButtonA.pressed(MRightSlap);
  Controller1.ButtonY.pressed(MLeftSlap);
  Controller1.ButtonB.pressed(Boost);
  Controller1.ButtonB.released(Unboost);
  Controller1.ButtonX.pressed(Reset);
  Controller1.ButtonRight.pressed(LeftSlapRight);
  Controller1.ButtonLeft.pressed(LeftSlapLeft);
  Controller1.ButtonUp.pressed(SlowInUp);
  Controller1.ButtonDown.pressed(SlowInDown);

  Controller1.ButtonUp.released(SlowInStop);
  Controller1.ButtonDown.released(SlowInStop);
  Controller1.ButtonRight.released(LeftSlapStop);
  Controller1.ButtonLeft.released(LeftSlapStop);
  Controller1.ButtonL1.released(IntakeStop);
  Controller1.ButtonL2.released(IntakeStop);
  Controller1.ButtonR1.released(GearsStop);
  Controller1.ButtonR2.released(GearsStop);

  Controller1.Axis2.changed(A2Changed);
  Controller1.Axis3.changed(A3Changed);
  FrontUp.setVelocity(75, percent);
  BackUp.setVelocity(75, percent);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  RightSlapMotor.setPosition(0,degrees);
  LeftSlapMotor.setPosition(0,degrees);

  competition().autonomous(autonomous);
  competition().drivercontrol(drivercontrol);
  
  while (true) {
    Brain.Screen.newLine();
    Brain.Screen.print(RightMotor.temperature());
    Brain.Screen.print(" : ");
    Brain.Screen.print(LeftMotor.temperature());
    Brain.Screen.print(" : ");
    Brain.Screen.print(Brain.Timer.time());
    if (RightMotor.temperature() > 60) {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Heat Warn : R-Motor");
    }
    if (RightMotor.temperature() > 70) {
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Heat Fail : R-Motor");
    }
    if (LeftMotor.temperature() > 60) {
      Controller1.Screen.clearLine(2);
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("Heart Warn : L-Motor");
    }
    if (LeftMotor.temperature() > 70) {
      Controller1.Screen.clearLine(2);
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("Heart Fail : L-Motor");
    }
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
  Controller1.rumble("--..--");
  Brain.Screen.newLine();
  Brain.Screen.print("MAIN LOOP ENDED");
  Controller1.Screen.print("MAIN LOOP ENDED");
}
