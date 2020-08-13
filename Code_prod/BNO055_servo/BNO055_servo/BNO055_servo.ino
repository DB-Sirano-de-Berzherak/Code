#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <PWMServo.h>

PWMServo myservo1; 
PWMServo myservo2; 
PWMServo myservo3;
PWMServo myservo4;// create servo object to control a servo

const int potpin = A9;  // analog pin used to connect the potentiometer
int colibr_koeff[] = {0, -5, -7, -10};
int val[] = {90, 90, 90, 90}; 
float x, y, z;
float angle;
  
Adafruit_BNO055 bno = Adafruit_BNO055(55,0x29);

 
void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
  
  myservo1.attach(6);
  myservo2.attach(7); //к сожалению, 7 и 8 порт приходится задействовать под NRF24L01P, поэтому эти два порта в будущем придётся поправить на иные
  myservo3.attach(8);
  myservo4.attach(9);
  
  myservo1.write(90);
  myservo2.write(85); //калибровачное число -5
  myservo3.write(83); //калибровачное число -7
  myservo4.write(80); //калибровачное число -10
  delay(1000);
  
}
 
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

  /*val[0] = analogRead(potpin); 
  val[1] = analogRead(potpin);  
  val[2] = analogRead(potpin); 
  val[3] = analogRead(potpin);  */

 // val[1] = 
 // val[3] = 

  /*val[0] = map(val[0], 0, 1023, 75, 105); 
  val[1] = map(val[1], 0, 1023, 75, 105);  
  val[2] = map(val[2], 0, 1023, 75, 105); 
  val[3] = map(val[3], 0, 1023, 75, 105);  */
  
  // reads the value of the potentiometer (value between 0 and 1023)
  Serial.println(val[0]);
  Serial.println(val[1]);
  Serial.println(val[2]);
  Serial.println(val[3]);
  
  //val = 90; //map(val, 0, 1023, 75, 105);     // scale it to use it with the servo (value between 0 and 180)// sets the servo position according to the scaled value
  rotate(1);
  rotate(2);
  rotate(3);
  rotate(4);
    
  delay(15);  

  imu::Quaternion quat = bno.getQuat();

  /* Display the quat data */
  Serial.print("qW: ");
  Serial.print(quat.w(), 4);
  Serial.print(" qX: ");
  Serial.print(quat.y(), 4);
  Serial.print(" qY: ");
  Serial.print(quat.x(), 4);
  Serial.print(" qZ: ");
  Serial.print(quat.z(), 4);
  Serial.println("");

  quattoaxis(quat);
    
}

void rotate(int num)
{
    if(num == 1)
      myservo1.write(val[num - 1] + colibr_koeff[num - 1]);
    if(num == 2)
      myservo2.write(val[num - 1] + colibr_koeff[num - 1]);
    if(num == 3)
      myservo3.write(val[num - 1] + colibr_koeff[num - 1]);
    if(num == 4)
      myservo4.write(val[num - 1] + colibr_koeff[num - 1]);
}

void quattoaxis(imu::Quaternion quat)
{
   if (quat.w() > 1) 
    quat.normalize(); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
   angle = 2 * acos(quat.w());
   double s = sqrt(1-quat.w()*quat.w()); // assuming quaternion normalised then w is less than 1, so term always positive.
   if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
     // if s close to zero then direction of axis not important
     x = quat.x(); // if it is important that axis is normalised then replace with x=1; y=z=0;
     y = quat.y();
     z = quat.z();
   } else {
     x = quat.x() / s; // normalise axis
     y = quat.y() / s;
     z = quat.z() / s;
   }

   Serial.print("x: ");
   Serial.print(x);
   Serial.print(" y: ");
   Serial.print(y);
   Serial.print(" z: ");
   Serial.print(z);
   Serial.print(" angle: ");
   Serial.print( angle);
   Serial.println("");
}
