#include <Wire.h>
#include <TroykaIMU.h>

Accelerometer accel;
Barometer barometer;

char SXYZ[32];
char Ti[32];

void GXYZ()
{
  char X[7], Y[7], Z[7]; 
  float x=accel.readAX()+360, y=accel.readAY()+360, z=accel.readAZ()+360;
  int j=0;
  
  dtostrf(x, 6, 2, X);
  dtostrf(y, 6, 2, Y);
  dtostrf(z, 6, 2, Z);

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

  for(; j<6; j++)
    SXYZ[j]=X[j];
  for(; j<12; j++)
    SXYZ[j]=Y[j-6];
  for(; j<18; j++)
    SXYZ[j]=Z[j-12];
}


void BAR()
{
  char B[7], T[6];
  float b=barometer.readPressureMillibars()-150, t=barometer.readTemperatureC()+40;
  int y=0;
  
  dtostrf(b, 6, 2, B);
  dtostrf(t, 5, 2, T);

  for(; y<6; y++)
    Ti[y]=B[y];
  for(; y<11; y++)
    Ti[y]=T[y-6];
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
  delay(100);
}
