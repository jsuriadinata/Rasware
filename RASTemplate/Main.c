#include <RASLib/inc/adc.h>
#include <RASLib/inc/common.h>
#include <RASLib//inc/gpio.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>

static tMotor *mLeft;
static tMotor *mRight;

static tADC *adc[3];
static tBoolean initialized = false;

static tADC *snr;

// initializes all of the pins for the line sensor
void initLineSensor(){
    if(initialized){
        return;
    }
    initialized = true;

    // set pins to ADC
    adc[0] = InitializeADC(PIN_D0);
    adc[1] = InitializeADC(PIN_D1);
    adc[2] = InitializeADC(PIN_D2);
}

// checks the value of the ADC
// returns false if line sensor sees white
// returns true if line sensor sees black
tBoolean checkADC(tADC *a){
  return ADCRead(a) < 0.5;
}

tBoolean watch(){
  return ADCRead(snr) > 0.1;
}

int main() {
  mRight = InitializeServoMotor(PIN_B0, true);
  mLeft = InitializeServoMotor(PIN_B1, false);
  snr = InitializeADC(PIN_E4);
  // object sensing
  tBoolean notSee = ADCRead(snr) < 0.4;
  while (true){
    if(notSee){
      SetMotor(mLeft, -0.05);
      SetMotor(mRight, 0.05);
    } else {
      SetMotor(mLeft, 0.2);
      SetMotor(mRight, 0.2);
    }
    notSee = ADCRead(snr) < 0.4;
  }
  // line following
  initLineSensor();
  SetMotor(mLeft, 0.1);
  SetMotor(mRight, 0.1);
  Wait(1);
  tBoolean r = ADCRead(adc[0]) < 0.5;
  tBoolean m = ADCRead(adc[1]) < 0.5;
  tBoolean l = ADCRead(adc[2]) < 0.5;
  while(true){
    if (r && m && l){
      SetMotor(mLeft, -0.1);
      SetMotor(mRight, -0.1);
      WaitUS(20);
    } else if (r && m){
      SetMotor(mLeft, 0.1);
      SetMotor(mRight, 0.25);
    } else if (l && m){
      SetMotor(mLeft, 0.25);
      SetMotor(mRight, 0.1);
    } else if (r){
      SetMotor(mLeft, 0.1);
      SetMotor(mRight, 0.15);
    } else if (l){
      SetMotor(mLeft, 0.15);
      SetMotor(mRight, 0.1);
    }
      r = checkADC(adc[0]);
      m = checkADC(adc[1]);
      l = checkADC(adc[2]);
  }

  //doesnt work with fast turns
  // while(true){
  //     SetMotor(mLeft, 0.1);
  //     SetMotor(mRight, 0.15);
  //   if (!r){
  //     SetMotor(mLeft, 0.15);
  //     SetMotor(mRight, 0.1);
  //     WaitUS(150);
  //   }
  //   r = ADCRead(adc[0]) < 0.5;
  //   m = ADCRead(adc[1]) < 0.5;
  //   l = ADCRead(adc[2]) < 0.5;
  // }
  // if (!r && !m){
  //   SetMotor(mLeft, 0.1);
  // } else {
  //   SetMotor(mRight, 0.1);
  // }
  // while(r && m && l){
  //   Wait(3);
  //   SetMotor(mLeft, 0.1);
  //   SetMotor(mRight, 0.1);
  //   Wait(3);
  //   SetMotor(mLeft, 0.0);
  //   SetMotor(mRight, 0.0);
  // }


  // while(true){
    // while (true){
    //   if (l && m && !r){
    //     SetMotor(mLeft, 0.1);
    //     SetMotor(mRight, 0);
    //   } else if (l && !m && !r){
    //     SetMotor(mLeft, 0.1);
    //     SetMotor(mRight, 0.0);
    //   } else if (!l && m && r){
    //     SetMotor(mLeft, 0.0);
    //     SetMotor(mRight, 0.1);
    //   } else if (!l && !m && r){
    //     SetMotor(mLeft, 0.0);
    //     SetMotor(mRight, 0.1);
    //   } else {
    //     SetMotor(mLeft, 0.1);
    //     SetMotor(mRight, 0.1);
    //   }
    //   WaitUS(15);
    //   SetMotor(mLeft, 0);
    //   SetMotor(mRight, 0);
    //   r = ADCRead(adc[0]) < 0.5;
    //   m = ADCRead(adc[1]) < 0.5;
    //   l = ADCRead(adc[2]) < 0.5;
    //   Wait(2);
    // }
    // SetMotor(mLeft, 0);
    // SetMotor(mRight, 0);
    // Wait(3);
  // }
  return 0;
}
