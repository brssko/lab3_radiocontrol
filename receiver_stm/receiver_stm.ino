/*
 * Робота Бруско А.В.
 * ст. гр. РС-21мп
 * 
 * Дана програма призначена для приймальної частини системи.
 * Побудована на основі наступних компонентів:
 * stm32f10c6 Bluepill (копія)
 * nRf24l01
 * Micro servo 9g
 * 
 * Підключення nRf24l01 та STM32:
 * CE -> PB0
 * CSN -> PA4
 * SCK -> PA5
 * MOSI -> PA7
 * MISO -> PA6
 * 
 * Підключення серво:
 *  -> PA0
 */


#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo servoone;                             //Створюємо об'єкт класу Servo
RF24 radio(PB0, PA4);                       //CE, CSN до nRF24l01
const uint64_t address = 0xF0F0F0F0E1LL;    //Канал, на якому буде здійснено прийом
uint8_t angle = 0;                          //Змінна для значення кута повороту

void setup() {
  radio.begin();                            //Ініціалізуємо радіомодуль
  radio.openReadingPipe(0, address);        //Встановлюємо адресу прийому
  radio.setPALevel(RF24_PA_MIN);            //Встановлюємо мінімальну потужність
  radio.startListening();                   //Конфігуруємо модуль на прийом
  servoone.attach(PA0);                     //Конфігуруємо сервопривід
}

void loop() {
  if (radio.available()) {                  //Якщо у каналі з'явились дані, то приймаємо їх
    char rxdata;
    radio.read(&rxdata, sizeof(rxdata));    //Зчитуємо у змінну, та конвертуємо її для передачі до...
    angle = (byte)rxdata;
    servoone.write(angle);                  //...функції керування сервоприводом.
  }
  delay(5);
}
