import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;

//массив давления за время полёта
float[] arr_pressure = new float[360000];
//массив времени
float[] arr_time = new float[360000];
//счётчик для массивов
int arr_cnt = 0;
//количесвтво значений в массиве
int cnt_max;
//максимальное значение давления
float max_pressure;
//минимальное значение давления
float min_pressure;
//изменение давления
float delta_pressure;
//конечное время полёта
float max_time;
//начальное время полёта
float min_time;
//время полёта
float delta_time;
//выводить с точностью до 2 знаков после запятой
char char_time;
//значение давления (в пикселях)
float pressure_y;
//значение времени по(в пикселях)
float time_x;
//предыдущее значение давления(в пикселях)
float pressure_y_prev;
//предыдущее значение времени(в пикселях)
float time_x_prev;
//время из функции scope
float scope_time;
//давления из функции scope
float scope_pressure;

void setup()
{
    //создать окно
    size(1880, 1000);
    //залить чёрным цветом
    background(0);
    //считать данные из файла
    parseFile();
}

void draw()
{
    //залить чёрным цветом
    background(0);
    //узнать максимальное и минимальное показания давления
    min_max_pressure();
    //узнать начальное и конечное время
    min_max_time();
    //посчитать изменение давления
    delta_pressure = max_pressure - min_pressure;
    //посчитать время полёта
    delta_time = max_time - min_time;
    //узнать время и давление в данной точке
    scope();
    //нарисовать график
    draw_plot();
    //нарисовать оси
    draw_axis();
}

void parseFile() 
{
    //считывание из файла
    BufferedReader reader = createReader("dataLaunch.txt");
    String line = null;
    try 
    {
        while ((line = reader.readLine()) != null) 
        {
            String[] pieces = split(line, ",");
            //если пришли все данные целые (11 значений)
            if(int(pieces[0]) % 2 == 0 && pieces.length >= 4)
            {
              //считать температуру
              float pressure = float(pieces[3]); 
              //считать время
              float time = float(pieces[1]) / 1000;
              //добавить температуру в массив температур
              arr_pressure[arr_cnt] = pressure;
              //добавть время в массив времени
              arr_time[arr_cnt] = time;
              //увеличить счётчик
              arr_cnt += 1;
              //обновить максимальное значение счётчика
              cnt_max = arr_cnt;
            }
        }
        //закрыть файл
        reader.close();
    } 
    catch (IOException e)
    {
        e.printStackTrace();
    }
} 

void min_max_pressure()
{
    //найти минимум и максимум в массиве давления
    int i;
    max_pressure = arr_pressure[0];
    min_pressure = arr_pressure[0];
    for(i = 0; i < cnt_max; i++)
    {
        if(arr_pressure[i] >= max_pressure)
            max_pressure = arr_pressure[i];
            
        if(arr_pressure[i] <= min_pressure)
            min_pressure = arr_pressure[i];
    }
}

void min_max_time()
{
    //найти максимум и минимум в массиве времени
    min_time = arr_time[0];
    max_time = arr_time[cnt_max - 1];
}

void draw_axis()
{
    //нарисовать оси, подписать их, выставить значения
    stroke(100);
    int i;
    fill(255);
    textSize(15);
    
    for(i = 750; i >= 100; i-=50)
    {
        line(150, i, 1450, i);
        text(String.format("%.2f",max_pressure - (max_pressure - min_pressure) * (i - 100) / 650), 70, i + 5);
    }
    
    for(i = 1450; i >= 100; i-=100)
    {
        line(i, 100, i, 750);
        text(String.format("%.2f",(max_time - (max_time - min_time) * (i - 150) / 1300)), 1580 - i, 780);
    }
           
    stroke(255);
    line(150, 100, 150, 750);
    line(150, 750, 1450, 750);
    
    textSize(20);
    text("Время, с", 750, 840);
    
    translate(40, 500);
    rotate(-PI/2);
    text("Давление, Па", 0, 0);
}

void draw_plot()
{
    //рисовать график
    
    time_x_prev = 150 + (arr_time[0] - min_time) * 1300 / delta_time;
    pressure_y_prev = 750 - (arr_pressure[0] - min_pressure) * 650 / delta_pressure;
    int i;
    for(i = 0; i < cnt_max; i++)
    {
        pressure_y = 750 - (arr_pressure[i] - min_pressure) * 650 / delta_pressure;
        time_x = 150 + (arr_time[i] - min_time) * 1300 / delta_time;
        stroke(0, 255, 0);
        line(time_x_prev, pressure_y_prev, time_x, pressure_y);
        time_x_prev = time_x;
        pressure_y_prev = pressure_y;
    }
}

void scope()
{
    noCursor();
    //выводить значения в данной точке
    fill(255);
    textSize(35);
    text("Давление: "+String.format("%.2f", scope_pressure), 1500, 200);
    text("Время: "+String.format("%.2f",scope_time), 1500, 250);
    
    //если курсор в области графика, рисовать прямоугольник и оси к нему, считать значения в данной точке
    if(mouseX >= 150 && mouseX <= 1450 && mouseY >= 100 && mouseY <= 750)
    {
        //рисовать прямоугольник прозрачный
        fill(0, 0, 0, 255);
        rect(mouseX - 10, mouseY - 10, 20, 20);
        line(mouseX - 10, mouseY, mouseX + 10, mouseY);
        line(mouseX, mouseY - 10, mouseX, mouseY + 10);
        //рисовать оси к нему
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
        //считать время и значение в данной точке
        scope_time = (mouseX - 150) * delta_time / 1300 + min_time;
        scope_pressure = (750 - mouseY) * delta_pressure / 650 + min_pressure;
    }
}
