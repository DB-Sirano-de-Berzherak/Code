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


unsigned long one  [8];
unsigned long two  [8];
float b, t, ax, ay, az, gx, gy, gz, la, lo; int i=-1, n, h, m, s;

void Time()
{
  one[1]=(long(gps.getHour()*100)+long(gps.getMinute()))*100+gps.getSecond();
}

void LaLo()
{
  if(gps.available()) {
    gps.readParsing();
    switch(gps.getState()) {
      case GPS_OK:
        one[3]=long(gps.getLatitudeBase10 ()*1000000);
        one[4]=long(gps.getLongitudeBase10()*1000000);
        break;
      case GPS_ERROR_DATA:
        one[3]=-9999999;
        one[4]=-9999999;
        break;
      case GPS_ERROR_SAT:
        one[3]=0;
        one[4]=0;
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

  two[1]=long((event.orientation.x+360)*100+0.5);
  two[2]=long((event.orientation.y+360)*100+0.5);
  two[3]=long((event.orientation.z+360)*100+0.5);
}


void AXYZ()
{
  two[4]=long((accel.readAX()+360)*100+0.5);
  two[5]=long((accel.readAY()+360)*100+0.5);
  two[6]=long((accel.readAZ()+360)*100+0.5);
}


void BAR()
{
  one[2]=long((barometer.readPressureMillibars()-150)*100+0.5)*10000+long((barometer.readTemperatureC()+30)*100+0.5);
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
  
  t=(float(one[2]-(one[2]/10000)*10000)/100)-30;
  b=(float(one[2])/1000000-t/10000)+150;

  gx=(float(two[1])/100-360);
  gy=(float(two[2])/100-360);
  gz=(float(two[3])/100-360);  
  
  ax=(float(two[4])/100-360);
  ay=(float(two[5])/100-360);
  az=(float(two[6])/100-360);  
  
  n=(one[0]-one[0]/10000*10000);

  h=one[1]/10000;
  m=one[1]/100-(h)*100;
  s=one[1]-(h)*10000-(m)*100;

  la=(float(one[3])/1000000);
  lo=(float(one[4])/1000000);

  Serial.print(n);
  Serial.print("\t");
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.print(s);
  Serial.print("\t");
  Serial.print(t);
  Serial.print("\t");
  Serial.print(b);
  Serial.print("\t");
  Serial.print(la, 6);
  Serial.print("\t");
  Serial.println(lo, 6);
  Serial.print(n);
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);  
  Serial.print("\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.println(az);
  Serial.println();
  //delay(100);
}
