#include "motor.h"
#include "control.h"
#include "PID.h"
#include "stdio.h"

#define PITCH_KP  (5.0)
#define PITCH_KI  (1)
#define PITCH_KD  (1)

#define ROLL_KP  (5.0)
#define ROLL_KI  (1)
#define ROLL_KD  (1)

static PIDController PitchControl;
static PIDController RollControl;

void ControlInit()
{
    PIDController_Init(&PitchControl);
    PIDControllerSetPar(&PitchControl, PITCH_KP,PITCH_KI,PITCH_KD);

    PIDController_Init(&RollControl);
    PIDControllerSetPar(&RollControl, ROLL_KP,ROLL_KI,ROLL_KD);
}

void Controlling(int pitch, int roll)
{
    float pc = PIDController_Update(&PitchControl, 0.0, pitch);
    float rc = PIDController_Update(&RollControl, 0.0, roll);

    printf("pc:%lf,rc%lf\r\n", pc, rc);
}

void MapPitchToContorl(int pitch)
{
    //90-180
    if(pitch > 0)
    {
        if(pitch>0 && pitch<=10)
        {
            SetMotorAngle(100);
        }
        else if(pitch>10 && pitch<=20)
        {
            SetMotorAngle(110);
        }
        else if(pitch>20 && pitch<=30)
        {
            SetMotorAngle(120);
        }
        else if(pitch>30 && pitch<=40)
        {
            SetMotorAngle(130);
        }
        else if(pitch>40 && pitch<=50)
        {
            SetMotorAngle(140);
        }
        else if(pitch>50 && pitch<=60)
        {
            SetMotorAngle(150);
        }
    }//90-0
    else if(pitch < 0)
    {
        if(pitch>=-10 && pitch<0)
        {
            SetMotorAngle(80);
        }
        else if(pitch>=-20 && pitch<-10)
        {
            SetMotorAngle(70);
        }
        else if(pitch>=-30 && pitch<-20)
        {
            SetMotorAngle(60);
        }
        else if(pitch>=-40 && pitch<-30)
        {
            SetMotorAngle(50);
        }
        else if(pitch>=-50 && pitch<-40)
        {
            SetMotorAngle(40);
        }
        else if(pitch>=-60 && pitch<-50)
        {
            SetMotorAngle(30);
        }   
    }
}
