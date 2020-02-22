#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>  //BNO

#include <Wire.h>
#include <TroykaIMU.h>  //IMU

Adafruit_BNO055 bno = Adafruit_BNO055(55,0x29);   //BNO

Accelerometer accel;
Barometer barometer;  // IMU

char Ti[32]; 
char SXYZ[32];
char I[5];
int i=0;

void XYZ(int i)
{
  sensors_event_t event; 
  bno.getEvent(&event);
  
  char X[8], Y[8], Z[8];
  float x=event.orientation.x+360, y=event.orientation.y+360, z=event.orientation.z+360;
  int j=0;
  
  itoa(i, I, 10);
  dtostrf(x, 7, 3, X);
  dtostrf(y, 7, 3, Y);
  dtostrf(z, 7, 3, Z);

  if(i<1000){
    if(i<100){
      if(i<10){
        I[3]=I[0];
        I[2]='0';
        I[1]='0';
        I[0]='0';
      }
      else{
        I[3]=I[1];
        I[2]=I[0];
        I[1]='0';
        I[0]='0';
      }  
    }
    else{
      I[3]=I[2];
      I[2]=I[1];
      I[1]=I[0];
      I[0]='0';
    }
  }

  if(x<100){
    if(x<10){
      for(j=6; j>=2; j--)
        X[j]=X[j-2];
      X[1]='0';
      X[0]='0';
    }
    else{
      for(j=6; j>=1; j--)
        X[j]=X[j-1];
      X[0]='0'; 
    }
  }

  if(y<100){
    if(y<10){
      for(j=6; j>=2; j--)
        Y[j]=Y[j-2];
      Y[1]='0';
      Y[0]='0';
    }
    else{
      for(j=6; j>=1; j--)
        Y[j]=Y[j-1];
      Y[0]='0'; 
    }
  }

  if(z<100){
    if(z<10){
      for(j=6; j>=2; j--)
        Z[j]=Z[j-2];
      Z[1]='0';
      Z[0]='0';
    }
    else{
      for(j=6; j>=1; j--)
        Z[j]=Y[j-1];
      Z[0]='0'; 
    }
  }

  for(j=0; j<4; j++)
    SXYZ[j]=I[j];
  for(j=4; j<11; j++)
    SXYZ[j]=X[j-4];
  for(j=11; j<18; j++)
    SXYZ[j]=Y[j-11];
  for(j=18; j<25; j++)
    SXYZ[j]=Z[j-18];
}

void GXYZ()
{
  char X[5], Y[5], Z[5]; 
  int x=((accel.readAX()/9.816)+36)*100, y=((accel.readAY()/9.816)+36)*100, z=((accel.readAZ()/9.816)+36)*100;
  int j=0;
  
  itoa(x, X, 10);
  itoa(y, Y, 10);
  itoa(z, Z, 10);

  for(j=0; j<4; j++)
    SXYZ[j]=X[j];
  for(; j<8; j++)
    SXYZ[j]=Y[j-4];
  for(; j<12; j++)
    SXYZ[j]=Z[j-8];
}


void BAR()
{
  char B[6], T[5];
  int b=0, t=(barometer.readTemperatureC()+30)*100;
  double c=barometer.readPressureMillibars(), f;
  b=((c*100)/100)*100;
  f=(c-(c*100)/100);
  int y=0;

  Serial.println(c);
  
  itoa(b, B, 10);
  itoa(t, T, 10);

  for(; y<5; y++)
    Ti[y]=B[y];
  for(; y<9; y++)
    Ti[y]=T[y-5];
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Begin init...");
  accel.begin();
  barometer.begin();
  Serial.println("Initialization completed");
  Serial.println("Accelerometer\t\t\tBarometer");
}

void loop()
{
  GXYZ();
  BAR();
  Serial.println(SXYZ);
  Serial.println(Ti);
  //Serial.print((barometer.readPressureMillibars()-150)*100);
  //Serial.print("\t");
  //Serial.println((barometer.readTemperatureC()+30)*100);
  delay(100);
}
