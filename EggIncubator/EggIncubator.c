#include <MsTimer2.h>
#include <core_build_options.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <core_build_options.h>
//�½��� ����
#define DHTPIN 4     // ������ 4���� ���
#define DHTTYPE DHT11   // DHT 11 ��
DHT dht(DHTPIN, DHTTYPE);

//LCD ���
volatile byte state = LOW;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//���� ����
const int TS = 12; // ���� ���� ��Ʈ
int sensorstate = 0;

//����
int speakerPin = 8;
int numTones = 8;
int tones[] = { 3520, 1568, 3520, 1568, 3520, 1568, 3520, 1568 };

//����ġ
int sw1 = 2;
int led1 = 10;
int sw2 = 13;
int flag = 0;

//Ÿ�̸�
int day = 0, hour = 0;
int t_flag = 0;

void timer_LCD()
{
    hour++;
    if (hour == 24)
    {
        hour = 0;
        day++;
    }
}
void setup()
{
    pinMode(5, OUTPUT);
    pinMode(sw1, INPUT_PULLUP);//LED_yellow
    pinMode(sw2, INPUT_PULLUP);
    pinMode(led1, OUTPUT);
    lcd.init();
    Serial.begin(9600);
    dht.begin();
    pinMode(TS, INPUT);
    MsTimer2::set(300, timer_LCD);
    attachInterrupt(digitalPinToInterrupt(sw1), light, CHANGE);
}

void loop() {
    //Ÿ�̸�
    if (flag == 0 && digitalRead(sw1) == LOW)
    {
        flag = 1;
        t_flag = 0;
    }
    else if (flag == 1 && digitalRead(sw1) == LOW)
    {
        flag = 0;
        day = 0;
        hour = 0;
        MsTimer2::stop();
    }
    if (flag == 1 && t_flag == 0) {
        MsTimer2::set(300, timer_LCD);
        MsTimer2::start();
    }

    //���� ����
    sensorstate = digitalRead(TS);
    if (sensorstate == HIGH) {
        t_flag = 1;
        MsTimer2::stop();
        for (int i = 0; i < numTones; i++)
        {
            tone(speakerPin, tones[i]);
            delay(50);
        }
        noTone(speakerPin);
        delay(10);
    }
    else {
        noTone(speakerPin);
    }
    //����ġ
    if (digitalRead(sw2) == LOW)
        digitalWrite(led1, HIGH);
    else
        digitalWrite(led1, LOW);

    //�½��� ����
      //float h = dht.readHumidity(); 
    float t = dht.readTemperature(); // ���� �µ� ����
    if (t < 25) {
        state = HIGH;
        digitalWrite(5, state);
    }
    else if (t > 30) {
        state = LOW;
        digitalWrite(5, state);
    }
    //LCD ��� 
    lcd.backlight();
    lcd.display();
    lcd.setCursor(0, 0);
    lcd.print("TEMP:     ");
    lcd.print(t);
    lcd.setCursor(0, 1);
    lcd.print("day:");
    lcd.print(day);
    lcd.print(" ");
    lcd.print("hour:");
    lcd.print(hour);

    delay(100);
    lcd.clear();

}
void light() {
    if (digitalRead(10) == LOW)
        digitalWrite(led1, HIGH);
    else
        digitalWrite(led1, LOW);
}