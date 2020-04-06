import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;

//     |||||||||||||||||||||||||||||||||||||||||
//     ||                                     ||
//     ||     Чась получаемых данных          ||
//     ||     закомментирована. Посмотрите    ||
//     ||     функцию serialEvent()           ||
//     ||                                     ||
//     |||||||||||||||||||||||||||||||||||||||||

void setup() 
{
    //создаём окно
    size(1880, 1000, P3D);
    //порт
    port = new Serial(this, "COM5", 115200);
    delay(1000);
    //подгружаем картинку карты
    img = loadImage("gps_map.jpg");
    
    dataFile = createWriter("dataLaunch.txt");
    
    //заполнить массивы для карты начальной координатой
    int i;
    for(i = 0; i < 360000; i++)
    {
        arr_x_map[i] = 990;           ///КООРДИНАТЫ МЕСТА ЗАПУСКА УКАЗАТЬ
        arr_y_map[i] = 50;            ///КООРДИНАТЫ МЕСТА ЗАПУСКА УКАЗАТЬ
    }
    
    //заполнить маассивы координат траектории 0
    for(i = 0; i < 360000; i++)
    {
        arr_sx[i] = 0;
        arr_sy[i] = 0;
        arr_sz[i] = 0;
    }
    delay(1000);

}

void draw()
{
    if(finish == 1)
    {
        //рисовать окно окончания программы
        Finish();
    }
    else
    {
        //рисовать обработку данных
        draw_process();
    }
}


void draw_process()
{
    //заливка окна - чёрная
    background(0);
    
    //обновление изменения времени
    time_prev = time;
    //получаем данные
    port.write('s');
    serialEvent();
    
    //выводящиеся на экран данные переводим в формат с двумя знаками после запятой
    format_2digit();
      
    //нарисовать карту, сетку; вывести значения широты и долготы
    draw_GPS();
    degreese_to_xy();
    xy_to_map();

    //рисуем раму для ИСО (до переназначения координат для удобства)
    draw_ISO();
    
    //рисуем оси к графику ускорения по осям x, y, z;
    drawAxis_ax();
    drawAxis_ay();
    drawAxis_az();
    //отображаем значения ускорения по осям x, y, z;
    draw_ax();
    draw_ay();
    draw_az();
    
    //выводим значение времени
    //выводим значения температуры, давления, высоты
    data();
    //выводим значения углов ориентации
    angles();
    //выводим бинарные значения состояния аппарата (РН, СОС, САС)
    condition();
    
    //отрисовываем положение аппарата в НИСО
    NISO();
    
    //рисуем рамку для ИСО
    draw_ISO();
    
    //поворачиваем весь график на некоторый угол вокруг осей y и z
    //уголы управляются стрелками с клавиатуры
    translate(1335, 720, 0);
    rotateY(radians(angle_y));
    
    //начало координат посредине графика
    translate(120, 0, 120);
    //уменьшить рамер графика (потому что продолбалась с начальным размером)
    scale(0.95);
    takeOff();
    //если аппарат взлетел, начать считать, отрисовывать ускорение
    if(takeoff == 1)
    {
        //посчитать следующие координаты
        calculateTrajectory();
        //отрисовать траекторию
        drawTrajectory();
        //переместить начало координат в начало графика
        translate(-120, 0, -120);
    }
    //нарисовать оси
    axis();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                              //ПЕРЕМЕННЫЕ
//радиус Земли
float R = 6731000;

//изображение карты
PImage img;
//порт
Serial port;  
//файл с данными
PrintWriter dataFile;

//пока 0 - не завершать обработку данных (меняется нажатием alt)
int finish = 0;

//массив квантарнионов
float[] q = new float[4];
Quaternion quat = new Quaternion(1, 0, 0, 0);
//значение конца строки (В ASCII)
int newLine = 13; 
//массив принятых данных
String [] mass = new String [9];
//сообщение с данными
String message;
//массив углов ориентации
float[] ypr = new float[3];
//массив углов ускорений по 3 осям

//номер получаемого пакета
float pack_number = 1;
//номер первго пришедшего пакета в момент начала обработки данных
float start_pack = 1;
//счётчик пришедших пакетов
int pack_cnt = 0;
//качество связи
float connection;
//время
float time;
//предыдущее отправленное время
float time_prev = 0;
//изменение времени
float delta_time = 0;
//температура
float tempreture = 22.45;
//давление предыдущее (для изменения давления)
float pressure2 = 979.86;
//давление текущее
float pressure = 979.86;
//высота
float altitude = 0;
//изменение высоты
float delta_altitude;

//углы ориентации
float roll = 0;
float pitch = 0;
float yaw = 0;


//углы вращения графика траектории
float angle_y = -70;

//ускорение аппарата по 3 осям
float[] a = new float[3];
float ax = 0;
float ay = 0;
float az = 0;
int takeoff = 0;

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
int trajectory_cnt = 0;


//бинарное состояние аппарата
int PH = 1;
int COC = 0;
int CAC = 0;

//широта, долгота
float latitude = 56.40845021;
float longitude = 40.98048019; 
//массив положений на карте
float arr_x_map[] = new float[360000];
float arr_y_map[] = new float[360000];
int path_cnt = 0;
//прямоугольные координаты
float x;
float y;
//координаты на карте
float x_map;
float y_map;

//переменные для отрисовки ускорения по оси x
int ax_x = 542;
int ax_y = 170;
int ax_xx [] = new int[360];
int ax_yy [] = new int[360];
int cnt_ax = 0;
int time_cnt_ax = 0;

//переменные для отрисовки ускорения по оси y
int ay_x = 542;
int ay_y = 465;
int ay_xx [] = new int[360];
int ay_yy [] = new int[360];
int cnt_ay = 0;
int time_cnt_ay = 0;

//переменные для отрисовки ускорения по оси z
int az_x = 542;
int az_y = 760;
int az_xx [] = new int[360];
int az_yy [] = new int[360];
int cnt_az = 0;
int time_cnt_az = 0;

//массив времени(для подписи осей к графикам ускорения)
String[] arr_time = new String[5];

//переменные, выводящиеся на экран, с точностью до 2 знаков после запятой
String char_time;
String char_roll;
String char_pitch;
String char_yaw;
String char_altitude;
String char_temperture;
String char_pressure;
String char_latitude;
String char_longitude;
String char_connection;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  //ПОЛУЧЕНИЕ ДАННЫХ ИЗ COM ПОРТА
void serialEvent() 
{ 
    //счиать сообщение
    message = port.readStringUntil(newLine);
    //если сообщение не пустое
    if (message != null) 
    {
      //переделать строку(сообщение) в массив данных
      mass = split(message, ",");
      //запись данных в файл
      dataFile.print(message);                                                                  ///Записывать ВСЕ данные в файл
      //номер пришедшего пакета
      pack_number = float(mass[0]);
      //задать номер "стартового пакета"
      if(pack_cnt == 1)
      {
        start_pack = pack_number;
      }
      if(mass.length == 4 || mass.length == 8)
      {
          //если номер пакета нечётный, считать квантернионы и ускорение
          if(pack_number % 2 == 1)
          {
              q[0] = float(mass[1]);
              q[1] = float(mass[2]);
              q[2] = float(mass[3]);
              q[3] = float(mass[4]);
              a[0] = float(mass[4]);
              a[1] = float(mass[6]);
              a[2] = float(mass[7]);
          }
          //если номер пакета чётный, считать остальные данные
          if(pack_number % 2 == 0)
          {
              time = float(mass[1]) / 1000;
              pressure = float(mass[2]);
              tempreture = float(mass[3]);
              //PH = int(mass[4]);
              //COC = int(mass[4]);
              //CAC = int(mass[4]);
              //latitude = float(mass[4]);
              //longitude = float(mass[4]);
          }
          //обновить счётчик пришедших пакетов
          pack_cnt += 1;
      }
    }
    quat.set(q[0], q[1], q[2], q[3]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //ФОРМАТИРОВАНИЕ ДАННЫХ
//(два знака после запятой + разделяющий знак - запятая
void format_2digit()
{
    stroke(0);
    //пересчитать полученные значения с точностью до 2 знаков после запятой
    char_time = String.format("%.2f", time);
    char_yaw = String.format("%.2f", yaw);
    char_pitch = String.format("%.2f", pitch);
    char_roll = String.format("%.2f", roll);
    char_altitude = String.format("%.2f", altitude);
    char_temperture = String.format("%.2f", tempreture);
    //давление перевести из милиБаров в Паскали
    char_pressure = String.format("%.2f", pressure * 100);
    char_connection = String.format("%.2f", connection);
    //У широты и долготы знаки не отбрасываются. Нужно для того, что бы данные выводились с запятой (а не точкой)
    char_longitude = String.format("%.5f", longitude);
    char_latitude = String.format("%.6f", latitude);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //РЕЖИМ ПОСЛЕ ОБРАБОТКИ ДАННЫХ
void Finish()
{
    dataFile.flush(); 
    dataFile.close();
    //режим окна после сохранения данных
    background(0);
    fill(255);
    textSize(64);
    text("Обработка данных завершена.", 480, 400);
    text("Ваши данные сохранены.", 570, 500);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //РАСЧЁТ И ОТОБРАЖЕНИЕ ВРЕМЕНИ, ДАВЛЕНИЯ, ВЫСОТЫ, БИНАРНЫХ СОСТОЯНИЙ АППАРАТА                                        ///сохранение данных в файл, качество связи
void data()
{
    //нарисовать рамку для тмпературы, давления, высоты
    fill(255);
    rect(50, 650, 400, 250);
    fill(0);
    rect(53, 653, 394, 244);
    //вычислить высоту
    calculate_altitude();
    //посчитать качество связи
    connection = pack_cnt / (pack_number - start_pack);
    //вывести значения температуры, давления, высоты, качества связи
    fill(255);
    //println(char_time);
    text("Время: "+char_time, 70, 695);
    text("Качество связи: "+char_connection, 70, 740);
    text("Температура: "+char_temperture, 70, 785);
    text("Давление: "+char_pressure, 70, 830);
    text("Высота: "+char_altitude, 70, 875);
    //обновить предыдущее значение давления
    pressure2 = pressure;
}

void angles()
{
    //нарисовать рамку для значения углов ориентации
    fill(255);
    rect(50, 455, 400, 160);
    fill(0);
    rect(53, 458, 394, 154);
    //посчитать углы ориентации
    YawPitchRoll();
    //вывести углы ориентации
    fill(255);
    text("Тангаж: "+char_pitch, 70, 500);
    text("Крен: "+char_yaw, 70, 545);
    text("Рыскание: "+char_roll, 70, 590); 
}

void calculate_altitude()
{
    //посчитать изменение высоты
    delta_altitude = 18400 * (1 + 0.003665 * tempreture) * log(pressure / pressure2);
    //прибавить зменение высоты к предыдущему значению
    altitude = altitude + delta_altitude;
}

void condition()
{
    //нарисовать рамку для состояний аппарата
    fill(255);
    rect(1460, 50, 370, 155);
    fill(0);
    rect(1463, 53, 364, 149);
    
    //выбрать красный цвет
    fill(255, 0, 0);
    //если аппарат в РН, выбрать зелёный
    if(PH == 1)
      fill(0, 255, 0);
    //написать текст выбранным цветом
    text("Аппарат в РН", 1480, 95);
    //выбрать красный цвет
    fill(255, 0, 0);
    //если система ориентации и стабилизации работает, выбрать зелёный цвет
    if(COC == 1)
      fill(0, 255, 0);
    //написать текст выбранным цветом
    text("СОС работает", 1480, 135);
    //выбрать красный цвет
    fill(255, 0, 0);
    //если система аварийного спасения сработала, выбрать зелёный
    if(CAC == 1)
      fill(0, 255, 0);
    //написать тект выбранным цветом
    text("САС сработала", 1480, 175); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                      //БЛОК ФУНКЦИЙ К АКСЕЛЕРОМЕТРУ
void draw_ax()
{
    stroke(255, 0, 0);
    fill(255, 0, 0); 
    //пересчёт ускорения в точку на графику
    ax_y = 170 - int(a[0] * 100 / 8);
    //сдвинуться по оси времени на 1 еденицу
    ax_x ++;
    //если координата по оси x (графика) кратна 72 вывести время
    //(72 что бы вывести 4 раз время) 
    if((542 - ax_x) % 72 == 0)
    {
        //в общий массив времени положить значение времени
        arr_time[time_cnt_ax] = char_time;
        //счётчик массива времени увеличить на 1
        time_cnt_ax ++;
    }
    //записать координаты в массив предыдущих значений
    ax_xx[cnt_ax] = ax_x;
    ax_yy[cnt_ax] = ax_y;
    //отрисовать предыдущие значения + только что записанное
    draw_ax_previous();
    //счётчик массива предыдущих значений
    cnt_ax ++;
    //когда график доходит до конца оси времени переместиться в начало оси, обнулить массив предыдущих значения и все счётчики
    if(ax_x >= 900)
    {
        ax_x = 542;
        cnt_ax = 0;
        int i;
        for(i = 0; i < 360; i++)
        {
            ax_xx[i] = 542;
            ax_yy[i] = 170;
        }
        time_cnt_ax = 0;
    }
  
}

void draw_ax_previous()
{
    //отрисовка предыдущих значений ускрения (стёрых background(0) в начале)
    int j;
    for(j = 0; j < cnt_ax - 1; j++)
    {
        line(ax_xx[j], ax_yy[j], ax_xx[j+1], ax_yy[j+1]);
    }
    //отрисовка предыдущих значений по времени (стёрых background(0) в начале)
    int g;
    for(g = 0; g < time_cnt_ax; g++)
    {
        fill(255);
        textSize(10);
        text(arr_time[g], 542 + 72 * (g + 1) - 8, 280);
    }
    
}

void draw_ay()
{
    stroke(255, 0, 0);
    fill(255, 0, 0);
    //пересчёт ускорения в точку на графику
    ay_y = 465 - int(a[1] * 100 / 8);
    //сдвинуться по оси времени на 1 еденицу
    ay_x ++;
    //если координата по оси x (графика) кратна 72 вывести время
    //(72 что бы вывести 4 раз время) 
    if((542 - ay_x) % 72 == 0)
        //счётчик массива времени увеличить на 1
        time_cnt_ay ++;
    //записать координаты в массив предыдущих значений
    ay_xx[cnt_ay] = ay_x;
    ay_yy[cnt_ay] = ay_y;
    //отрисовать предыдущие значения + только что записанное
    draw_ay_previous();
    //счётчик массива предыдущих значений увеличить на 1
    cnt_ay ++;
    //когда график доходит до конца оси времени переместиться в начало оси, обнулить массив предыдущих значения и все счётчики
    if(ay_x >= 900)
    {
        ay_x = 542;
        cnt_ay = 0;
        int i;
        for(i = 0; i < 360; i++)
        {
            ay_xx[i] = 542;
            ay_yy[i] = 465;
        }
        time_cnt_ay = 0;
    }
}

void draw_ay_previous()
{
    //отрисовка предыдущих значений ускрения (стёрых background(0) в начале)
    int j;
    for(j = 0; j < cnt_ay - 1; j++)
    {
        line(ay_xx[j], ay_yy[j], ay_xx[j+1], ay_yy[j+1]);
    }
    //отрисовка предыдущих значений по времени (стёрых background(0) в начале)
    int g;
    for(g = 0; g < time_cnt_ay; g++)
    {
        fill(255);
        textSize(10);
        text(arr_time[g], 542 + 72 * (g + 1) - 8, 575);
    }
}

void draw_az()
{
    stroke(255, 0, 0);
    fill(255, 0, 0);
    //пересчёт ускорения в точку на графику
    az_y = 760 - int(a[2] * 100 / 8);
    //сдвинуться по оси времени на 1 еденицу
    az_x ++;
    //если координата по оси x (графика) кратна 72 вывести время
    //(72 что бы вывести 4 раз время) 
    if((542 - az_x) % 72 == 0)
        //счётчик массива времени увеличить на 1
        time_cnt_az ++; 
    //записать координаты в массив предыдущих значений
    az_xx[cnt_az] = az_x;
    az_yy[cnt_az] = az_y;
    //отрисовать предыдущие значения + только что записанное
    draw_az_previous();
    //счётчик массива предыдущих значений увеличить на 1
    cnt_az ++;
    //когда график доходит до конца оси времени переместиться в начало оси, обнулить массив предыдущих значения и все счётчики
    if(az_x >= 900)
    {
        az_x = 542;
        cnt_az = 0;
        int i;
        for(i = 0; i < 360; i++)
        {
            az_xx[i] = 542;
            az_yy[i] = 465;
        }
        time_cnt_az = 0;
    }
    
    
    textSize(32);
    stroke(0);
}

void draw_az_previous()
{
    //отрисовка предыдущих значений ускрения (стёрых background(0) в начале)
    int j;
    for(j = 0; j < cnt_az - 1; j++)
    {
        line(az_xx[j], az_yy[j], az_xx[j+1], az_yy[j+1]);
    }
    //отрисовка предыдущих значений по времени (стёрых background(0) в начале)
    int g;
    for(g = 0; g < time_cnt_az; g++)
    {
        fill(255);
        textSize(10);
        text(arr_time[g], 542 + 72 * (g + 1) - 8, 870);
    }
}


//отрисовка осей

void drawAxis_ax()
{
    fill(255);
    rect(500, 50, 430, 240);
    fill(0);
    rect(502, 52, 426, 236); 
    fill(255);
    textSize(15);
    text("Ускорение по оси x от времени", 600, 310);
    
    //шкала
    stroke(255);
    line(542, 170, 900, 170);
    line(542, 70, 542, 270);
    stroke(100);
    int i;
    for(i = 70; i <= 270; i += 25)
    {
        line(542, i, 900, i);
    }
    for(i = 614; i <= 902; i += 72)
    {
        line(i, 70, i, 270);
    }
    
    fill(255);
    textSize(15);
    text("8", 525, 80);
    text("4", 525, 125);
    text("0", 525, 175);
    text("-4", 515, 225);
    text("-8", 515, 275);
    textSize(12);
    text("ax,g", 545, 65);
    text("t,c", 907, 173);
}

void drawAxis_ay()
{
    stroke(0);
    fill(255);
    rect(500, 345, 430, 240);
    fill(0);
    rect(503, 348, 424, 234);
    fill(255);
    textSize(15);
    text("Ускорение по оси y от времени", 600, 605);
    
    //шкала  
    stroke(255);
    line(542, 465, 900, 465);
    line(542, 363, 542, 567);
    stroke(100);
    int i;
    for(i = 365; i <= 565; i += 25)
    {
        line(542, i, 900, i);
    }
    for(i = 614; i <= 902; i += 72)
    {
        line(i, 365, i, 565);
    }

    fill(255);
    textSize(15);
    text("8", 525, 375);
    text("4", 525, 420);
    text("0", 525, 470);
    text("-4", 515, 520);
    text("-8", 515, 570); 
    textSize(12);
    text("ay,g", 545, 358);
    text("t,c", 907, 468);
}


void drawAxis_az()
{
    stroke(0);
    fill(255);
    rect(500, 640, 430, 240);
    fill(0);
    rect(503, 643, 424, 234);
    fill(255);
    textSize(15);
    text("Ускорение по оси z от времени", 600, 900);
    
    //шкала
    stroke(255);
    line(542, 760, 900, 760);
    line(542, 660, 542, 860);
    stroke(100);
    int i;
    for(i = 660; i <= 860; i += 25)
    {
        line(542, i, 900, i);
    }
    for(i = 614; i <= 902; i += 72)
    {
        line(i, 660, i, 860);
    }
    
    fill(255);
    textSize(15);
    text("8", 525, 670);
    text("4", 525, 715);
    text("0", 525, 765);
    text("-4", 515, 815);
    text("-8", 515, 865); 
    textSize(12);
    text("az,g", 545, 655);
    text("t,c", 907, 763);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
                                //БЛОК ФУНКЦИЙ К GPS
void draw_GPS()
{
    //отобразиить картинку карты
    image(img, 990, 50, 400, 300);
    //нарисовать сетку на керте
    stroke(50);
    int i;
    for(i = 1030; i <=1350; i += 40)
    {
        line(i, 50, i, 350);
    }
    
    for(i = 50; i <=320; i += 30)
    {
        line(990, i, 1390, i);
    }
    //нарисовать раму для долготы и широты
    fill(255);
    rect(1460, 230, 370, 120);
    fill(0);
    rect(1463, 233, 364, 114);
    //вывести долготу и широту
    fill(255);
    textSize(32);
    text("Широта: "+char_latitude, 1480, 280);
    text("Долгота: "+char_longitude, 1480, 325);
  
}

void degreese_to_xy()
{
    //перевод долготы и широты в плоские координаты
    //*см пост на habr "Занимательная геодезия"
    //для точного определения широты и долготы 
    //использовался сайт https://www.mapcoordinates.net/ru
    
    x = radians(longitude) * R;
    y = log(tan(PI / 4 + radians(latitude) / 2)) * R;
}

void xy_to_map()
{
    //задать соотношение координат и их положения на карте
    x_map = 1390 - 400 * (4816799.5 - x) / 3708;
    y_map = 50 + 300 * (8064540.0 - y) / 2883;
    //добавить новые координаты в массив положений на карте
    arr_x_map[path_cnt] = x_map;
    arr_y_map[path_cnt] = y_map;
    //увеличить счётчик
    path_cnt += 1;
}

void drawPath()
{
    ///ПРОВЕРИТЬ РАБОТУ GPS
    //если счётчик >= 10800 (данные gps отправляются 1 раз/с, ищем дольше 3 часов, рисовать эллипсв точке, где аппарат находится)
    if(path_cnt >= 10800)
    {
        fill(255, 0, 0);
        ellipse(arr_x_map[10799], arr_y_map[10799], 5, 5);
    }
    //рисовать на карте путь по gps
    int j;
    for(j = 0; j < path_cnt - 1; j++)
    {
        stroke(255, 0, 0);
        line(arr_x_map[j], arr_y_map[j], arr_x_map[j + 1], arr_y_map[j + 1]);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                    //БЛОК ФУНККЦИЙ НИСО
void cylinder(int numSegments, float h, float r)
{
    float angle = 360.0 / (float)numSegments;
    // top 
    beginShape();
    for (int i=0; i <numSegments; i++) 
    { 
        float x = cos(radians(angle * i)) * r; 
        float y = sin(radians(angle * i)) * r; 
        vertex(x, y, -h/2);
    }
    endShape( CLOSE );
    // side
    beginShape( QUAD_STRIP );
    for ( int i = 0; i < numSegments + 1; i++ ) 
    { 
        float x = cos(radians(angle * i)) * r; 
        float y = sin(radians(angle * i)) * r; 
        vertex(x, y, -h/2);
        vertex(x, y, h/2);
    }
    endShape();
    // bottom 
    beginShape();
    for (int i=0; i<numSegments; i++) 
    { 
      float x = cos(radians(angle * i)) * r; 
      float y = sin(radians(angle * i)) * r; 
      vertex(x, y, h/2);
    }
    endShape( CLOSE );
}

void YawPitchRoll() 
{
    //перевод квантарнионов в углы ориентации
    ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1)*57.2;
    ypr[1] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1)*57.2;
    ypr[2] = -atan2(2.0f * (q[0] * q[2] - q[1] * q[3]), 1.0f - 2.0f * (q[2] * q[2] + q[1] * q[1]))*57.2;
   
    pitch = ypr[1];
    yaw = ypr[0];
    roll = ypr[2];
}

void NISO()
{
    //отрисовка рамы для блока НИСО
    stroke(255);
    line(50, 50, 450, 50);
    line(50, 50, 50, 400);
    line(50, 400, 450, 400);
    line(450, 50, 450, 400);
    
    //Отрисовка модели в НИСО
    translate(width / 7.5, height / 4.5);
    pushMatrix();
    float[] axis = quat.toAxisAngle();
    rotate(axis[0], axis[2], axis[3], axis[1]);
    stroke(0);
    fill(0, 0, 255);
    cylinder(10, 160, 42);
    popMatrix();
    port.write('s');
    translate(-width / 7.5, -height / 4.5);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                    //БЛОК ФУНКЦИЙ ИСО
void takeOff()   
{
    if(abs(a[2]) >= 1.5)
      takeoff = 1;
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

void calculateTrajectory()
{
    //преревод ускорения из g в м/с^2
    ax = a[0] * 9.816;
    ay = a[2] * 9.816;
    az = a[1] * 9.816;

    //посчитать изменение времени
    delta_time = time - time_prev;
    //если это не первый расчёт, взять значение предыдущих координат. Иначе взять 0
    //*считаем, что не маленьком промежутке времени движение равноускорено, используем соответствующие формулы
    if(trajectory_cnt != 0)
    {
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
     else
     {
        vx = vx_prev + ax * delta_time;
        sx = vx * delta_time + ax * delta_time * delta_time / 2;
        vx_prev = vx;
        arr_sx[trajectory_cnt] = sx / 10;
            
        vy = vy_prev + ay * delta_time;
        sy = -vy * delta_time - ay * delta_time * delta_time / 2;
        vy_prev = vy;
        arr_sy[trajectory_cnt] = sy / 10;
            
        vz = vz_prev + az * delta_time;
        sz = vz * delta_time + az * delta_time * delta_time / 2;
        vz_prev = vz;
        arr_sz[trajectory_cnt] = sz / 10;
    }
    //увеличить счётчик тректории
    trajectory_cnt++;
    if(trajectory_cnt == 360000)
      trajectory_cnt = 0;
}

void drawTrajectory()
{
    stroke(255, 0, 0);
    beginShape(LINES);
    //отрисовка траектории
    int i;
    for(i = 1; i < trajectory_cnt; i++)
    {
        vertex(arr_sx[i - 1], -arr_sy[i - 1], arr_sz[i - 1]); vertex(arr_sx[i], -arr_sy[i], arr_sz[i]);
    }
    endShape();
} 

void keyPressed() 
{
    if (key == CODED) 
    {
      //управление углами вращения графика
      if (keyCode == RIGHT) 
        angle_y += 10;
      if (keyCode == LEFT)
        angle_y -= 10;
        
      //кнопки старта и финиша
      if(keyCode == ALT)
        finish = 1;
    }
}

void draw_ISO()
{
    //отрисовка рамки ИСО
    stroke(255);
    line(990, 400, 1830, 400);
    line(990, 400, 990, 900);
    line(1830, 400, 1830, 900);
    line(990, 900, 1830, 900);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
