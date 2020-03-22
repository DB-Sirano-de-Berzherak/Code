import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;

float R = 6731000;
float pi = 3.14159265359;

PImage img;
Serial port;                         
String message;
 
float[] q = new float[4];
Quaternion quat = new Quaternion(1, 0, 0, 0);
int newLine = 13; 
String [] massQ = new String [4];
float[] ypr = new float[3];


float time = 0;
float tempreture = 0;
float pressure = 0;
float altitude = 0;
float roll = 0;
float pitch = 0;
float yaw = 0;
int PH = 1;
int COC = 0;
int CAC = 0;
float latitude = 56.40252332;
float longitude = 41.00058174; 
float x;
float y;
float x_map;
float y_map;

void setup() 
{
    size(1880, 1000, P3D);
    port = new Serial(this, "COM5", 115200);
    img = loadImage("gps_map.jpg");
    delay(1000);
    degreese_to_xy();
    println(x);
    println(y);
}

void draw()
{
    background(0);
    
    //GPS
    image(img, 990, 50, 400, 300);
    fill(255, 0, 0);
    //x_map = 
    //y_map =
    //ellipse(990, 50, 5, 5);
    //ellipse(1390, 350, 5, 5);
    
    fill(255);
    rect(1460, 230, 370, 120);
    fill(0);
    rect(1463, 233, 364, 114);
    
    fill(255);
    textSize(32);
    text("Широта: "+latitude, 1480, 280);
    text("Долгота: "+longitude, 1480, 325);
    
    
    //ИСО  
    fill(255);
    rect(990, 400, 840, 500);
    fill(0);
    rect(993, 403, 834, 494);
    
    
    //акселерометр
    //ось x
    fill(255);
    rect(500, 50, 430, 240);
    fill(0);
    rect(503, 53, 424, 234);
    fill(255);
    textSize(15);
    text("Ускорение по оси x от времени", 600, 310);
    
    //шкала
    fill(255);
    rect(542, 170, 360, 2);
    rect(543, 70, 2, 202);
    fill(100);
    rect(545, 70, 360, 2);
    rect(545, 95, 360, 2);
    rect(545, 120, 360, 2);
    rect(545, 145, 360, 2);
    rect(545, 195, 360, 2);
    rect(545, 220, 360, 2);
    rect(545, 245, 360, 2);
    rect(545, 270, 360, 2);
    
    fill(255);
    textSize(15);
    text("8", 525, 80);
    text("4", 525, 125);
    text("0", 525, 175);
    text("-4", 515, 225);
    text("-8", 515, 275);
    
    
    //ось y
    fill(255);
    rect(500, 345, 430, 240);
    fill(0);
    rect(503, 348, 424, 234);
    fill(255);
    textSize(15);
    text("Ускорение по оси y от времени", 600, 605);
    
    //шкала
    fill(255);
    rect(542, 465, 360, 2);
    rect(543, 365, 2, 202);
    fill(100);
    rect(545, 365, 360, 2);
    rect(545, 390, 360, 2);
    rect(545, 415, 360, 2);
    rect(545, 440, 360, 2);
    rect(545, 490, 360, 2);
    rect(545, 515, 360, 2);
    rect(545, 540, 360, 2);
    rect(545, 565, 360, 2);
    
    fill(255);
    textSize(15);
    text("8", 525, 375);
    text("4", 525, 420);
    text("0", 525, 470);
    text("-4", 515, 520);
    text("-8", 515, 570); 
    
    //ось z
    fill(255);
    rect(500, 640, 430, 240);
    fill(0);
    rect(503, 643, 424, 234);
    fill(255);
    textSize(15);
    text("Ускорение по оси z от времени", 600, 900);
    
    //шкала
    fill(255);
    rect(542, 760, 360, 2);
    rect(543, 660, 2, 202);
    fill(100);
    rect(545, 660, 360, 2);
    rect(545, 685, 360, 2);
    rect(545, 710, 360, 2);
    rect(545, 735, 360, 2);
    rect(545, 785, 360, 2);
    rect(545, 810, 360, 2);
    rect(545, 835, 360, 2);
    rect(545, 860, 360, 2);
    
    fill(255);
    textSize(15);
    text("8", 525, 670);
    text("4", 525, 715);
    text("0", 525, 765);
    text("-4", 515, 815);
    text("-8", 515, 865); 
    
    
    textSize(32);
    
    //Блок Время   
    fill(255);
    rect(50, 440, 400, 70);
    fill(0);
    rect(53, 443, 394, 64);
    
    fill(255);
    text("Время: "+time, 70, 485);
    
    
    //Блок температура, давление, высоты
    fill(255);
    rect(50, 740, 400, 160);
    fill(0);
    rect(53, 743, 394, 154);
    
    fill(255);
    text("Температура: "+tempreture, 70, 785);
    text("Давление: "+pressure, 70, 830);
    text("Высота: "+altitude, 70, 875);
    
    
    //Блок углы ориентации
    fill(255);
    rect(50, 545, 400, 160);
    fill(0);
    rect(53, 548, 394, 154);
    
    YawPitchRoll();
    fill(255);
    text("Тангаж: "+pitch, 70, 590);
    text("Крен: "+yaw, 70, 635);
    text("Рыскание: "+roll, 70, 680); 
    
    
    //Блок состояния аппарата
    fill(255);
    rect(1460, 50, 370, 155);
    fill(0);
    rect(1463, 53, 364, 149);
    
    fill(0, 255, 0);
    fill(255, 0, 0);
    if(PH == 1)
      fill(0, 255, 0);
    text("Аппарат в РН", 1480, 95);
    fill(255, 0, 0);
    if(COC == 1)
      fill(0, 255, 0);
    text("СОС работает", 1480, 135);
    fill(255, 0, 0);
    if(CAC == 1)
      fill(0, 255, 0);
    text("САС сработала", 1480, 175); 
    
    
    //НИСО
    fill(255);
    rect(50, 50, 400, 2);
    rect(50, 50, 2, 350);
    rect(50, 400, 400, 2);
    rect(450, 50, 2, 350);
   
    //Обработка углов ориентации
    serialEvent();
    translate(width / 7.5, height / 5);
    pushMatrix();
    float[] axis = quat.toAxisAngle();
    rotate(axis[0], axis[2], axis[3], axis[1]);
    drawCylinder();
    drawQuards();
    popMatrix();
    port.write('s');
}


void serialEvent() 
{ 
    message = port.readStringUntil(newLine);
    
    if (message != null) 
    {
      massQ = split(message, ",");
      q[0] = float(massQ[0]);
      q[1] = float(massQ[1]);
      q[2] = float(massQ[2]);
      q[3] = float(massQ[3]);
    }
    quat.set(q[0], q[1], q[2], q[3]);
 
}
 
void drawCylinder() 
{
    float topRadius = 66;
    float bottomRadius = 66;
    float tall = 200;
    int sides = 32;
    pushMatrix();
    translate(0, 0, -120);
    rotateX(PI/2);
    fill(0, 0, 255, 200);
   
    float angle = 0;
    float angleIncrement = TWO_PI / sides;
    beginShape(QUAD_STRIP);
    for (int i = 0; i < sides + 1; ++i) {
      vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
      vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
      angle += angleIncrement;
    }
    endShape();
 
    if (topRadius != 0) {
      angle = 0;
      beginShape(TRIANGLE_FAN);
   
      vertex(0, 0, 0);
      for (int i = 0; i < sides + 1; i++) {
        vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
        angle += angleIncrement;
      }
      endShape();
    }
   
    if (bottomRadius != 0) {
      angle = 0;
      beginShape(TRIANGLE_FAN);
   
      vertex(0, tall, 0);          
      for (int i = 0; i < sides + 1; i++) {
        vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
        angle += angleIncrement;
      }
     endShape();
    }
    popMatrix(); 
}
 
void drawBody() 
{
    fill(255, 0, 0, 200);
    box(10, 10, 200);
}
 
void drawTriangles() 
{
    fill(0, 255, 0, 200);
    beginShape(TRIANGLES);
    vertex(-100,  2, 30); vertex(0,  2, -80); vertex(100,  2, 30);
    vertex(-100, -2, 30); vertex(0, -2, -80); vertex(100, -2, 30);
    vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);
    vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);
    endShape();
}
 
void drawQuards() 
{
    beginShape(QUADS);
    vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
    vertex( 100, 2, 30); vertex( 100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
    vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(100, -2,  30); vertex(100, 2,  30);
    vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2, -30, 98); vertex(-2, -30, 98);
    vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
    vertex(-2, -30, 98); vertex(2, -30, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
    endShape();
}

void YawPitchRoll() 
{
    ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1)*57.2;
    ypr[1] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1)*57.2;
    ypr[2] = -atan2(2.0f * (q[0] * q[2] - q[1] * q[3]), 1.0f - 2.0f * (q[2] * q[2] + q[1] * q[1]))*57.2;
   
    pitch = ypr[1];
    yaw = ypr[0];
    roll = ypr[2];
}

void degreese_to_xy()
{
    x = longitude * pi / 180 * R;
    y = R * log(tan(pi / 4 + (latitude * pi / 180) / 2));
} 
