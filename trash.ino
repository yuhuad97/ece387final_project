#include <Servo.h>//library for servo motor
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <dht.h>

Servo servo_11;//Declare servo variables
volatile float chao1;//Ultrasonic 1 variable
volatile float chao2;//Ultrasonic 2 variable
LiquidCrystal_I2C mylcd(0x27,16,2);//lcd 1602 address
dht myDHT_A3;//DHT11
volatile int guang;//Light variable
volatile int shidu;//Humidity variable
volatile int wendu;//Temperature variable

int dht_A3_gethumidity() //湿度函数
{
  int chk = myDHT_A3.read11(A3);
  int value = myDHT_A3.humidity;
  return value;
}

int dht_A3_gettemperature() //Temperature function
{
  int chk = myDHT_A3.read11(A3);
  int value = myDHT_A3.temperature;
  return value;
}

float checkdistance_4_3()//First ultrasound
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

float checkdistance_6_7() //Second ultrasound
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

void setup()//Set function
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
  pinMode(8, OUTPUT);//buzzer
  digitalWrite(8, LOW);//The buzzer is triggered at high level, so the default setting is low level
  pinMode(9, OUTPUT);//light led
  digitalWrite(9, LOW);//
  servo_11.attach(11);//Servo pin
}

void loop()
{
   guang = analogRead(A2);
   wendu = dht_A3_gettemperature();
  shidu = dht_A3_gethumidity();
  chao1 = checkdistance_4_3();//3E4T
  chao2 = checkdistance_6_7();//6e7t
  Serial.println(chao1);//Use the serial port to print the distance of ultrasonic 1 and 2 respectively
  Serial.println(chao2);
  mylcd.clear();
  mylcd.setCursor(1-1, 1-1);
  mylcd.print(String("GUANG ") + String(guang));//Light display
  mylcd.setCursor(1-1, 0-1);
  mylcd.print(String("TEMP ") + String(wendu));//temperature display
  mylcd.setCursor(10-1, 0-1);
  mylcd.print(String("RH ") + String(shidu));//Humidity display
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
    if (chao1 <= 15) //The distance between the person close to the trash can is less than 15 and the lid of the trash can is opened
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
    if (chao2 <= 4)//If Ultrasonic 2 detects that the bin is full of garbage, it will alarm
    {
    digitalWrite(8, HIGH);
    }
    else 
    {
      digitalWrite(8, LOW);
    }
  }
  }
