import processing.serial.*;
import toxi.geom.*;
import toxi.processing.*;

//массив температуры за время полёта
float[] arr_tempreture = new float[360000];
//массив времени
float[] arr_time = new float[360000];
//счётчик для массивов
int arr_cnt = 0;
//количесвтво значений в массиве
int cnt_max;
//максимальное значение температуры
float max_tempreture;
//минимальное значение температуры
float min_tempreture;
//изменение температуры
float delta_tempreture;
//конечное время полёта
float max_time;
//начальное время полёта
float min_time;
//время полёта
float delta_time;
//выводить с точностью до 2 знаков после запятой
char char_time;
//значение температуры (в пикселях)
float tempreture_y;
//значение времени по(в пикселях)
float time_x;
//предыдущее значение температуры (в пикселях)
float tempreture_y_prev;
//предыдущее значение времени(в пикселях)
float time_x_prev;
//время из функции scope
float scope_time;
//температура из функции scope
float scope_tempreture;

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
    //узнать максимальное и минимальное показание температуры
    min_max_Tempreture();
    //узнать начальное и конечное время
    min_max_time();
    //посчитать изменение температуры
    delta_tempreture = max_tempreture - min_tempreture;
    //посчитать время полёта
    delta_time = max_time - min_time;
    //узнать время и температуру в данной точке
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
            if(pieces.length == 11)
            {
              //считать температуру
              float tempreture = float(pieces[10]); 
              //считать время
              float time = float(pieces[8]) / 1000;
              //добавить температуру в массив температур
              arr_tempreture[arr_cnt] = tempreture;
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

void min_max_Tempreture()
{
    //найти минимум и максимум в массиве температуры
    int i;
    max_tempreture = arr_tempreture[0];
    min_tempreture = arr_tempreture[0];
    for(i = 0; i < cnt_max; i++)
    {
        if(arr_tempreture[i] >= max_tempreture)
            max_tempreture = arr_tempreture[i];
            
        if(arr_tempreture[i] <= min_tempreture)
            min_tempreture = arr_tempreture[i];
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
        text(String.format("%.2f",max_tempreture - (max_tempreture - min_tempreture) * (i - 100) / 650), 90, i + 5);
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
    text("Температура, °C", 0, 0);
}

void draw_plot()
{
    //рисовать график
    time_x_prev = 150 + (arr_time[0] - min_time) * 1300 / delta_time;
    tempreture_y_prev = 750 - (arr_tempreture[0] - min_tempreture) * 650 / delta_tempreture;
    int i;
    for(i = 0; i < cnt_max; i++)
    {
        tempreture_y = 750 - (arr_tempreture[i] - min_tempreture) * 650 / delta_tempreture;
        time_x = 150 + (arr_time[i] - min_time) * 1300 / delta_time;
        stroke(0, 255, 0);
        line(time_x_prev, tempreture_y_prev, time_x, tempreture_y);
        time_x_prev = time_x;
        tempreture_y_prev = tempreture_y;
    }
}

void scope()
{
    //выводить значения в данной точке
    fill(255);
    textSize(35);
    text("Температура: "+String.format("%.2f", scope_tempreture), 1500, 200);
    text("Время: "+String.format("%.2f", scope_time), 1500, 250);
    
    //если курсор в области графика, рисовать прямоугольник и оси к нему, считать значения в данной точке
    if(mouseX >= 150 && mouseX <= 1450 && mouseY >= 100 && mouseY <= 750)
    {
        //рисовать прямоугольник прозрачный
        fill(0, 0, 0, 255);
        rect(mouseX - 10, mouseY - 10, 20, 20);
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
        scope_tempreture = (750 - mouseY) * delta_tempreture / 650 + min_tempreture;
    }
}
