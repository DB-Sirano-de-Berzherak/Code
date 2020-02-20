#include <Wire.h>
#include <TroykaIMU.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <TroykaGPS.h>
GPS gps(Serial1);


Accelerometer accel;
Barometer barometer;

Adafruit_BNO055 bno = Adafruit_BNO055(55,0x29);


float one  [8];
float two  [8];
int i=0;

void Time()
{
  //one[1]=float((gps.getHour()*100+gps.getMinute())*100+gps.getSecond());
}

void LaLo()
{
  if(gps.available()) {
    gps.readParsing();
    switch(gps.getState()) {
      case GPS_OK:
        one[4]=gps.getLatitudeBase10 ();
        one[5]=gps.getLongitudeBase10();
        break;
      case GPS_ERROR_DATA:
        one[4]=-9999999;
        one[5]=-9999999;
        break;
      case GPS_ERROR_SAT:
        one[4]=-1;
        one[5]=-1;
        break;
    }
  }
}

void NOMBER(int i)
{
  if(i>=10000)
    i=0;
    
  one  [0]=10000+i;
  two  [0]=20000+i;
}

void GXYZ()
{
  sensors_event_t event; 
  bno.getEvent(&event);

  two[1]=event.orientation.x;
  two[2]=event.orientation.y;
  two[3]=event.orientation.z;
}


void AXYZ()
{
  two[4]=accel.readAX();
  two[5]=accel.readAY();
  two[6]=accel.readAZ();
}


void BAR()
{
  one[2]=barometer.readPressureMillibars();
  one[3]=barometer.readTemperatureC();
}


void setup()
{
  Serial.begin(9600);

  one  [0] = 10000;
  two  [0] = 20000;
  
  Serial.println("Begin init...");
  accel.begin();
  barometer.begin();
  Serial.println("Initialization completed");

  Serial.println("Orientation Sensor Test"); //Serial.println("");
  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  while (!Serial) {}
  Serial.print("Serial init OK\r\n");
  Serial1.begin(9600); 
  
  delay(1000);
  bno.setExtCrystalUse(true);
}


void loop()
{
  i++;
  AXYZ();
  BAR();
  GXYZ();
  NOMBER(i);
  LaLo();
  Time();
  
  for(int y=0; y<=5; y++){
    Serial.print(one[y]); Serial.print("\t");}
  Serial.println();
  
  for(int y=0; y<=6; y++){
    Serial.print(two[y]); Serial.print("\t");}
  Serial.println();
  //delay(100);
}
