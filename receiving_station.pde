import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;
 
Serial port;                         
 
String message;
 
float[] q = new float[4];
Quaternion quat = new Quaternion(1, 0, 0, 0);
// new line character in ASCII
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

void setup() 
{
    size(1880, 1000, P3D);
    port = new Serial(this, "COM5", 115200);
    delay(1000);
}

void draw()
{
    background(0);
    //GPS
    fill(255);
    rect(50, 500, 400, 400); 
    fill(0);
    rect(53, 503, 394, 394);
    
    //ИСО
    fill(255);
    rect(990, 480, 880, 420);
    fill(0);
    rect(993, 483, 874, 414);
    
    //акселерометр
    //ось x
    fill(255);
    rect(500, 50, 430, 240);
    fill(0);
    rect(503, 53, 424, 234);
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
    textSize(10);
    text("8", 530, 75);
    text("6", 530, 100);
    text("4", 530, 125);
    text("2", 530, 150);
    text("0", 530, 175);
    text("-2", 525, 200);
    text("-4", 525, 225);
    text("-6", 525, 250);
    text("-8", 525, 275);
    
    
    //ось y
    fill(255);
    rect(500, 360, 430, 240);
    fill(0);
    rect(503, 363, 424, 234);
    
    //шкала
    fill(255);
    rect(542, 480, 360, 2);
    rect(543, 380, 2, 202);
    fill(100);
    rect(545, 380, 360, 2);
    rect(545, 405, 360, 2);
    rect(545, 430, 360, 2);
    rect(545, 455, 360, 2);
    rect(545, 505, 360, 2);
    rect(545, 530, 360, 2);
    rect(545, 555, 360, 2);
    rect(545, 580, 360, 2);
    
    fill(255);
    textSize(10);
    text("8", 530, 385);
    text("6", 530, 410);
    text("4", 530, 435);
    text("2", 530, 460);
    text("0", 530, 485);
    text("-2", 525, 510);
    text("-4", 525, 535);
    text("-6", 525, 560);
    text("-8", 525, 585);
    
    //ось z
    fill(255);
    rect(500, 660, 430, 240);
    fill(0);
    rect(503, 663, 424, 234);
    
    fill(255);
    rect(542, 780, 360, 2);
    rect(543, 680, 2, 202);
    fill(100);
    rect(545, 680, 360, 2);
    rect(545, 705, 360, 2);
    rect(545, 730, 360, 2);
    rect(545, 755, 360, 2);
    rect(545, 805, 360, 2);
    rect(545, 830, 360, 2);
    rect(545, 855, 360, 2);
    rect(545, 880, 360, 2);
    
    fill(255);
    textSize(10);
    text("8", 530, 685);
    text("6", 530, 705);
    text("4", 530, 735);
    text("2", 530, 760);
    text("0", 530, 785);
    text("-2", 525, 810);
    text("-4", 525, 835);
    text("-6", 525, 860);
    text("-8", 525, 885);
    
    
    textSize(32);
    
    //Блок Время
    fill(255);
    rect(990, 50, 370, 70);
    fill(0);
    rect(993, 53, 364, 64);
    
    fill(255);
    text("Время: "+time, 1010, 95);
    
    //Блок температура, давление, высот
    fill(255);
    rect(990, 250, 370, 160);
    fill(0);
    rect(993, 253, 364, 154);
    
    fill(255);
    text("Температура: "+tempreture, 1010, 295);
    text("Давление: "+pressure, 1010, 335);
    text("Высота: "+altitude, 1010, 375);
    
    //Блок углы ориентации
    fill(255);
    rect(1460, 250, 370, 160);
    fill(0);
    rect(1463, 253, 364, 154);
    
    fill(255);
    text("Тангаж: "+pitch, 1480, 295);
    text("Крен: "+yaw, 1480, 335);
    text("Рыскание: "+roll, 1480, 375);
    
    
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
    rect(50, 50, 2, 400);
    rect(50, 450, 400, 2);
    rect(450, 50, 2, 400);
   
    //Обработка углов ориентации
    serialEvent();
    translate(width / 7, height / 4);
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
