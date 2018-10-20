#include "RASLib/inc/common.h"
#include "RASLib/inc/linesensor.h"

static tADC *adc[3];
static tBoolean initialized = false;

int main() {
  initGPIOLineSensor();
}

// initializes all of the pins for the line sensor
void initGPIOLineSensor(void){
    if(initialized){
        return;
    }
    initialized = true;

    // set pins to ADC
    adc[0] = InitializeADC(PIN_D0);
    adc[1] = InitializeADC(PIN_D1);
    adc[2] = InitializeADC(PIN_D2);
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
