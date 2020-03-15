import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;
 
Serial port;                         
 
String message;
float[] data = new float[8];
// new line character in ASCII
int newLine = 13; //(символ конца строки carriage return)
String [] massQ = new String [8];

void setup() {
    // List all the available serial ports:
    printArray(Serial.list());
    // Open the port you are using at the rate you want:
    port = new Serial(this, Serial.list()[0], 115200);
    delay(5000);

}

void draw() {
    //port.write('s');
    println("TEST");
    message = port.readString();
    print(message);
    if (message != null) 
    {
      print("DATA");
      massQ = split(message, ",");
      data[0] = float(massQ[0]);
      data[1] = float(massQ[1]);
      data[2] = float(massQ[2]);
      data[3] = float(massQ[3]);
      data[4] = float(massQ[4]);
      data[5] = float(massQ[5]);
      data[6] = float(massQ[6]);
      data[7] = float(massQ[7]);
      print(data[0]);
      print(",");
      print(data[1]);
      print(",");
      print(data[2]);
      print(",");
      print(data[3]);
      print(",");
      print(data[4]);
      print(",");
      print(data[5]);
      print(",");
      print(data[6]);
      print(",");
      println(data[7]);
    }
    
 
}
