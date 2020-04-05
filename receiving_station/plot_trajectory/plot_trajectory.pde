import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;

//      |||||||||||||||||||||||||||||||||||||||||||||
//      ||                                         ||
//      ||     Чтобы вернуть график в началье      || 
//      ||     положение, нажмите на control       ||
//      ||                                         ||
//      |||||||||||||||||||||||||||||||||||||||||||||


//время
float time;
//массив времени
float[] arr_time = new float[360000];
//изменение времени
float delta_time;

//ускорение аппарата по 3 осям
float ax = 0;
float ay = 0;
float az = 0;

//массивы ускорений по 3 осям
float[] arr_ax = new float[360000];
float[] arr_ay = new float[360000];
float[] arr_az = new float[360000];

//скорость аппарата
float vx = 0;
float vy = 0;
float vz = 0;

//скорость предыдущая
float vx_prev = 0;
float vy_prev = 0;
float vz_prev = 0;

//расстояние, пройденное за малый промежуток времени
float sx = 0;
float sy = 0;
float sz = 0;

//массив координат аппарата
float[] arr_sx = new float[360000];
float[] arr_sy = new float[360000];
float[] arr_sz = new float[360000];


//счётчик для массивов координат
int arr_cnt = 0;
//максимальное значение счётчика
int cnt_max;
//начальное значение счётчика
int cnt_start;
//счётчик для построения графика траектории
int trajectory_cnt = 0;

//текущие координаты мышки
int x1, y1;
//предыдущие координаты мышку
int x2, y2;
//left-right (направление поворота вокруг оси y)
float rl;
//up-down (направление поворота вокруг осей x и z)
float ud;
//углы поворота вокруг осей
float angle_x = 0;
float angle_y = -45;
float angle_z = 0;


void setup() 
{
    //создать окно
    size(1880, 1000, P3D);
    //залить чёрным
    background(0);
    //счиать данные из файла
    parseFile();
    
    //заполнить массивы траектории 0
    int i;
    for(i = 0; i < 360000; i++)
    {
        arr_sx[i] = 0;
        arr_sy[i] = 0;
        arr_sz[i] = 0;
    }
}


void draw()
{
    //увеличить размер
    scale(1.2);
    //залить чёрным цветом
    background(0);
    //перезадать начало координат
    translate(width/2.3, height/2, 0);
    //повернуть на соответствующие углы вокруг осей
    rotateY(radians(angle_y));
    rotateX(radians(angle_x));
    rotateZ(radians(angle_z));
    //перезадать начало координат
    translate(120, 0, 120);
    //посчитать траекторию
    calculateTrajectory();
    //нарисовать траеторию
    drawTrajectory();
    //вернуть обратно начало координат
    translate(-120, 0, -120);
    //нарисовать оси
    axis();    
}

void parseFile() 
{
    //Чтение из файла
    BufferedReader reader = createReader("dataLaunch.txt");
    String line = null;
    try 
    {
        while ((line = reader.readLine()) != null) 
        {
            String[] pieces = split(line, ",");
            //если пришли все данные целые (8 значений) из нечётного пакета
            if(int(pieces[0]) % 2 == 1 && pieces.length == 8)
            {
                ax = float(pieces[5]) * 9.816;
                ay = float(pieces[6]) * 9.816;
                az = float(pieces[7]) * 9.816;
            }
            //если пришли хотя бы номер пакета и время из чётного пакета
            if(int(pieces[0]) % 2 == 0 && pieces.length >= 2)
            {
                time = float(pieces[1]) / 1000;
            }
            //добавить ускорения и время в массивы
            arr_ax[arr_cnt] = ax;
            arr_ay[arr_cnt] = ay;
            arr_az[arr_cnt] = az;
            arr_time[arr_cnt] = time;
            //если это первый пакет - задать номер начального пакета
            if(arr_cnt == 0)
                cnt_start = int(pieces[0]);
            //увеличить счётчик пакетов
            arr_cnt += 1;
            //обновить максимальное значение счётчика
            cnt_max = arr_cnt;
        }
        reader.close();
    } 
    catch (IOException e)
    {
        e.printStackTrace();
    }
} 

void calculateTrajectory()
{
    //взять ускорения из массивов
    ax = arr_ax[trajectory_cnt];
    ay = arr_az[trajectory_cnt];
    az = arr_ay[trajectory_cnt];
    
    //если счётчик траектории в нужном диапазоне (больше одного, меньше максимального номера пакета)
    if(trajectory_cnt >= 1 && trajectory_cnt < (cnt_max - cnt_start))
    {
        delta_time = arr_time[trajectory_cnt] - arr_time[trajectory_cnt - 1];
        vx = vx_prev + ax * delta_time;
        sx = arr_sx[trajectory_cnt - 1] * 10 + vx * delta_time + ax * delta_time * delta_time / 2;
        vx_prev = vx;
        arr_sx[trajectory_cnt] = sx / 10;
            
        vy = vy_prev + ay * delta_time;
        sy = arr_sy[trajectory_cnt - 1] * 10 - vy * delta_time - ay * delta_time * delta_time / 2;
        vy_prev = vy;
        arr_sy[trajectory_cnt] = sy / 10;
            
        vz = vz_prev + az * delta_time;
        sz = arr_sz[trajectory_cnt - 1] * 10 + vz * delta_time + az * delta_time * delta_time / 2;
        vz_prev = vz;
        arr_sz[trajectory_cnt] = sz / 10;
     }

    //увеличить счётчик тректории
    trajectory_cnt++;
}

void drawTrajectory()
{
    stroke(255, 0, 0);
    beginShape(LINES);
    //отрисовка траектории
    int i;
    for(i = 1; i < (cnt_max - cnt_start); i++)
    {
        vertex(arr_sx[i - 1], -arr_sy[i - 1], arr_sz[i - 1]); vertex(arr_sx[i], -arr_sy[i], arr_sz[i]);
    }
    endShape();
}


void axis()
{
    stroke(255);
    beginShape(LINES);    
    //сетка
    stroke(100);
    int i;
    
    for(i = 0; i > -260; i -= 20)
    {
        vertex(0, i, 0); vertex(240, i, 0);
    }
    for(i = 0; i < 260; i += 20)
    {
        vertex(i, 0, 0); vertex(i, -240, 0);
    }
   for(i = 0; i < 260; i += 20)
    {
        vertex(0, 0, i); vertex(240, 0, i);
    }
    for(i = 0; i < 260; i += 20)
    {
        vertex(i, 0, 0); vertex(i, 0, 240);
    } 
    for(i = 0; i < 260; i += 20)
    {
        vertex(0, 0, i); vertex(0, -240, i);
    }
    
    for(i = 0; i > -260; i -= 20)
    {
        vertex(0, i, 0); vertex(0, i, 240);
    }
    //оси 
    stroke(255);
    vertex(0, 0, 240); vertex(0, -240, 240);
    vertex(0, 0, 240); vertex(240, 0, 240);
    vertex(240, 0, 240); vertex(240, 0, 0);
   
    endShape();
  
    //подписи к осям
    fill(255);
    
    textSize(10);
    text("Z(м)", 115, 20, 250);
    textSize(8);
    text("-1200", -15, 7, 245);
    text("-600", 50, 7, 245);
    text("0", 120, 7, 245);
    text("600", 170, 7, 245);
    text("1200", 230, 7, 245);
    
    translate(240, 0, 240);
    rotateY(PI / 2);
    
    textSize(10);
    text("X(м)", 115, 20, 5);
    textSize(8);
    text("-1200", 225, 7, 5);
    text("-600", 170, 7, 5);
    text("0", 120, 7, 5);
    text("600", 52, 7, 5);
    text("1200", -5, 7, 5);
    
    rotateY(-PI / 2);
    translate(-240, 0, -240);
    
    translate(0, 0, 260);
    rotateY(-radians(angle_y));
    textSize(10);
    text("Y(м)", -55, -115, 0);
    textSize(8);
    text("2400", -30, -235, 0);
    text("1800", -30, -175, 0);
    text("1200", -30, -115, 0);
    text("600", -30, -55, 0);
    text("0", -30, 0, 0);
    translate(0, 0, -260);
}

void keyPressed() 
{
    //если нажать кнопка cntrl, график вернётся в начальное состоние
    if (key == CODED) 
    {
        if (keyCode == CONTROL) 
        {
            angle_x = 0;
            angle_y = -45;
            angle_z = 0;
        }
    }
}

void mouseDragged() 
{
    //поворот графика с помощбю мышки
    //узнать координаты мышки текущие
    x1 = mouseX;
    y1 = mouseY;
    //если мышка сдвинуласть вправо, повернуть на 1 градус вправо; 
    //если сдвинулась влево, повернуть на 1 градус влево
    if(x1 - x2 >= 0)
      rl = 1;
    else
      rl = -1;
    //обновить угол поворота
    angle_y += rl;
    
    //если угол поворота график в определённом диапазоне, двигать вокруг оси x
    {
        if(y1 - y2 >= 0)
            ud = -0.5;
        else
            ud = 0.5;
      
        angle_x += ud;
    }
    //если угол поворота график в определённом диапазоне, двигать вокруг оси z
    if(angle_y > -90 || angle_y < -180)
    {
        if(y1 - y2 >= 0)
            ud = 0.5;
        else
            ud = -0.5;
      
        angle_z += ud;
    }
    //обновить координаты мышки конечные
    x2 = x1;
    y2 = y1;
}
