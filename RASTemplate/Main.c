#include <RASLib/inc/adc.h>
#include <RASLib/inc/common.h>
#include <RASLib//inc/gpio.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>

static tBoolean initialized = false;

static tMotor *mLeft;
static tMotor *mRight;

static tADC *adc[3];

static tADC *snrLeft;
static tADC *snrRight;

static tBoolean close = true;


// initializes all of the pins for the line sensor
void initialize(){
    if(initialized){
        return;
    }
    initialized = true;

    // set pins to ADC
    adc[0] = InitializeADC(PIN_D0);
    adc[1] = InitializeADC(PIN_D1);
    adc[2] = InitializeADC(PIN_D2);
    mRight = InitializeServoMotor(PIN_B0, true);
    mLeft = InitializeServoMotor(PIN_B1, false);
    snrLeft = InitializeADC(PIN_E4);
    // snrRight = InitializeADC(PIN_E5);
}

// checks the value of the ADC
// returns false if line sensor sees white
// returns true if line sensor sees black
tBoolean checkADC(tADC *a){
  return ADCRead(a) < 0.5;
}

// tBoolean watch(){
//   return ADCRead(snr) > 0.1;
// }

void rotate(){
  SetMotor(mLeft, -1);
  SetMotor(mRight, 1);
  close = true;
}

int main() {
  initialize();
  float tempLeft;
  float tempRight;
  tBoolean seeLeft = ADCRead(snrLeft) > 0.4;
  //tBoolean seeRight = ADCRead(snrRight) > 0.4;
  tBoolean r = checkADC(adc[0]);
  tBoolean m = checkADC(adc[1]);
  tBoolean l = checkADC(adc[2]);

  // float temp1 = 999999;
  // tBoolean turning = true;
  // SetMotor(mLeft, -0.1);
  // SetMotor(mRight, 0.1);
  while (true){
    // object sensing
    // temp = ADCRead(snr);
    // notSee = temp < 0.375;
    // if(!notSee && turning){
    //   if(temp > temp1 + 0.01){
    //     SetMotor(mLeft, 0.3);
    //     SetMotor(mRight, 0.3);
    //     turning = false;
    //   }
    // }
    // if(!turning && notSee){
    //   SetMotor(mLeft, -0.2);
    //   SetMotor(mRight, 0.2);
    //   turning = true;
    // }
    // temp1 = ADCRead(snr);

    // left distance sensor
    if(!seeLeft){
      SetMotor(mLeft, -1);
      SetMotor(mRight, 1);
      close = true;
    } else if (temp > 0.97 && close){
        SetMotor(mLeft, -1);
        SetMotor(mRight, -0.85);
        Wait(1);
        close = false;
    } else {
      WaitUS(20 - (20 * temp));
      SetMotor(mLeft, 1);
      SetMotor(mRight, 1);
      close = false;
    }
    tempLeft = ADCRead(snrLeft);
    tempRight = ADCRead(snrRight);
    seeLeft = tempLeft > 0.4;
    //seeRight = tempRight > 0.4;
    r = checkADC(adc[0]);
    m = checkADC(adc[1]);
    l = checkADC(adc[2]);

    /* right distance sensor


    */

    // line following
    // SetMotor(mLeft, 0.1);
    // SetMotor(mRight, 0.1);
    // Wait(1);
    // if(r){
    //   SetMotor(mLeft, 0);
    //   SetMotor(mRight, 0);
    //   Wait(2);
    // }
    while(r || m || l){
      if (r && m && l){
      // if (m && l){
        SetMotor(mLeft, -1);
        SetMotor(mRight, -1);
        Wait(1);
      } else if (r && m){
        SetMotor(mLeft, 0.5);
        SetMotor(mRight, 1);
      } else if (l && m){
        SetMotor(mLeft, 1);
        SetMotor(mRight, 0.5);
      } else if (r){
        SetMotor(mLeft, 0.7);
        SetMotor(mRight, 1);
      } else if (l){
        SetMotor(mLeft, 1);
        SetMotor(mRight, 0.7);
      }
      r = checkADC(adc[0]);
      m = checkADC(adc[1]);
      l = checkADC(adc[2]);
    }
    // increasing the value compared to will decrease the distance sensed
    tempLeft = ADCRead(snrLeft);
    tempRight = ADCRead(snrRight);
    seeLeft = tempLeft > 0.4;
    //seeRight = ADCRead(snrRight) > 0.4;
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
