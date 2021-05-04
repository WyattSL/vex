/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\390105249                                        */
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

bool SLAPMAN = false;
bool SLAPAUTO = false;
bool USEVISION = true;
bool MYTURN = false;

// The color of our team.
auto COLOR = "UNKNOWN";
auto SIDE = "UNKNOWN";

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

void Slap() {
  if (!SLAPMAN) {
    bool NTS = false; //need-to-stop
    SLAPAUTO = true;
    SlapMotor.setVelocity(75,percent);
    SlapMotor.setMaxTorque(50, percent);
    //SlapMotor.spinFor(0.95,turns);
    // all comments below this line are needed to move the intake upwards during a slap
    /*
    if (GearsStatus() != 1) {
      GearsUp(true);
      NTS = true;
    }
    */
    SlapMotor.spinToPosition(SlapMotor.position(deg)+360, deg);
    while (!SlapMotor.isDone()) {
      /*
      if (GearsStatus() != 1) {
        GearsUp(true);
        NTS = true;
      }
      */
    }
    SlapMotor.setPosition(SlapMotor.position(deg)-360, deg);
    SlapMotor.stop(coast);
    //if (GearsStatus() == 1 && NTS) GearsStop(true);
    SLAPAUTO = false;
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

void SlapLeft(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  SLAPMAN = true;
  SlapMotor.setVelocity(30, percent);
  SlapMotor.spin(reverse);
}

void SlapLeft() { SlapLeft(false); }

void SlapRight(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  SLAPMAN = true;
  SlapMotor.setVelocity(30, percent);
  SlapMotor.spin(forward);
}

void SlapRight() {
  SlapRight(false);
}

void SlapStop(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  SLAPMAN = false;
  SlapMotor.stop(hold);
}

void SlapStop() { SlapStop(false); }

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

void MSlap() {
  if ((!competition().isEnabled() || !competition().isDriverControl())) return;
  if (MYTURN) return;
  Slap();
  Controller1.rumble(".");
}

void ToggleVision(bool Man) {
  if (!Man && (!competition().isEnabled() || !competition().isDriverControl())) return;
  if (USEVISION) {
    USEVISION = false;
    Controller1.rumble("..");
  } else {
    USEVISION = true;
    Controller1.rumble("-");
  }
}

void ToggleVision() {
  ToggleVision(false);
}

bool Cycled = false;

void ScreenCycle() {
  if (Cycled) return;
  if (MYTURN) wait(0.5, seconds);
  if (MYTURN) return;
  Cycled = true;
  while (Cycled && !MYTURN) {
    Controller1.Screen.clearScreen();
    int off = -1;
    if (!competition().isEnabled()) {
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("DISABLED");
      off = 2;
    } else if (competition().isAutonomous()) {
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("AUTONOMOUS");
      off = 6;
    }
    Controller1.Screen.setCursor(1,23-off);
    Controller1.Screen.print(Brain.Battery.capacity(pct));
    if (USEVISION) {
      Controller1.Screen.setCursor(3, 23);
      Controller1.Screen.print("ON");
    } else {
      Controller1.Screen.setCursor(3, 22);
      Controller1.Screen.print("OFF");
    }
    wait(15, msec);
  }
}

void SelectorColorDraw(int Selection) {
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("Select team color");
  Controller1.Screen.setCursor(2, 1);
  if (Selection == 1) {
    Controller1.Screen.print("> Red");
  } else {
    Controller1.Screen.print("  Red");
  }
  Controller1.Screen.setCursor(3, 1);
  if (Selection == 2) {
    Controller1.Screen.print("> Blue");
  } else {
    Controller1.Screen.print("  Blue");
  }
}

void SelectorSideDraw(int Selection) {
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("Select starting side");
  Controller1.Screen.column();
  Controller1.Screen.setCursor(2, 18);
  if (Selection == 1) {
    Controller1.Screen.print("> Right");
  } else {
    Controller1.Screen.print("  Right");
  }
  Controller1.Screen.setCursor(3, 1);
  if (Selection == 2) {
    Controller1.Screen.print("Left <");
  } else {
    Controller1.Screen.print("Left");
  }
}

void SelectorMenu() {
  Cycled = false;
  MYTURN = true;
  int Selection = 1;
  int lsel = Selection;
  SelectorColorDraw(Selection);
  while (true) {
    if (lsel != Selection) {
      SelectorColorDraw(Selection);
      lsel = Selection;
    }
    if (Controller1.ButtonDown.pressing()) {
      Selection = 2;
    } else if (Controller1.ButtonUp.pressing()) {
      Selection = 1;
    } else if (Controller1.ButtonA.pressing()) {
      if (Selection == 1) COLOR = "red";
      if (Selection == 2) COLOR = "blue";
      Controller1.rumble(".");
      break;
    }
  };
  Selection = 1;
  lsel = Selection;
  SelectorSideDraw(Selection);
  while (true) {
    if (lsel != Selection) {
      SelectorSideDraw(Selection);
      lsel = Selection;
    }
    if (Controller1.ButtonDown.pressing()) {
      Selection = 2;
    } else if (Controller1.ButtonUp.pressing()) {
      Selection = 1;
    } else if (Controller1.ButtonA.pressing()) {
      if (Selection == 1) SIDE = "right";
      if (Selection == 2) SIDE = "left";
      Controller1.rumble(".");
      break;
    }
  };
  wait(3, msec);
  MYTURN = false;
}

void SelectorReturn() {
  int VX = 3;
  if (competition().isCompetitionSwitch()) VX = 6;
  int TIME = Brain.Timer.time(seconds)+VX;
  while (Controller1.ButtonX.pressing() && Brain.Timer.time(seconds) < TIME) {}
  if (Controller1.ButtonX.pressing()) {
    MYTURN = true;
    Controller1.rumble("...");
    IntakeStop(true);
    GearsStop(true);
    RightMotor.stop();
    LeftMotor.stop();
    bool TUV = USEVISION;
    USEVISION = false;
    TIME = Brain.Timer.value()+1;
    while (Brain.Timer.value() < TIME) {};
    SelectorMenu();
    USEVISION = TUV;
  }
  TIME = Brain.Timer.value()+1;
  while (Brain.Timer.value() < TIME) {};
  MYTURN = false;
  ScreenCycle();
}

/*
std::map<int, std::string> WLstore;

void WriteLine(int Column, std::string Text) {
  WLstore[Column] = Text;
}

void PublishLine(int Line) {
  Controller1.Screen.clearLine(Line);
  for (int i=0;i<sizeof(WLstore);i++) {
    
  }
}

*/

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
  Forward();
  RightMotor.resetPosition();
  while (!Revs(RightMotor, 1));
  
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  wait(0.25, seconds);

  Controller1.ButtonA.pressed(ScreenCycle);

  SelectorMenu();

  SlapMotor.setPosition(0,degrees);
  SlapMotor.spinToPosition(0, degrees);

  competition().autonomous(autonomous);
  
  Controller1.ButtonL1.pressed(IntakeIn);
  Controller1.ButtonL2.pressed(IntakeOut);
  Controller1.ButtonR1.pressed(GearsUp);
  Controller1.ButtonR2.pressed(GearsDown);
  Controller1.ButtonA.pressed(MSlap);
  Controller1.ButtonX.pressed(SelectorReturn);
  Controller1.ButtonRight.pressed(SlapRight);
  Controller1.ButtonLeft.pressed(SlapLeft);
  Controller1.ButtonUp.pressed(SlowInUp);
  Controller1.ButtonDown.pressed(SlowInDown);
  Controller1.ButtonB.pressed(ToggleVision);

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
  auto red = "red";
  auto blue = "blue";
  while (true) {
    Brain.Screen.newLine();
    Brain.Screen.print(Brain.timer(msec));
    if (MYTURN) continue;
    if (COLOR == red) Vision.takeSnapshot(Vision__SIG_BLUE);
    if (COLOR == blue) Vision.takeSnapshot(Vision__SIG_RED);
    if (Vision.objectCount > 0 && Vision.largestObject.exists && Vision.largestObject.height > 20 && Vision.largestObject.width > 20) {
      if (USEVISION && !SLAPMAN && !SLAPAUTO) {
        Slap();
      }
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
}
