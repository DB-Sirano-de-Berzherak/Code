// библиотека для работы I²C
#include <Wire.h>
//#include <SD.h>
#include <SPI.h>
// библиотека для работы с модулями IMU
#include <TroykaIMU.h>
 
// создаём объект для работы с гироскопом
Gyroscope gyro;
// создаём объект для работы с акселерометром
Accelerometer accel;
// создаём объект для работы с компасом
Compass compass;
// создаём объект для работы с барометром
Barometer barometer;
 
// калибровочные значения компаса
// полученные в калибровочной матрице из примера «compassCalibrateMatrix»
const double compassCalibrationBias[3] = {
  524.21,
  3352.214,
  -1402.236
};
 
const double compassCalibrationMatrix[3][3] = {
  {1.757, 0.04, -0.028},
  {0.008, 1.767, -0.016},
  {-0.018, 0.077, 1.782}
};

//const int chipSelect = BUILTIN_SDCARD;
int led = 13;

 
void setup()
{
  // открываем последовательный порт
  Serial.begin(115200);
  // выводим сообщение о начале инициализации
  Serial.println("Begin init...");
  // инициализация гироскопа
  gyro.begin();
  // инициализация акселерометра
  accel.begin();
  // инициализация компаса
  compass.begin();
  // инициализация барометра
  barometer.begin();
  // калибровка компаса
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias);
  // выводим сообщение об удачной инициализации
  
  //Serial.println("Initialization completed");
  //Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  //if (!SD.begin(chipSelect)) {
  //  Serial.println("Card failed, or not present");
  //  // don't do anything more:
  //  return;
  //}
  //Serial.println("card initialized.");
  
  //pinMode(led, OUTPUT);
  
  //File dataFile = SD.open("datalog.txt", FILE_WRITE);
  //dataFile.println("===                     data                       ===");
  //dataFile.println("Gyroscope\t\t\tAccelerometer\t\t\tCompass\t\tBarometer");
  //dataFile.close();
  //delay(1000);
}
 
void loop()
{
  //File dataFile = SD.open("datalog.txt", FILE_WRITE);
   //if (dataFile) {
    digitalWrite(led, HIGH);
    Serial.print(millis());
    Serial.print("\t");
    // вывод угловой скорости в градусах в секунду относительно оси X
    Serial.print(gyro.readDegPerSecX());
    Serial.print("\t");
    // вывод угловой скорости в градусах в секунду относительно оси Y
    Serial.print(gyro.readDegPerSecY());
    Serial.print("\t");
    // вывод угловой скорости в градусах в секунду относительно оси Z
    Serial.print(gyro.readDegPerSecZ());
    Serial.print("\t\t");
    // вывод направления и величины ускорения в м/с² по оси X
    Serial.print(accel.readAX());
    Serial.print("\t");
    // вывод направления и величины ускорения в м/с² по оси Y
    Serial.print(accel.readAY());
    Serial.print("\t");
    // вывод направления и величины ускорения в м/с² по оси Z
    Serial.print(accel.readAZ());
    Serial.print("\t\t");
    // выводим азимут относительно оси Z
    Serial.print(compass.readGaussX());
    Serial.print("\t");
    Serial.print(compass.readGaussY());
    Serial.print("\t");
    Serial.print(compass.readGaussZ());
    Serial.print("\t");
    // вывод значения абсолютного давления
    Serial.print(barometer.readPressurePascals());
    Serial.print("\t");
    // вывод значения температуры окружающей среды
    Serial.print(barometer.readTemperatureC());
    Serial.print("\t");
    Serial.println("");
    delay(100);
    //Serial.close();
    digitalWrite(led, LOW);
   //}
   //else {
    //Serial.println("error opening datalog.txt");
    //} 
   delay(100);
}
