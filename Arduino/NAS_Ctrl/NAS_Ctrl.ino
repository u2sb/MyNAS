#define TempPin 14   //A0 温度检测
#define TempR1Pin 15 //A1 调温电阻1
#define TempR2Pin 16 //A2 调温电阻2
#define PwPin 18     //A4 开关按钮
#define PwOkPin 19   //A5 检测电源是否开启

#define PCPwONPin 2 //D2 电脑开机键
#define FunPwnPin 3 //D3 风扇输出调速
#define PsONPin 4   //D4 电源启动开关
#define PwLedPin 5  //D5 电源LED灯
#define Disk1Pin 6  //D6 硬盘控制1
#define Disk2Pin 7  //D7 硬盘控制2
#define Disk3Pin 8  //D8 硬盘控制3

//0 关闭   1 开机，  2 正在开机
int IsOn = 0;

//风扇速度 0-255
int FunSpeed = 0;

//风扇调速曲线
const int TempK = -225;
const int TempB = 622;

void setup()
{
  //设置pinMode
  pinMode(TempPin, INPUT);
  pinMode(TempR1Pin, INPUT);
  pinMode(TempR2Pin, INPUT);
  pinMode(PwPin, INPUT);
  pinMode(PwOkPin, INPUT);

  pinMode(PCPwONPin, OUTPUT);
  pinMode(FunPwnPin, OUTPUT);
  pinMode(PsONPin, OUTPUT);
  pinMode(PwLedPin, OUTPUT);
  pinMode(Disk1Pin, OUTPUT);
  pinMode(Disk2Pin, OUTPUT);
  pinMode(Disk3Pin, OUTPUT);

  //初始化
  SetInit();

  //延迟10秒自动启动
  delay(10000);
  SetPcON();
}

void loop()
{
  if (!CheckPwOff())
  {
    //开机状态

    //调整风扇转速
    SetFunPwm(ReadTemp());

    //监听开关按键输入
    if (digitalRead(PwPin) == HIGH)
    {
      while (digitalRead(PwPin) == HIGH)
      {
        //当按下开关键时，给主板上电
        digitalWrite(PCPwONPin, HIGH);
        delay(200);
      }
      //释放开机键
      digitalWrite(PCPwONPin, LOW);
    }
  }
  else
  {
    //关机状态
    //监听开关按键输入
    if (digitalRead(PwPin) == HIGH)
    {
      while (digitalRead(PwPin) == HIGH)
      {
        //按下开机键无反应，直到释放
        delay(200);
      }

      //执行开机程序
      SetPcON();
    }
  }

  //每0.5s检测一次，所以必须按住开机键0.5s以上才有用
  delay(500);
  // put your main code here, to run repeatedly:
}

void SetInit()
{
  //将状态设置为关机
  IsOn = 0;

  //关闭LED
  digitalWrite(PwLedPin, LOW);

  // 关闭风扇
  SetFunPwm(0);

  //给磁盘开关高电平，高电平控制MOS管关闭
  digitalWrite(Disk1Pin, HIGH);
  digitalWrite(Disk1Pin, HIGH);
  digitalWrite(Disk1Pin, HIGH);
}

//开机程序
void SetPcON()
{
  IsOn = 2;

  //开启电源
  OpenPw();
  delay(500);

  //点亮LED灯
  digitalWrite(PwLedPin, HIGH);
  //开启风扇，设置转速为大概一半;
  SetFunPwm(125);
  delay(2000);

  //硬盘分批上电
  digitalWrite(Disk1Pin, LOW);
  delay(5000);
  digitalWrite(Disk2Pin, LOW);
  delay(6000);
  digitalWrite(Disk3Pin, LOW);
  delay(6000);

  //给主板开机
  digitalWrite(PCPwONPin, HIGH);
  delay(500);
  digitalWrite(PCPwONPin, LOW);

  //关闭电源开关，并将状态设置为开机
  delay(5000);
  ResetPw();
  IsOn = 1;
}

//开启电源
void OpenPw()
{
  digitalWrite(PsONPin, HIGH);
}

//复位电源开关
void ResetPw()
{
  digitalWrite(PsONPin, LOW);
}

//检查是否已关机，开机 false，关机或其他状态 true
boolean CheckPwOff()
{
  if (IsOn == 0)
  {
    if (!CheckPwIsON())
    {
      return true;
    }
    else
    {
      // 关机状态，但是已接通电源，将状态设置为开机，并且返回false
      IsOn = 1;
      return false;
    }
  }

  if (IsOn == 1)
  {
    if (CheckPwIsON())
    {
      //开机状态，并且电源通电，返回false
      return false;
    }
    else
    {
      //设置状态为开机，但是电源没有输出，需要再次检测
      delay(500);
      //0.5s后再次检查是否有输出
      if (CheckPwIsON())
      {
        //有输出，直接返回true
        return false;
      }
      else
      {
        //没有输出，将状态设置为关机，并且重置状态
        SetInit();
        return true;
      }
    }
  }

  return true;
}

//检测电源是否已经通电，通电开机返回true，否则返回false
boolean CheckPwIsON()
{
  if (digitalRead(PwOkPin) == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//设置风扇转速
void SetFunPwm(int val)
{
  if (val == 0 && FunSpeed != 0)
  {
    FunSpeed = val;
    analogWrite(FunPwnPin, val);
  }
  else
  {
    if (abs(FunSpeed - val) > 5)
    {
      FunSpeed = val;
      analogWrite(FunPwnPin, val);
    }
  }
}

//读取温度
//NTC B3950 10K 串联 8.06K电阻，测量热敏电阻两侧电压
int ReadTemp()
{
  int tempV = analogRead(TempPin);
  int k = TempK + int(analogRead(TempR1Pin)) * 50;
  int b = TempB + int(analogRead(TempR1Pin)) * 200;

  int p = k * tempV + b;

  if (p > 255)
  {
    return 255;
  }
  if (p < 0)
  {
    return 0;
  }
  return p;
}
