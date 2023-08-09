#include "motor.h"
#include "control.h"

void MapPitchToContorl(int pitch)
{
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
    }
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
