#include <pt.h>
#define LED_1_PIN 9

static struct pt pt1;
static struct pt pt2;

static int protothreadBlinkLED1(struct pt *pt)
{
  static unsigned long lastTimeBlink = 0;
  PT_BEGIN(pt);
  while(1) {
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 1000);
    digitalWrite(LED_1_PIN, HIGH);
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 1000);
    digitalWrite(LED_1_PIN, LOW);
  }
  PT_END(pt);
}

static int print_thread(struct pt *pt) {
  PT_BEGIN(pt);
  while(1) {
    int startTime = millis();
    PT_WAIT_UNTIL(pt, millis() - startTime > 2000);
    Serial.println("HERE");
    startTime = millis();
    PT_WAIT_UNTIL(pt, millis() - startTime > 2000);
  }
  PT_END(pt);
}

void setup() {
  pinMode(LED_1_PIN, OUTPUT);
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  Serial.begin(9600);
}

void loop() {
  print_thread(&pt2);
  protothreadBlinkLED1(&pt1);
}
