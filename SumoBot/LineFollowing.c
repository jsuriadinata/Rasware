#include "RASLib/inc/common.h"
#include "RASLib/inc/linesensor.h"
#include <RASLib/inc/motor.h>

static tMotor *mLeft;
static tMotor *mRight;

static tADC *adc[3];
static tBoolean initialized = false;

int main() {
  mLeft = InitializeServoMotor(PIN_B6, false);
  mRight = InitializeServoMotor(PIN_B7, true);
  initGPIOLineSensor();
  boolean lsLeft = checkADC(0);
  boolean lsMiddle = checkADC(1);
  boolean lsRight = checkADC(2);
  moveMotors(lsLeft, lsMiddle, lsRight)

}

void moveMotors(boolean l, boolean m, boolean r){
  if (l && m && !r){
    SetMotor(mLeft, 0.5);
    SetMotor(mRight, 0.75);
  } else if (l && !m && !r){
    SetMotor(left, 0.25);
    SetMotor(right, 0.75);
  } else if (!l && m && r){
    SetMotor(left, 0.75);
    SetMotor(right, 0.5);
  } else if (!l && !m && r){
    SetMotor(left, 0.75);
    SetMotor(right, 0.25);
  }
}

// checks the value of the ADC
// returns false if line sensor sees white
// returns true if line sensor sees black
boolean checkADC(int x){
  float f = ADCRead(adc[x]);
  if (f > 0.5){
    return false;
  } else {
    return true;
  }
}



// initializes all of the pins for the line sensor
void initGPIOLineSensor(void){
    if(initialized){
        return;
    }
    initialized = true;

    // set pins to ADC
    adc[0] = InitializeADC(PIN_C4);
    adc[1] = InitializeADC(PIN_C5);
    adc[2] = InitializeADC(PIN_C6);
}

void gpioLineSensorDemo(void){
    Printf("Press any key to quit\n");

    //loop until key is pressed
    while(!KeyWasPressed()){
        Printf(
            "Line Sensor values:  %1.3f,  %1.3f,  %1.3f\r",
            ADCRead(adc[0]),
            ADCRead(adc[1]),
            ADCRead(adc[2])
            );
    }
    Printf("\n");
}
