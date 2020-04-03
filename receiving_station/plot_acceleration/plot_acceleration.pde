import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;

//массив ускорений по оси x
float[] arr_ax = new float[360000];
//массив ускорений по оси y
float[] arr_ay = new float[360000];
//массив ускорений по оси z
float[] arr_az = new float[360000];
//массив времени
float[] arr_time = new float[360000];
//счётчик в массивах
int arr_cnt = 0;
//максимальное значение счётчика
int cnt_max;
//минимально ускорение
float min_a;
//максимальное ускорение
float max_a;
//изменение ускорения
float delta_a;
//начальное время
float min_time;
//конечное время
float max_time;
//время полёта
float delta_time;
//ускорение (в пикселях)
float a_y;
//время (в пикселях)
float time_x;
//ускорение предыдущее(в пикселях)
float a_y_prev;
//предыдущее время (в пикселях)
float time_x_prev;
//значение времени в данной точке
float scope_time;
//значения ускорения в данной точке
float scope_a;

void setup()
{
    //создать окно
    size(1880, 1000);
    //залить чёрным
    background(0);
    //считать данные из файла
    parseFile();
}

void draw()
{
    //залить чёрным
    background(0);
    //узнать максимальное и минимально ускорение
    min_max_a();
    //узнать начальное и конечное время
    min_max_time();
    //вычислить изменение ускорения
    delta_a = max_a - min_a;
    //вычислить время полёта
    delta_time = max_time - min_time;
    //узнать значения времени и ускорения в данной точке
    scope();
    //нарисовать гафик
    draw_plot();
    //нарисовать оси
    draw_axis();
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
            //если пришли все данные целые (11 значений)
            if(pieces.length == 11)
            {
              float ax = float(pieces[5]) * 9.816;
              float ay = float(pieces[6]) * 9.816;
              float az = float(pieces[7]) * 9.816;
              float time = float(pieces[8]) / 1000;
              arr_ax[arr_cnt] = ax;
              arr_ay[arr_cnt] = ay;
              arr_az[arr_cnt] = az;
              arr_time[arr_cnt] = time;
              arr_cnt += 1;
              cnt_max = arr_cnt;
            }
        }
        reader.close();
    } 
    catch (IOException e)
    {
        e.printStackTrace();
    }
} 

void min_max_time()
{
    //начальное и конечное время
    min_time = arr_time[0];
    max_time = arr_time[cnt_max - 1];
}

void min_max_a()
{  
    //максимальное и минимальное ускорение. Ищем по 3 массивам
    int i;
    max_a = arr_ax[0];
    min_a = arr_ax[0];
    for(i = 0; i < cnt_max; i++)
    {
        if(arr_ax[i] >= max_a)
            max_a = arr_ax[i];
            
        if(arr_ax[i] <= min_a)
            min_a = arr_ax[i];
    }
    for(i = 0; i < cnt_max; i++)
    {
        if(arr_ay[i] >= max_a)
            max_a = arr_ay[i];
            
        if(arr_ay[i] <= min_a)
            min_a = arr_ay[i];
    }
    for(i = 0; i < cnt_max; i++)
    {
        if(arr_az[i] >= max_a)
            max_a = arr_az[i];
            
        if(arr_az[i] <= min_a)
            min_a = arr_az[i];
    }
}

void draw_axis()
{
    //рисуем сетку графика и делаем подписи значений
    stroke(100);
    int i;
    fill(255);
    textSize(15);
    
    for(i = 750; i >= 100; i-=50)
    {
        line(150, i, 1450, i);
        text(String.format("%.2f",max_a - (max_a - min_a) * (i - 100) / 650), 100, i + 5);
    }
    
    for(i = 1450; i >= 100; i-=100)
    {
        line(i, 100, i, 750);
        text(String.format("%.2f",(max_time - (max_time - min_time) * (i - 150) / 1300)), 1580 - i, 780);
    }
    
    //оси графика
    stroke(255);
    line(150, 100, 150, 750);
    //line(150, 750, 1450, 750);
    line(150, 750 - (0 - min_a) * 650 / delta_a, 1450, 750 - (0 - min_a * 650 / delta_a));
    
    //пояснение каким цветом какое ускорение
    textSize(35);
    fill(255, 0, 60);
    text("Ускорение по оси x", 1500, 300);
    fill(0, 255, 98);
    text("Ускорение по оси y", 1500, 350);
    fill(0, 238, 255);
    text("Ускорение по оси z", 1500, 400);
    
    //подпись осей
    fill(255);
    textSize(20);
    text("Время, с", 750, 840);
    
    translate(80, 500);
    rotate(-PI/2);
    text("Ускорение, м/с^2", 0, 0);
}

void draw_plot()
{
    //рисовать график
    time_x_prev = 150 + (arr_time[0] - min_time) * 1300 / delta_time;
    a_y_prev = 750 - (arr_ax[0] - min_a) * 650 / delta_a;
    int i;
    for(i = 0; i < cnt_max; i++)
    {
        a_y = 750 - (arr_ax[i] - min_a) * 650 / delta_a;
        time_x = 150 + (arr_time[i] - min_time) * 1300 / delta_time;
        stroke(255, 0, 60);
        line(time_x_prev, a_y_prev, time_x, a_y);
        time_x_prev = time_x;
        a_y_prev = a_y;
    }
    
    time_x_prev = 150 + (arr_time[0] - min_time) * 1300 / delta_time;
    a_y_prev = 750 - (arr_ay[0] - min_a) * 650 / delta_a;
    for(i = 0; i < cnt_max; i++)
    {
        a_y = 750 - (arr_ay[i] - min_a) * 650 / delta_a;
        time_x = 150 + (arr_time[i] - min_time) * 1300 / delta_time;
        stroke(0, 255, 98);
        line(time_x_prev, a_y_prev, time_x, a_y);
        time_x_prev = time_x;
        a_y_prev = a_y;
    }
    
    time_x_prev = 150 + (arr_time[0] - min_time) * 1300 / delta_time;
    a_y_prev = 750 - (arr_az[0] - min_a) * 650 / delta_a;
    for(i = 0; i < cnt_max; i++)
    {
        a_y = 750 - (arr_az[i] - min_a) * 650 / delta_a;
        time_x = 150 + (arr_time[i] - min_time) * 1300 / delta_time;
        stroke(0, 238, 255);
        line(time_x_prev, a_y_prev, time_x, a_y);
        time_x_prev = time_x;
        a_y_prev = a_y;
    }
}

void scope()
{
    //вывести значения ускорения и времени
    textSize(35);
    stroke(255);
    text("ускорение: "+String.format("%.3f", scope_a), 1500, 200);
    text("time: "+String.format("%.2f",scope_time), 1500, 250);
    //если курсор находится в области графика
    if(mouseX >= 150 && mouseX <= 1450 && mouseY >= 100 && mouseY <= 750)
    {
        //нарисовать квадрат
        fill(0, 0, 0, 255);
        rect(mouseX - 10, mouseY - 10, 20, 20);
        //нарисовать к нему оси
        int i;
        stroke(150);
        for(i = 170; i < mouseX - 10; i += 20)
            line(i - 10, mouseY, i, mouseY);
        for(i = mouseX + 30; i < 1450; i += 20)
            line(i - 10, mouseY, i, mouseY);
        for(i = 120; i < mouseY - 10; i += 20)
            line(mouseX, i - 10, mouseX, i);
        for(i = mouseY + 30; i < 750; i += 20)
            line(mouseX, i - 10, mouseX, i);
        //считать время и значение ускорения в данной точке
        scope_time = (mouseX - 150) * delta_time / 1300 + min_time;
        scope_a = (750 - mouseY) * delta_a / 650 + min_a;
    }
}
