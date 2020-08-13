#include <TinyGPS.h>
TinyGPS gps; //tx od gps to serial2_rx of teensy 
bool newdata = false;
unsigned long start;
long lat, lon;
unsigned long time, date;

#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку для работы с nRF24L01+

RF24           radio(7, 8);                              // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
float            data[8]={0,0,0,0,0,0,0,0}; 
int i=0;                                   // Создаём массив для приёма данных


void setup()
{
  Serial2.begin(9600); // скорость обмена с GPS-приемником
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  

  SPI.begin();
  delay(1000);
  radio.begin();                                        // Инициируем работу nRF24L01+
  delay(1000);
     //radio.setAutoAck(0);
     //radio.setRetries(0,15);
     //radio.setCRCLength(RF24_CRC_8);
  radio.setChannel(0);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_2MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_MAX);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
  radio.stopListening();
  delay(1000);  
  Serial.println("Start");
  
}
void loop()
{
    data[0] = 10.1;                             // считываем показания Trema слайдера с вывода A1 и записываем их в 0 элемент массива data
    data[1] = 2.2;
    data[2] = 3.3;
    data[3] = 4.4;
    data[4] = 5.5;
    data[5] = 6.6;
    data[6] = 7.7;
    data[7] = 123.12;
  if(Serial2.available())
  {
    // задержка в секунду между обновлениями координат
    //if (millis() - start > 1000)
    //{
      newdata = readgps();
      if (newdata)
      {
        start = millis();
        gps.get_position(&lat, &lon);
        gps.get_datetime(&date, &time);
        Serial.print("Lat: "); Serial.print(lat);
        Serial.print(" Long: "); Serial.print(lon);
        Serial.print(" Date: "); Serial.print(date);
        Serial.print(" Time: "); Serial.println(time);
        float lat_float = lat;
        float lon_float = lon;
        data[0] = lat_float;
        data[1] = lon_float;
      }
    //}
  }
  radio.write(&data, sizeof(data));
  //Serial.println("TEST");
}
  // проверка наличия данных
bool readgps()
{
    while (Serial2.available())
    {
      int b = Serial2.read();
      //в TinyGPS есть ошибка: не обрабатываются данные с \r и \n
      if('\r' != b)
      {
        if (gps.encode(b))
        return true;
      }
    }
    return false;
}
