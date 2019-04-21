#include <Arduino.h>

#define PWR D0 // GPIO 16 (has a pulldown res, the others have a pullup)
#define VOLUP D1 // VOL+
#define VOLDWN D2 // VOL-





void setup() {
  pinMode(PWR, OUTPUT);
  digitalWrite(PWR, 255);

  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
}


void increaseVolume() {
  // Clockwise => V+ raises first (5-6ms / 20ms), then
  // V- raises, then 40ms
  // V+ sinks
  // V- sinks


  //
  //
  //   20         40
  //  +++-+-+-+-+-----
  //  +  -      +    -
  //  +  -      +    -
  //  +  -      +    -


  pinMode(VOLUP, OUTPUT);
  pinMode(VOLDWN, OUTPUT);

  digitalWrite(VOLUP, HIGH);
  delay(20);
  digitalWrite(VOLDWN, HIGH);
  delay(70);
  digitalWrite(VOLUP, LOW);
  delay(40);
  digitalWrite(VOLDWN, LOW);
  delay(50);

}


void pushPower() {
  // set pin LOW (pulldown)
  pinMode(PWR, INPUT_PULLDOWN_16);
  int read = digitalRead(PWR);
  Serial.printf("POWER: %i", read);

  // wait
  delay(200);

  // set pin back to HIGH again
  pinMode(PWR, OUTPUT);
  digitalWrite(PWR, 255);
}


void loop() {
  delay(2 * 1000);
  increaseVolume();
}
