
#include "Keyboard.h"
#include "Mouse.h"
#include <Wire.h>
int x;

void setup() {
  Wire.begin(9); // 9 здесь адрес (упоминается также в коде основной платы) 
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}
void receiveEvent(int bytes) {
  x = Wire.read(); // Получите значения от основной платы
  Serial.println(x);

  if(x == 1){
    Keyboard.press('w');
    Keyboard.releaseAll();
    }

    if(x == 2){
    Keyboard.press('s');
    Keyboard.releaseAll();
    }

    if(x == 3){
    Mouse.move(1,0,0);
    }

    if(x == 4){
    Mouse.move(-1,0,0);
    }
}
void loop() {
}
