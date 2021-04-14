#include <PWM.h>

#define rt_pin1 14
#define rt_pin2 15
#define fan_pin1 3
#define fan_pin2 11

int32_t frequency = 25000;

int8_t fan_pwm1 = 0;
int8_t fan_pwm2 = 0;

int8_t pwm1 = 0;
int8_t pwm2 = 0;

int8_t min_pwm1 = 100;
int8_t min_pwm2 = 100;

void setup()
{
    InitTimers();
    SetPinFrequency(fan_pin1, frequency);
    SetPinFrequency(fan_pin2, frequency);

    //为了兼容旧版PCB，使用最新的PCB不需要这一句，加上也无所谓
    pinMode(5, INPUT);

    pinMode(rt_pin1, INPUT);
    pinMode(rt_pin2, INPUT);
    pinMode(fan_pin1, OUTPUT);
    pinMode(fan_pin2, OUTPUT);

    delay(250000);
    set_fan();
}

void loop()
{
    read_temp();
    set_fan();
    delay(50000);
}

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

void read_temp()
{
    float v_rt1 = analogRead(rt_pin1) / 204.6;
    float v_rt2 = analogRead(rt_pin2) / 204.6;

    pwm1 = floor((254 * (-20 * v_rt1 + 80)) / 50);
    pwm2 = floor((254 * (-20 * v_rt2 + 80)) / 50);

    if (pwm1 < min_pwm1)
    {
        pwm1 = 0;
    }
    else if (pwm1 > 254)
    {
        pwm1 = 254;
    }

    if (pwm2 < min_pwm2)
    {
        pwm2 = 0;
    }
    else if (pwm2 > 254)
    {
        pwm2 = 254;
    }
}
