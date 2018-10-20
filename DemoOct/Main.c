
#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>

static tMotor *left;
static tMotor *right;

int main() {
    left = InitializeServoMotor(PIN_B6, false);
    right = InitializeServoMotor(PIN_B7, true);

//    int direction = 1;
//    float speed = 0;
//    while(true) {
//        if(speed <= 0.6)
//            direction = 1;
//        else if(speed > 0.6)
//            direction = -1;
//    speed = speed + direction * 0.1;
//    Wait(1);
//    }
    SetMotor(left, 1.0);
    SetMotor(right, 1.0);
}
