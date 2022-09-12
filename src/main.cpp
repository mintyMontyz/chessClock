#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const int button = 2;
bool player = true;
int seconds1;
int seconds2;
bool TimeSelect = true;
int time1 = 60;
int time2 = time1;
bool delayTime = false;
int times[6] = {60, 180, 300, 600, 900, 1800};
int current = 0;
int selectTime = 0;
int soundPin = 3;
bool finished = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int tick(int Time) {
  delay(1000);
  if (Time >0){
    return Time - 1;
  } else {
    return 0;
  }
}


void ring() {
  for (int i=0; i<400; i++) {
    digitalWrite(soundPin, HIGH);
    delay(1);
    digitalWrite(soundPin, LOW);
    delay(1);
  }
}

void changePlayer() {
  if (delayTime == false) {
    ring();
    selectTime = 0;
    if (TimeSelect) {
      if (current == 6) {
        current = 0;
      }
      time1 = times[current];
      time2 = time1;
      current += 1;
    } else if (TimeSelect == false) {
      if (player) {
        player = false;
      } else if (player == false){
        player = true;
      }
    }
  }
  delayTime = true;
}

int displayedTime(int Time) {
  if (Time > 60) {
    return Time / 60;
  } else {
    return Time;
  } 
}

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(soundPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(button), changePlayer, RISING);
  lcd.init();
  lcd.backlight();

}

void loop() {
  if (TimeSelect) {
    lcd.setCursor(0, 0);
    lcd.print("time: ");
    if (time1/60 < 10) {
      lcd.print("0");
    }
    lcd.print(time1/60);
    if (delayTime) {
      delay(200);
      delayTime = false;
    }
    delay(1000);
    selectTime += 1;
    if (selectTime == 5) {
      TimeSelect = false;
    }
    
  } else {
    if (time1 == 0 || time2 == 0) {
      finished = true;
    }
    if (player) {
      time1 = tick(time1);
    } else {
      time2 = tick(time2);
    }
    seconds1 = time1 % 60;
    seconds2 = time2 % 60;
  
    if (time1 > 60) {
      lcd.setCursor(0, 0);
      lcd.print("Time1: ");
      if (time1 < 600) {
        lcd.print("0");
      }
      lcd.print(displayedTime(time1));
      lcd.print(":");
      if (seconds1 < 10){
        lcd.print("0");
      }
      lcd.print(seconds1);
    } else {
        lcd.setCursor(0, 0);
        lcd.print("seconds1: ");
        if (time1 < 10){
          lcd.print("0");
        }
        lcd.print(displayedTime(time1));
      }
    if (time2 > 60) {
      lcd.setCursor(0, 1);
      lcd.print("Time2: ");
      if (time2 < 600){
        lcd.print("0");
      }
      lcd.print(displayedTime(time2));
      lcd.print(":");
      if (seconds2 < 10){
        lcd.print("0");
      }
      lcd.print(seconds2);
    } else {
        lcd.setCursor(0, 1);
        lcd.print("seconds2: ");
        if (time2 < 10){
          lcd.print("0");
        }
        lcd.print(displayedTime(time2));
      }
      if (finished) {
        ring();
      }
      if (delayTime) {
      delay(200);
      delayTime = false;
    }
  }
}