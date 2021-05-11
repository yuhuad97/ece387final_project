#include <Servo.h>//舵机库
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dht.h>

Servo servo_11;//声明舵机变量
volatile float chao1;//超声波1变量
volatile float chao2;//超声波2变量
LiquidCrystal_I2C mylcd(0x27,16,2);//1602地址
dht myDHT_A3;//DHT11
volatile int guang;//光线变量
volatile int shidu;//湿度变量
volatile int wendu;//温度变量

int dht_A3_gethumidity() //湿度函数
{
  int chk = myDHT_A3.read11(A3);
  int value = myDHT_A3.humidity;
  return value;
}

int dht_A3_gettemperature() //温度函数
{
  int chk = myDHT_A3.read11(A3);
  int value = myDHT_A3.temperature;
  return value;
}

float checkdistance_4_3()//第一个超声波
{
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  float distance = pulseIn(3, HIGH) / 58.00;
  delay(10);
  return distance;
}

float checkdistance_6_7() //第二个超声波
{
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  float distance = pulseIn(7, HIGH) / 58.00;
  delay(10);
  return distance;
}

void setup()//设置函数
{
  chao1 = 0;
  chao2 = 0;
  Serial.begin(9600);
  mylcd.init();
  mylcd.backlight();
  pinMode(4, OUTPUT);
  pinMode(3, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, OUTPUT);//蜂鸣器
  digitalWrite(8, LOW);//蜂鸣器高电平触发所以默认设置为低电平
  pinMode(9, OUTPUT);//灯
  digitalWrite(9, LOW);//
  servo_11.attach(11);//舵机的引脚
}

void loop()
{
   guang = analogRead(A2);
   wendu = dht_A3_gettemperature();
  shidu = dht_A3_gethumidity();
  chao1 = checkdistance_4_3();//3E4T
  chao2 = checkdistance_6_7();//6e7t
  Serial.println(chao1);//分别用串口打印超声波1和2的距离
  Serial.println(chao2);
  mylcd.clear();
  mylcd.setCursor(1-1, 1-1);
  mylcd.print(String("GUANG ") + String(guang));//光线显示
  mylcd.setCursor(1-1, 0-1);
  mylcd.print(String("TEMP ") + String(wendu));//温度显示
  mylcd.setCursor(10-1, 0-1);
  mylcd.print(String("RH ") + String(shidu));//湿度显示
  delay(100);
if(guang>500)
{
  digitalWrite(9, HIGH);
  jiance();
  }
else
{
  digitalWrite(9, LOW);
  jiance();
  }
}

void jiance()
{
    if (chao1 <= 15) //人靠近垃圾桶距离小于15了垃圾桶盖子打开
  {
    do
    {
    servo_11.write(20);
    delay(100);
    delay(1500);
    }
while(chao1 >= 15);
  }
  else //否则关闭
  {
    servo_11.write(90);
    delay(100);
    if (chao2 <= 4)//如果超声波2检测到桶内垃圾满了报警 
    {
    digitalWrite(8, HIGH);
    }
    else 
    {
      digitalWrite(8, LOW);
    }
  }
  }
