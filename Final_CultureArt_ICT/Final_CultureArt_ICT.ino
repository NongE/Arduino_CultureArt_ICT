// 네오픽셀 관련 라이브러리 선언 //
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// LED 선언 //
#define LED_PIN    13 // 네오픽셀 13번 핀에 연결
#define LED_COUNT 60 // 네오픽셀의 총 갯수는 60개

// 압력센서 5개 선언
int fsrSensor0 = A0; // 1번 압력센서 A0에 연결


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

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
}


void loop() {

  int fsr0 = analogRead(fsrSensor0); // A0로 부터 아날로그 신호를 읽고 이를 fsr0에 저장
  int transFsr0 = map(fsr0, 0, 1024, 0, 255); // maapping 실시

  // transFsr0 (A0)에 압력 센서가 감지되면
  if (transFsr0 > 5)
  {
    colorWipe(strip.Color(0,   255,   0), 0, 9); // 0~9번 led On
  }
  else
  {
    colorWipe(strip.Color(0,   0,   0), 0, 9); // 인식이 안된다면 led Off
  }

}

// LED on관련 함수, 색 정보, 시작점과 끝점을 입력받는다.
void colorWipe(uint32_t color, int startPoint, int endPoint) {

  for (int i = startPoint; i < endPoint; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(30);
  }

}
