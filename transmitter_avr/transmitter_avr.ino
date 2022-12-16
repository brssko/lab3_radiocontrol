/*
 * Робота Бруско А.В.
 * ст. гр. РС-21мп
 * 
 * Дана програма призначена для передавальної частини системи.
 * Побудована на основі наступних компонентів:
 * Arduino Pro Mini (atmega328p@16MHz)
 * nRf24l01
 * Oled SSD1306 I2C display
 * 2 тактові кнопки
 * 
 * Підключення nRf24l01 та Arduino:
 * CE -> 9
 * CSN -> 10
 * SCK -> 13
 * MOSI -> 11
 * MISO -> 12
 * 
 * Підключення кнопок:
 * "+" -> 2
 * "-" -> 3
 * 
 * Підключення дисплею:
 * SDA -> A4
 * SCL -> A5
 */

#include <SPI.h>
#include <GyverOLED.h>
#include <nRF24L01.h>
#include <RF24.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;     //Створюємо об'єкт класу GyverOled
RF24 radio(9, 10);                                  //CE, CSN до nRF24l01
const uint64_t address = 0xF0F0F0F0E1LL;            //Канал, на якому буде здійснено передача
uint8_t angle = 0;                                  //Змінна для значення кута повороту

void setup() {
  pinMode(2, INPUT_PULLUP);                         //Конфігуруємо входи для кнопок
  pinMode(3, INPUT_PULLUP);
  oled.init();                                      //Ініціалізуємо дисплей
  show_angle();                                     //Виводимо інформацію на дисплей
  radio.begin();                                    //Ініціалізуємо радіомодуль
  radio.openWritingPipe(address);                   //Встановлюємо адресу передачі
  radio.setPALevel(RF24_PA_MIN);                    //Встановлюємо мінімальну потужність
  radio.stopListening();                            //Конфігуруємо модуль на передачу
}

void show_angle() {                                 //Виводимо зображення на дисплей
  oled.setScale(3);
  oled.setCursor(35, 2);
  if(angle<10){oled.print("00"); oled.print(angle);}
  else if(angle<100){oled.print("0"); oled.print(angle);}
  else{oled.print(angle);}
  oled.setScale(2);
  oled.print("deg");
}

void loop() {
  if (digitalRead(2) == 0) {                        //Перевіряємо стан кнопки і величину кута для збільшення
    if (angle < 180) {
      angle+=10;
    }
    const char txdata = (char)angle;
    radio.write(&txdata, sizeof(txdata));           //Відправляємо по радіо
    show_angle();                                   //Та відображаємо нове значення на дисплеї
    delay(30);
  }
  if (digitalRead(3) == 0) {                        //Перевіряємо стан кнопки і величину кута для зменшення
    if (angle > 9) {
      angle-=10;
    }
    const char txdata = (char)angle;
    radio.write(&txdata, sizeof(txdata));           //Відправляємо по радіо
    show_angle();                                   //Та відображаємо нове значення на дисплеї
    delay(30);
  }
}
