#include <TroykaGPS.h>
GPS gps(Serial1);
char LL[32];
char Ti[32];
char I[5];
int i=0;

void Time(int i)
{
  char H[3], M[3], S[3];
  int h = gps.getHour(), m = gps.getMinute(), s = gps.getSecond();

  itoa(h, H, 10);
  itoa(m, M, 10);
  itoa(s, S, 10);
  itoa(i, I, 10);

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

  for(int y=0; y<4; y++)
    Ti[y]=I[y];

  if(h<10){
    Ti[4]='0';
    Ti[5]=H[0];}
  else{
    Ti[4]=H[0];
    Ti[5]=H[1];}  

  if(m<10){
    Ti[6]='0';
    Ti[7]=M[0];}
  else{
    Ti[6]=M[0];
    Ti[7]=M[1];}

  if(s<10){
    Ti[8]='0';
    Ti[9]=S[0];}
  else{
    Ti[8]=S[0];
    Ti[9]=S[1];}
}

void LaLo(int i)
{
  char Lo[10], La[10];
  int y=0;

  for(; y<4; y++)
    LL[y]=I[y];
    
  dtostrf(gps.getLatitudeBase10(), 9, 6, La);
  dtostrf(gps.getLongitudeBase10(), 9, 6, Lo);

  for(; y<13; y++)
     LL[y]=La[y-4];
  for(; y<22; y++)
     LL[y]=Lo[y-13];
}
     
void setup()
{ 
    Serial.begin(9600);
    while (!Serial) {}
    Serial.print("Serial init OK\r\n");
    Serial1.begin(9600); 
}
     
void loop()
{
      if (gps.available()) {
        gps.readParsing();
        switch(gps.getState()) {
          // всё OK
          case GPS_OK:
            Time(i);
            Serial.println(Ti);
            LaLo(i);          
            Serial.println(LL);  
            i++;
            if(i==10000)
              i=0;
            break;
          case GPS_ERROR_DATA:
            Serial.println("GPS error data");
            break;
          case GPS_ERROR_SAT:
            Serial.println("GPS no connect to satellites!!!");
            break;
    }
  }
}
