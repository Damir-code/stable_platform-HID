/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
 *   данных, который генерируется согласно показаниям с кнопки и с 
 *   двух потенциомтеров. Приёмник (RX) получает массив, и записывает
 *   данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/
//TX out
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
#include "MPU9250.h"
MPU9250 mpu;

RF24 radio(8, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte counter;



bool isSended = false;

const float Y_FORWARD = 23.0;
const float Y_BACK = 12.0;
const float Z_RIGHT = -14.0;
const float Z_LEFT = 14.0;

void setup() {
      Serial.begin(9600);
    Wire.begin();
    delay(2000);

    mpu.setup(0x68);
  //Serial.begin(9600); //открываем порт для связи с ПК

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}

void loop() {
byte forwardSignal = 1;
byte backSignal = 2;
byte rightSignal = 3;
byte leftSignal = 4;
byte nullSignal = 0;

if (mpu.update()) { 
  if(mpu.getRoll() > 10){
    radio.write(&forwardSignal, sizeof(forwardSignal));
    Serial.println('F');
    isSended = true;
    }
}

if (mpu.update()) {
  if(mpu.getRoll() < -10){
    radio.write(&backSignal, sizeof(byte));
    Serial.println('B');
    isSended = true;
  }
}

if (mpu.update()) {
  if(mpu.getPitch() > 10){
    radio.write(&rightSignal, sizeof(byte));
    Serial.println('R');
    isSended = true;
    }
}
if (mpu.update()) {
  if(mpu.getPitch() < -10){
    radio.write(&leftSignal, sizeof(byte));
    Serial.println('L');
    isSended = true;
    }
}

if (mpu.update()) {
  if(isSended == false){
    radio.write(&nullSignal, sizeof(byte));
    Serial.println('N');
    isSended = false;
    }
}
isSended = false;
delay(10);
  /*
  Serial.print("Sent: "); Serial.println(counter);
  radio.write(&counter, sizeof(counter));
  counter++;
  delay(10);
*/
  
}
