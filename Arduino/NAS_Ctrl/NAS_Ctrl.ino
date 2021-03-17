#define TmpPin 14;   //A0 温度检测
#define TmpR1Pin 15; //A1 调温电阻1
#define TmpR2Pin 16; //A2 调温电阻2
#define PwPin 18;    //A4 开关按钮
#define Pw_OkPin 19; //A5 检测电源是否开启

#define PC_PwONPin 2; //D2 电脑开机键
#define Fun_PwnPin 3; //D3 风扇输出调速
#define Ps_ONPin 4;   //D4 电源启动开关
#define Disk1Pin 6;   //D5 硬盘控制1
#define Disk2Pin 7;   //D6 硬盘控制2
#define Disk3Pin 8;   //D7 硬盘控制3

//0 关闭   1 开机，  2 正在开机
int IsOn = 0;

void setup()
{
  //设置pinMode
  pinMode(TmpPin, INPUT);
  pinMode(TmpR1Pin, INPUT);
  pinMode(TmpR2Pin, INPUT);
  pinMode(PwPin, INPUT);
  pinMode(Pw_OkPin, INPUT);

  pinMode(PC_PwONPin, OUTPUT);
  pinMode(Fun_PwnPin, OUTPUT);
  pinMode(Ps_ONPin, OUTPUT);
  pinMode(Disk1Pin, OUTPUT);
  pinMode(Disk2Pin, OUTPUT);
  pinMode(Disk3Pin, OUTPUT);

  init();
}

void loop()
{

  // put your main code here, to run repeatedly:
}

void init()
{
  //将状态设置为关机
  IsOn = 0;
  //给磁盘开关高电平，高电平控制MOS管关闭
  digitalWrite(Disk1Pin, HIGH);
  digitalWrite(Disk1Pin, HIGH);
  digitalWrite(Disk1Pin, HIGH);
}

//开机程序
void SetPcON()
{
  IsOn = 2;

  digitalWrite(PC_PwONPin, HIGH);
  sleep(500);
  digitalWrite(PC_PwONPin, LOW);
  IsOn = 1;
}

//开启电源
void OpenPw()
{
  digitalWrite(Ps_ONPin, HIGH);
}

//复位电源开关
void ResetPw()
{
  digitalWrite(Ps_ONPin, LOW);
}

//检查是否已关机
void CheckPwOff()
{
  if (IsOn == 0 && !CheckPwIsON())
  {
    return true;
  }

  if (IsOn == 1)
  {
    if (CheckPwIsON())
    {
      return true;
    }
    else
    {
      //设置状态为开机，但是电源没有输出，需要再次检测
      sleep(500);
      //0.5s后再次检查是否有输出
      if (CheckPwIsON())
      {
        //有输出，直接返回true
        return true;
      }
      else
      {
        //没有输出，将状态设置为关机，并且重置状态
        init();
      }
    }
  }
}

//检测电源是否已经通电，通电开机返回true，否则返回false
boolean CheckPwIsON()
{
  if (digitalRead(Pw_OkPin) == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}
