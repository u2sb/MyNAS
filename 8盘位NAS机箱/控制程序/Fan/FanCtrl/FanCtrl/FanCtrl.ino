/*
 Name:		FanCtrl.ino
 Created:	2021/5/8 14:51:38
 Author:	MonoLogueChi
*/

#include <PWM.h>

#define rt_pin1 14
#define rt_pin2 15
#define fan_pin1 3
#define fan_pin2 11

int32_t frequency = 25000;

int16_t fan_pwm1 = 0;
int16_t fan_pwm2 = 0;

int16_t pwm1 = 0;
int16_t pwm2 = 0;

int16_t min_pwm1 = 90;
int16_t min_pwm2 = 90;

// the setup function runs once when you press reset or power the board
void setup() {
    InitTimersSafe();
    SetPinFrequencySafe(fan_pin1, frequency);
    SetPinFrequencySafe(fan_pin2, frequency);

    pinMode(rt_pin1, INPUT);
    pinMode(rt_pin2, INPUT);
    pinMode(fan_pin1, OUTPUT);
    pinMode(fan_pin2, OUTPUT);

    delay(1000);
}

// the loop function runs over and over again until power down or reset
void loop() {
    read_temp();
    set_fan();
    delay(2000);
}

//设置风扇转速
void set_fan()
{
    if (pwm1 != fan_pwm1)
    {
        fan_pwm1 = pwm1;
        pwmWrite(fan_pin1, fan_pwm1);
    }

    if (pwm2 != fan_pwm2)
    {
        fan_pwm2 = pwm2;
        pwmWrite(fan_pin2, fan_pwm2);
    }
}

//读取温度
void read_temp()
{
    float v_rt1 = analogRead(rt_pin1) / 204.6;
    float v_rt2 = analogRead(rt_pin2) / 204.6;

    pwm1 = static_cast<int>(5 * (-20 * v_rt1 + 80));
    pwm2 = static_cast<int>(5 * (-20 * v_rt2 + 80));

    if (pwm1 < min_pwm1)
    {
        pwm1 = 0;
    }
    else if (pwm1 > 254)
    {
        pwm1 = 255;
    }

    if (pwm2 < min_pwm2)
    {
        pwm2 = 0;
    }
    else if (pwm2 > 254)
    {
        pwm2 = 255;
    }
}
