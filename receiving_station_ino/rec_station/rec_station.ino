// скетч для вывода кватернионов в serial-порт
// для дальнейшего графического просмотра ориентации объекта
// в среде processing «MadgwickProcessingDraw9DOF.pde»
 
// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с модулями IMU
#include <TroykaIMU.h>
 
// множитель фильтра
#define BETA 0.22
 
// создаём объект для фильтра Madgwick
Madgwick filter;
 
// создаём объект для работы с акселерометром
Accelerometer accel;
// создаём объект для работы с гироскопом
Gyroscope gyro;
// создаём объект для работы с компасом
Compass compass;
// создаём объект для работы с барометром
Barometer barometer;
 
// переменные для данных с гироскопа, акселерометра и компаса
float gx, gy, gz, ax, ay, az, mx, my, mz;
 
// получаемые углы ориентации (Эйлера)
float yaw, pitch, roll;
//номер пакета
int number = 0;

// переменная для хранения частоты выборок фильтра
float fps = 100;
 
// калибровочные значения компаса
// полученные в калибровочной матрице из примера «compassCalibrateMatrixx»
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
 
void setup()
{
  // открываем последовательный порт
  Serial.begin(115200);
  // инициализация акселерометра
  accel.begin();
  // инициализация гироскопа
  gyro.begin();
  // инициализация компаса
  compass.begin();
  // инициализация барометра
  barometer.begin();
  // калибровка компаса
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias);
}

unsigned long Time;
 
void loop()
{
  // запоминаем текущее время
  unsigned long startMillis = millis();
  Time = millis();
  // считываем данные с акселерометра в единицах G
  accel.readGXYZ(&ax, &ay, &az);
  // считываем данные с гироскопа в радианах в секунду
  gyro.readRadPerSecXYZ(&gx, &gy, &gz);
  // считываем данные с компаса в Гауссах
  compass.readCalibrateGaussXYZ(&mx, &my, &mz);
  // устанавливаем коэффициенты фильтра
  filter.setKoeff(fps, BETA);
  // обновляем входные данные в фильтр
  filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
  number = number + 1;
  if (Serial.available() > 0) {
    int val = Serial.read();
    // если пришёл символ 's'
    if (val == 's') 
    {
          float q0, q1, q2, q3;
          filter.readQuaternions(&q0, &q1, &q2, &q3);
          // выводим кватернионы в serial-порт
          if(number % 2 == 1)
          {
              Serial.print(number);
              Serial.print(",");
              Serial.print(q0);
              Serial.print(",");
              Serial.print(q1);
              Serial.print(",");
              Serial.print(q2);
              Serial.print(",");
              Serial.print(q3);
              Serial.print(",");
              Serial.print(ax);
              Serial.print(",");
              Serial.print(ay);
              Serial.print(",");
              Serial.println(az);
          }
          else
          {
              Serial.print(number);
              Serial.print(",");
              Serial.print(Time);
              Serial.print(",");
              Serial.print(barometer.readPressureMillibars());
              Serial.print(",");
              Serial.println(barometer.readTemperatureC());
          }
          /*Serial.print(number);
          Serial.print(",");
          Serial.print(q0);
          Serial.print(",");
          Serial.print(q1);
          Serial.print(",");
          Serial.print(q2);
          Serial.print(",");
          Serial.print(q3);
          Serial.print(",");
          Serial.print(ax);
          Serial.print(",");
          Serial.print(ay);
          Serial.print(",");
          Serial.print(az);
          Serial.print(",");
          Serial.print(Time);
          Serial.print(",");
          Serial.print(barometer.readPressureMillibars());
          Serial.print(",");
          Serial.println(barometer.readTemperatureC());*/
    }
  }
  // вычисляем затраченное время на обработку данных
  unsigned long deltaMillis = millis() - startMillis;
  // вычисляем частоту обработки фильтра
  fps = 1000 / deltaMillis;
}
