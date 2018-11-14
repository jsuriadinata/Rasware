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

static tBoolean r;
static tBoolean m;
static tBoolean l;

static float tempLeft = 0;
static float tempRight = 0;

static tBoolean seeLeft;
static tBoolean seeRight;

static tBoolean first = true;

static tBoolean leftLost;
static tBoolean rightLost;


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
    snrRight = InitializeADC(PIN_B5);
}

// checks the value of the ADC
// returns false if line sensor sees white
// returns true if line sensor sees black
tBoolean checkADC(tADC *a){
  return ADCRead(a) < 0.5;
}

// watches for object using distance sensor
float watch(int x){
  // increasing the value compared to will decrease the distance sensed
  tempLeft = ADCRead(snrLeft);
  tempRight = ADCRead(snrRight);
  seeLeft = tempLeft > 0.38;
  seeRight = tempRight > 0.38;
  if (x = 0){
    return tempLeft;
  } else {
    return tempRight;
  }
}

void rotateLeft(){
  SetMotor(mLeft, -0.5);
  SetMotor(mRight, 0.5);
}

void forwardLeft(){
  SetMotor(mLeft, 0.25);
  SetMotor(mRight, 1);
}

void rotateRight(){
  SetMotor(mLeft, 0.5);
  SetMotor(mRight, -0.5);
}

void forwardRight(){
  SetMotor(mLeft, 1);
  SetMotor(mRight, 0.25);
}

void forward(){
  SetMotor(mLeft, 1);
  SetMotor(mRight, 1);
}

void checkWhite(){
  r = checkADC(adc[0]);
  m = checkADC(adc[1]);
  l = checkADC(adc[2]);
}

void whiteSensor(){
  checkWhite();
  while(r || m || l){
    if (r && m && l){
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
      rotateLeft();
    } else if (l){
      rotateRight();
    }
    checkWhite();
  }
}

void detection(){
  if(seeLeft && seeRight){
    // Wait(0.1 * (20 - (20 * tempLeft)));
    forward();
    leftLost = false;
    rightLost = false;
    // close = true;
  // } else if (tempLeft > 0.97 && close){
  //     SetMotor(mLeft, -1);
  //     SetMotor(mRight, -0.85);
  //     Wait(1);
  //     close = false;
  } else if (seeLeft && !seeRight){
    // SetMotor(mLeft, 0);
    forwardLeft();
    rightLost = true;
    leftLost = false;
  } else if (!seeLeft && seeRight){
    // SetMotor(mRight, 0);
    forwardRight();
    leftLost = true;
    rightLost = false;
  } else if (leftLost){
    rotateRight();
  } else if (rightLost){
    rotateLeft();
    // SetMotor(mLeft, 0);
    // SetMotor(mRight, 0);
    // close = false;
  }
}

int main() {
  initialize();
  int x = 0;
  int left = 0;
  int right = 0;
  while(x < 5 && first){
    left = watch(0);
    right = watch(1);
    SetMotor(mLeft, -0.1);
    SetMotor(mRight, -1);
    Wait(0.2);
    watch(0);
    x++;
  }
  rotateLeft();

  // float temp1 = 999999;
  // tBoolean turning = true;
  // SetMotor(mLeft, -0.1);
  // SetMotor(mRight, 0.1);
  while (true){
    first = false;
    watch(0);
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
    // whiteSensor();
    detection();
    // line following
    // SetMotor(mLeft, 0.1);
    // SetMotor(mRight, 0.1);
    // Wait(1);
    // if(r){
    //   SetMotor(mLeft, 0);
    //   SetMotor(mRight, 0);
    //   Wait(2);
    // }
    checkWhite();
    watch(0);
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
