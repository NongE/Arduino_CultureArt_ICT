// 네오픽셀 관련 라이브러리 선언 //
#include <Adafruit_NeoPixel.h>
//  MP3 관련 라이브러리 선언 //
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

// LED 선언 //
#define LED_PIN    13 // 네오픽셀 13번 핀에 연결
#define LED_COUNT 60 // 네오픽셀의 총 갯수는 60개
SoftwareSerial mySerial(6, 7); // mp3 모듈 관련 가상핀 6,7번 선언

// 압력센서 5개 선언
int fsrSensor0 = A0; // 1번 압력센서 A0에 연결
int fsrSensor1 = A1; // 2번 압력센서 A1에 연결
int fsrSensor2 = A2; // 3번 압력센서 A2에 연결
int fsrSensor3 = A3; // 4번 압력센서 A3에 연결
int fsrSensor4 = A4; // 5번 압력센서 A4에 연결
int count = 0;

// 들어온 발판을 끌때 해당 발판이 밟인 적이 있는지 확인하는 flag
int fsrFlag0 = 0;
int fsrFlag1 = 0;
int fsrFlag2 = 0;
int fsrFlag3 = 0;
int fsrFlag4 = 0;

// 모드 변경을 위한 택트 스위치 관련 변수
int modeSwitch = 11;  // 택트 스위치는 12번 디지털 핀에 들어감
int modeState = HIGH; // 올라온 상태
int modePrevious = LOW; // 이전 상태
int modeRead; // 택트 스위치 디지털 신호를 읽어서 변환
int modeFlag = 0; // 택트 스위치의 변화에 따라 모드 전환을 위한 플래그

// 신호등 상태 변경을 위한 택트 스위치 관련 변수
int signalSwitch = 12;  // 택트 스위치는 12번 디지털 핀에 들어감
int signalState = HIGH; // 올라온 상태
int signalPrevious = LOW; // 이전 상태
int signalRead; // 택트 스위치 디지털 신호를 읽어서 변환
int signalFlag = 0; // 신호등 상태 1은 빨간불, 0은 초록불


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // 네오픽셀 정의

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  // 네오픽셀 시작 //
  strip.begin();
  strip.show();
  strip.setBrightness(255);

  // 시리얼 포트 9600번 선언 //
  Serial.begin(9600);

  // 택트 스위치 선언 //
  pinMode(modeSwitch, INPUT_PULLUP);
  pinMode(signalSwitch, INPUT_PULLUP);

  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module
  //delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (30);
  mp3_set_EQ(3);
}


void loop() {

  // 모드 택트 스위치 상태 확인 //
  modeRead = digitalRead(modeSwitch);
  if (modeRead == HIGH && modePrevious == LOW)
  {
    if (modeState == HIGH)
      modeState = LOW;
    else
      modeState = HIGH;

    // 모드 변경 시 시각적으로 확인하기 위해 white color wipe
    colorWipe(strip.Color(255,   255,   255), 0, 60);
    colorOff(strip.Color(0,   0,   0), 0, 60);
    count = 0;

    if (modeFlag == 1)
    {
      modeFlag = 0;
      // 초록불로 변경 후 만일 모드가 3~5세 모드라면 안내 멘트 출력
      count = 0;
      Serial.println("3~5세 모드로 변경합니다.");
    }
    else
    {
      colorWipe(strip.Color(255,   255,   255), 0, 60);
      colorOff(strip.Color(0,   0,   0), 0, 60);
      modeFlag = 1;
      Serial.println("6~7세 모드로 변경합니다.");
    }
  }
  modePrevious = modeRead;
  // Serial.print("modeFlag is ");
  //Serial.println(modeFlag);

  // 신호등 택트 스위치 상태 확인 //
  signalRead = digitalRead(signalSwitch);
  if (signalRead == HIGH && signalPrevious == LOW)
  {
    if (signalState == HIGH)
      signalState = LOW;
    else
      signalState = HIGH;


    if (signalFlag == 1)
    {
      // 초록불로 변경 시 green color wipe
      colorWipe(strip.Color(0,   255,   0), 0, 60);
      colorOff(strip.Color(0,   0,   0), 0, 60);
      signalFlag = 0;
      count = 0;
      Serial.println("신호등 상태를 초록불로 변경합니다.");
      if (modeFlag == 0)
      {
        mp3_play (2); // 2번 멈춘다~
        delay(15000);
        mp3_stop();
      }
      //delay (4000);
    }
    else
    {

      // 빨간불로 변경 시 red color wipe
      colorWipe(strip.Color(255,   0,   0), 0, 60);
      colorOff(strip.Color(0,   0,   0), 0, 60);
      signalFlag = 1;
      Serial.println("신호등 상태를 빨간불로 변경합니다.");
    }
  }
  signalPrevious = signalRead;

  //Serial.print("signalFag is ");
  //Serial.println(signalFlag);



  int fsr0 = analogRead(fsrSensor0); // A0로 부터 아날로그 신호를 읽고 이를 fsr0에 저장
  int transFsr0 = map(fsr0, 0, 1024, 0, 255); // maapping 실시

  int fsr1 = analogRead(fsrSensor1); // A0로 부터 아날로그 신호를 읽고 이를 fsr0에 저장
  int transFsr1 = map(fsr1, 0, 1024, 0, 255); // maapping 실시

  int fsr2 = analogRead(fsrSensor2); // A0로 부터 아날로그 신호를 읽고 이를 fsr0에 저장
  int transFsr2 = map(fsr2, 0, 1024, 0, 255); // maapping 실시

  int fsr3 = analogRead(fsrSensor3); // A0로 부터 아날로그 신호를 읽고 이를 fsr0에 저장
  int transFsr3 = map(fsr3, 0, 1024, 0, 255); // maapping 실시

  int fsr4 = analogRead(fsrSensor4); // A0로 부터 아날로그 신호를 읽고 이를 fsr0에 저장
  int transFsr4 = map(fsr4, 0, 1024, 0, 255); // maapping 실시

  // 신호등 상태가 빨간불인 상태에 진입하게 되면 전체 red wipe 들어오기
  if (signalFlag == 1 && (transFsr0 > 5 || transFsr1 > 5 ||
                          transFsr2 > 5 || transFsr3 > 5 || transFsr4 > 5))
  {
    warningWipe(strip.Color(255,   0,   0), 0, 60);
    colorOff(strip.Color(0,   0,   0), 0, 60);
  }


  // 신호등 상태가 초록불이라면 정상적으로 진행
  else {
    // transFsr4 (A0)에 압력 센서가 감지되면
    if (transFsr4 > 20)
    {
      fsrFlag4 = 1;
      colorWipe(strip.Color(0,   255,   0), 0, 9); // 0~9번 led On
      if (modeFlag == 0)
      {
        count ++;
        mp3_play (3);
        mp3_stop();
      }
    }
    else
    {
      if (fsrFlag4 == 1)
      {
        colorOff(strip.Color(0,   0,   0), 0, 9);
        fsrFlag4 = 0;
      }
    }


    // transFsr3 (A3)에 압력 센서가 감지되면
    if (transFsr3 > 20)
    {

      fsrFlag3 = 1;
      colorWipe(strip.Color(0,   255,   0), 10, 20); // 10~20번 led On
      if (modeFlag == 0)
      {
        count ++;
        mp3_play (4);
        mp3_stop();
      }
    }
    else
    {
      if (fsrFlag3 == 1)
      {

        colorOff(strip.Color(0,   0,   0), 10, 20);
        fsrFlag3 = 0;
      }

    }


    // transFsr2 (A2)에 압력 센서가 감지되면
    if (transFsr1 > 20)
    {
      fsrFlag1 = 1;
      colorWipe(strip.Color(0,   255,   0), 24, 30); // 24~30번 led On
      if (modeFlag == 0)
      {
        count ++;
        mp3_play (6);
        mp3_stop();
      }
    }
    else
    {
      if (fsrFlag1 == 1)
      {
        colorOff(strip.Color(0,   0,   0), 24, 30);
        fsrFlag1 = 0;
      }

    }


    // transFsr4 (A4)에 압력 센서가 감지되면
    if (transFsr0 > 20)
    {
      fsrFlag0 = 1;
      colorWipe(strip.Color(0,   255,   0), 36, 46); // 36~46번 led On
      if (modeFlag == 0)
      {
        count ++;
        mp3_play (7);
        mp3_stop();
      }
    }
    else
    {
      if (fsrFlag0 == 1)
      {
        colorOff(strip.Color(0,   0,   0), 36, 46);
        fsrFlag0 = 0;
      }

    }


    // transFsr1 (A1)에 압력 센서가 감지되면
    if (transFsr2 > 20)
    {
      fsrFlag2 = 1;
      colorWipe(strip.Color(0,   255,   0), 52, 60); // 52~60번 led On
      if (modeFlag == 0)
      {
        count ++;
        mp3_play (5);
        mp3_stop();
      }
    }
    else
    {
      if (fsrFlag2 == 1)
      {
        colorOff(strip.Color(0,   0,   0), 50, 60);
        fsrFlag2 = 0;
      }
    }


  }

  // Serial.println(count);
  if (count >= 5)
  {
    delay(1000);
    mp3_play(8);
    mp3_stop();
    count = 0;
  }

}

// LED on 관련 함수, 색 정보, 시작점과 끝점을 입력받는다.
void colorWipe(uint32_t color, int startPoint, int endPoint) {

  for (int i = startPoint; i < endPoint; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(25);
  }

}

// 신호등 상태가 빨간불일때 횡단보도 진입 시 나오는 효과
void warningWipe(uint32_t color, int startPoint, int endPoint) {

  for (int i = startPoint; i < endPoint; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(5);
  }

  for (int i = startPoint; i < endPoint; i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(5);
  }

}

// led off 관련
void colorOff(uint32_t color, int startPoint, int endPoint) {

  for (int i = startPoint; i < endPoint; i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(30);
  }

}
