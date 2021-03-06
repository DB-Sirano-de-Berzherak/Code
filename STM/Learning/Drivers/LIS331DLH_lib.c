#include "lis331dlh_reg.h"

//                                Раздел функция для чтения с общего регистра и записи в него


//ctx  определения интерфейса чтения/записи (указатель)
//reg  регистр, который читают 
//data указатель на буфер, в котором хранятся прочитанные данные (ptr)
//len  по сколько бит читать
 
//Чтение из регистра
int32_t lis331dlh_read_reg(stmdev_ctx_t* ctx, uint8_t reg, uint8_t* data, uint16_t len)
{
    int32_t ret;
    ret = ctx->read_reg(ctx->handle, reg, data, len);
    return ret;
}

//запись в регистр
int32_t lis331dlh_write_reg(stmdev_ctx_t* ctx, uint8_t reg, uint8_t* data, uint16_t len)
{
    int32_t ret;
    ret = ctx->write_reg(ctx->handle, reg, data, len);
    return ret;
}


//                      Функции, преобразующие необработанные данные в технические единицы

//перевести данные (диапазон 2g)
float lis331dlh_from_fs2_to_mg(int16_t lsb)
{
    return ((float)lsb / 16.0f);
}

//перевести данные (диапазон 4g)
float lis331dlh_from_fs4_to_mg(int16_t lsb)
{
    return ((float)lsb * 2.0f / 16.0f);
}

//перевести данные (диапазон 8g)
float lis331dlh_from_fs8_to_mg(int16_t lsb)
{
    return ((float)lsb * 3.9f / 16.0f);
}

//                                   Функции, относящиеся к генерации данных

//ctx определения интерфейса чтения/записи (указатель)
//val меняет значения xen/yen/zen в reg CTRL_REG1 (xen показывает включить/выключить ось x/y/z)

//Ось x включить/выключить
int32_t lis331dlh_axis_x_data_set(stmdev_ctx_t *ctx, uint8_t val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //в ret записать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //если записалось правильно
    if(ret == 0) 
    {
      //в xen присвоить значение (включить/выключить)
      ctrl_reg1.xen = val;
      //в ret записать обновлённую структуру
      ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    }
    return ret;
}

//узнать состояние оси x
int32_t lis331dlh_axis_x_data_get(stmdev_ctx_t *ctx, uint8_t *val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //считать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //в val записать значение бита xen 
    *val = ctrl_reg1.xen 
    return ret;
}

//Ось y включить/выключить
int32_t lis331dlh_axis_y_data_set(stmdev_ctx_t *ctx, uint8_t val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //в ret записать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //если записалось правильно
    if(ret == 0) {
      //в yen присвоить значение (включить/выключить)
      ctrl_reg1.yen = val;
      //в ret записать обновлённую структуру
      ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    }
    return ret;
}

//узнать состояние оси y
int32_t lis331dlh_axis_y_data_get(stmdev_ctx_t *ctx, uint8_t *val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //считать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //в val записать значение бита yen
    *val = ctrl_reg1.yen;

    return ret;
}

//Ось z включить/выключить
int32_t lis331dlh_axis_z_data_set(stmdev_ctx_t *ctx, uint8_t val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //в ret записать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //если записалось правильно
    if(ret == 0) {
      //в zen присвоить значение (включить/выключить)
      ctrl_reg1.zen = val;
      //в ret записать обновлённую структуру
      ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    }
    return ret;
}

//узнать состояние оси z
int32_t lis331dlh_axis_z_data_get(stmdev_ctx_t *ctx, uint8_t *val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //считать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //в val записать значение бита zen
    *val = ctrl_reg1.zen;

    return ret;
}

//                             Функции относящиеся к выбору скорости передачи данных

//ctx определения интерфейса чтения/записи (указатель)
//val значение бита dr в CTRL_REG1

//установить скорость пережачи данных
int32_t lis331dlh_data_rate_set(stmdev_ctx_t *ctx, lis331dlh_dr_t val)
{
    lis331dlh_ctrl_reg1_t ctrl_reg1;
    int32_t ret;
    //считать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    //если считалось правильно
    if(ret == 0) {
      //с помоощью xor (побитовое "и") расчитать режим pm
      ctrl_reg1.pm = (uint8_t)val & 0x07U;
      //с помоощью xor (побитовое "и") расчитать режим dr, сдвинуться на 4 бита
      ctrl_reg1.dr = ( (uint8_t)val & 0x30U ) >> 4;
      //перезаписать структуру
      ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
    }
    return ret;
}

//выбор скорости передачи данных акселерометра
int32_t lis331dlh_data_rate_get(stmdev_ctx_t *ctx, lis331dlh_dr_t *val)
{
  lis331dlh_ctrl_reg1_t ctrl_reg1;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG1, (uint8_t*)&ctrl_reg1, 1);
  //посчитать режим. В зависимости от результата присвоить val значенние частоты 
  switch ((ctrl_reg1.dr << 4) + ctrl_reg1.pm)
  {
    case LIS331DLH_ODR_OFF:
      *val = LIS331DLH_ODR_OFF;
      break;
    case LIS331DLH_ODR_Hz5:
      *val = LIS331DLH_ODR_Hz5;
      break;
    case LIS331DLH_ODR_1Hz:
      *val = LIS331DLH_ODR_1Hz;
      break;
    case LIS331DLH_ODR_2Hz:
      *val = LIS331DLH_ODR_2Hz;
      break;
    case LIS331DLH_ODR_5Hz:
      *val = LIS331DLH_ODR_5Hz;
      break;
    case LIS331DLH_ODR_10Hz:
      *val = LIS331DLH_ODR_10Hz;
      break;
    case LIS331DLH_ODR_50Hz:
      *val = LIS331DLH_ODR_50Hz;
      break;
    case LIS331DLH_ODR_100Hz:
      *val = LIS331DLH_ODR_100Hz;
      break;
    case LIS331DLH_ODR_400Hz:
      *val = LIS331DLH_ODR_400Hz;
      break;
    case LIS331DLH_ODR_1kHz:
      *val = LIS331DLH_ODR_1kHz;
      break;
    default:
      *val = LIS331DLH_ODR_OFF;
      break;
  }

  return ret;
}

//Установка режима фильтра верхних частот

//val изменяет значения hpm в регистре CTRL_REG2
int32_t lis331dlh_reference_mode_set(stmdev_ctx_t *ctx, lis331dlh_hpm_t val)
{
    lis331dlh_ctrl_reg2_t ctrl_reg2;
    int32_t ret;
    //считать структуру
    ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
    //если считалось правильно
    if(ret == 0) {
      //установить в hmp значение режима
      //LIS331DLH_NORMAL_MODE (00 или 10) или LIS331DLH_REF_MODE_ENABLE (01)
      ctrl_reg2.hpm = (uint8_t)val;
      //обновить структуру
      ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
    }
    return ret;
}

//Выбор фильтра верхних частот
int32_t lis331dlh_reference_mode_get(stmdev_ctx_t *ctx, lis331dlh_hpm_t *val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2,(uint8_t*)&ctrl_reg2, 1);
  //присвоить val значение режима фильтра
  switch (ctrl_reg2.hpm)
  {
    case LIS331DLH_NORMAL_MODE:
      *val = LIS331DLH_NORMAL_MODE;
      break;
    case LIS331DLH_REF_MODE_ENABLE:
      *val = LIS331DLH_REF_MODE_ENABLE;
      break;
    default:
      *val = LIS331DLH_NORMAL_MODE;
      break;
  }
  return ret;
}

//установка диапаона значений акселерометра
int32_t lis331dlh_full_scale_set(stmdev_ctx_t *ctx, lis331dlh_fs_t val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  //если структура считалась правильно
  if(ret == 0) {
    //записать в бит fs режим (00 - 2g, 01 - 4g, 11 - 8g) / LIS331DLH_ng
    ctrl_reg4.fs = (uint8_t)val
    //перезаписать структуру
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  }
  return ret;
}

//узнать диапазон значений акселерометра
int32_t lis331dlh_full_scale_get(stmdev_ctx_t *ctx, lis331dlh_fs_t *val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;
  //считать 
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  //записать в val значение бита fs регистра reg4
  switch (ctrl_reg4.fs)
  {
    case LIS331DLH_2g:
      *val = LIS331DLH_2g;
      break;
    case LIS331DLH_4g:
      *val = LIS331DLH_4g;
      break;
    case LIS331DLH_8g:
      *val = LIS331DLH_8g;
      break;
    default:
      *val = LIS331DLH_2g;
      break;
  }

  return ret;
}

//установка блокировки обновления данных
int32_t lis331dlh_block_data_update_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  if(ret == 0) {
    //записать в бит bdu значение val
    //0 - можно менять, 1 - нельзя менять
    ctrl_reg4.bdu = val;
    //перезаписать структуру
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  }
  return ret;
}

//Узнать состояние блокировки данных
int32_t lis331dlh_block_data_update_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;
  //считать структтуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  //в val записать значение бита bdu
  *val = ctrl_reg4.bdu;

  return ret;
}

//регист STATUS_REG считывается интерфейсом
//val состояние регистра STATUS_REG
int32_t lis331dlh_status_reg_get(stmdev_ctx_t *ctx, lis331dlh_status_reg_t *val)
{
  int32_t ret;
  //считать структуру регистра
  ret = lis331dlh_read_reg(ctx, LIS331DLH_STATUS_REG, (uint8_t*) val, 1);
  return ret;
}

//Узнать доступны ли новые данные акселерометра
//val бит zyxda регистра STATUS_REG
int32_t lis331dlh_flag_data_ready_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_status_reg_t status_reg;
  int32_t ret;
  //считать регистр
  ret = lis331dlh_read_reg(ctx, LIS331DLH_STATUS_REG, (uint8_t*)&status_reg, 1);
  //в val записать значение бита zyxda
  *val = status_reg.zyxda;
 
  return ret;
}

//                                          Блок функций для получения данных

//регистр линейного ускорения. Val представлено как 16-бтовое слово со знаком
//в buffer хранятся данные для чтения
int32_t lis331dlh_acceleration_raw_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;
  //считать данные из buff в OUT_X_L, OUT_X_H....
  ret = lis331dlh_read_reg(ctx, LIS331DLH_OUT_X_L, buff, 6);
  return ret;
}

//                                          Блок общих полезных функций

//Device "Who am I"
//получтить id устройства 
int32_t lis331dlh_device_id_get(stmdev_ctx_t *ctx, uint8_t *buff)
{
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_WHO_AM_I, buff, 1);
  return ret;
}

//перезагрузить содержимого памяти. Перезагрузить параметров калибровки
//val значение бита boot (перезагрузка памяьи) в регистре CTRL_REG2
int32_t lis331dlh_boot_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  if(ret == 0) {
    //присвоить в boot значение val
    ctrl_reg2.boot = val;
    //перезаписать структуру
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  }
  return ret;
}

//Узнать состояние бита boot (перезагрузка памяти)
int32_t lis331dlh_boot_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  //в val записать значение бита boot
  *val = ctrl_reg2.boot;

  return ret;
}

//включить самотестирование датчика ускорения
//val изменяет значение бита st (самотестирование отключено/включено) в регистре CTRL_REG4
int32_t lis331dlh_self_test_set(stmdev_ctx_t *ctx, lis331dlh_st_t val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  //если считалось правильно
  if(ret == 0) {
    //записать в st значение val 
    ctrl_reg4.st = (uint8_t)val;
    //перезаписать структуру
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  }
  return ret;
}

//узнать состояние бита самотестрирования
int32_t lis331dlh_self_test_get(stmdev_ctx_t *ctx, lis331dlh_st_t *val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;
  //считать структуру
  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  //в val записать состояние бита st
  switch (ctrl_reg4.st)
  {
    case LIS331DLH_ST_DISABLE:
      *val = LIS331DLH_ST_DISABLE;
      break;
    case LIS331DLH_ST_POSITIVE:
      *val = LIS331DLH_ST_POSITIVE;
      break;
    case LIS331DLH_ST_NEGATIVE:
      *val = LIS331DLH_ST_NEGATIVE;
      break;
    default:
      *val = LIS331DLH_ST_DISABLE;
      break;
  }

  return ret;
}

//Установить порядок устанвки битов (от страршего к младшему или от младшего к старшему)
//val значения бита ble в регистре CTRL_REG4
int32_t lis331dlh_data_format_set(stmdev_ctx_t *ctx, lis331dlh_ble_t val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  if(ret == 0) {
    ctrl_reg4.ble = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG4,
                              (uint8_t*)&ctrl_reg4, 1);
  }
  return ret;
}

//Узнать порядок устанвки битов (от страршего к младшему или от младшего к старшему)
//val значения бита ble в регистре CTRL_REG4
int32_t lis331dlh_data_format_get(stmdev_ctx_t *ctx, lis331dlh_ble_t *val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);

  switch (ctrl_reg4.ble)
  {
    case LIS331DLH_LSB_AT_LOW_ADD:
      *val = LIS331DLH_LSB_AT_LOW_ADD;
      break;
    case LIS331DLH_MSB_AT_LOW_ADD:
      *val = LIS331DLH_MSB_AT_LOW_ADD;
      break;
    default:
      *val = LIS331DLH_LSB_AT_LOW_ADD;
      break;
  }

  return ret;
}

//                              Блок функций для настрйки фильтров

//настройка фильтра верхних частот
//val значение бита hpcf в регистре CTRL_REG2
int32_t lis331dlh_hp_bandwidth_set(stmdev_ctx_t *ctx, lis331dlh_hpcf_t val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  if(ret == 0) {
    ctrl_reg2.hpcf = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  }
  return ret;
}

//узнать настройки фильтра верхних частот
int32_t lis331dlh_hp_bandwidth_get(stmdev_ctx_t *ctx, lis331dlh_hpcf_t *val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);

  switch (ctrl_reg2.hpcf)
  {
    case LIS331DLH_CUT_OFF_8Hz:
      *val = LIS331DLH_CUT_OFF_8Hz;
      break;
    case LIS331DLH_CUT_OFF_16Hz:
      *val = LIS331DLH_CUT_OFF_16Hz;
      break;
    case LIS331DLH_CUT_OFF_32Hz:
      *val = LIS331DLH_CUT_OFF_32Hz;
      break;
    case LIS331DLH_CUT_OFF_64Hz:
      *val = LIS331DLH_CUT_OFF_64Hz;
      break;
    default:
      *val = LIS331DLH_CUT_OFF_8Hz;
      break;
  }

  return ret;
}

//Установить профиль фильтра
//val значение бита hpen в регистре CTRL_REG2
int32_t lis331dlh_hp_path_set(stmdev_ctx_t *ctx, lis331dlh_hpen_t val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  if(ret == 0) {
    ctrl_reg2.hpen = (uint8_t)val & 0x03U;
    ctrl_reg2.fds = ((uint8_t)val & 0x04U) >> 2;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);
  }
  return ret;
}

//Узнать профиль фильтра
int32_t lis331dlh_hp_path_get(stmdev_ctx_t *ctx, lis331dlh_hpen_t *val)
{
  lis331dlh_ctrl_reg2_t ctrl_reg2;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG2, (uint8_t*)&ctrl_reg2, 1);

  switch ( (ctrl_reg2.fds << 2) + ctrl_reg2.hpen )
  {
    case LIS331DLH_HP_DISABLE:
      *val = LIS331DLH_HP_DISABLE;
      break;
    case LIS331DLH_HP_ON_OUT:
      *val = LIS331DLH_HP_ON_OUT;
      break;
    case LIS331DLH_HP_ON_INT1:
      *val = LIS331DLH_HP_ON_INT1;
      break;
    case LIS331DLH_HP_ON_INT2:
      *val = LIS331DLH_HP_ON_INT2;
      break;
    case LIS331DLH_HP_ON_INT1_INT2:
      *val = LIS331DLH_HP_ON_INT1_INT2;
      break;
    case LIS331DLH_HP_ON_INT1_INT2_OUT:
      *val = LIS331DLH_HP_ON_INT1_INT2_OUT;
      break;
    case LIS331DLH_HP_ON_INT2_OUT:
      *val = LIS331DLH_HP_ON_INT2_OUT;
      break;
    case LIS331DLH_HP_ON_INT1_OUT:
      *val = LIS331DLH_HP_ON_INT1_OUT;
      break;
    default:
      *val = LIS331DLH_HP_DISABLE;
      break;
  }
  return ret;
}

/*чтение по этому адресу мгновенно обнуляется
   содержание внутреннего фильтра верхних частот.
   Если фильтр верхних частот включен, все три оси
   мгновенно установлены на 0g. Это позволяет
   преодолеть время установления высокого прохода
   фильтр
*/
int32_t lis331dlh_hp_reset_get(stmdev_ctx_t *ctx)
{
  uint8_t dummy;
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_HP_FILTER_RESET, (uint8_t*)&dummy, 1);
  return ret;
}

//установить эталонное значение ускорения в регистре reference                  ??
//val менияет значение ref в регистре REFERENCE
int32_t lis331dlh_hp_reference_value_set(stmdev_ctx_t *ctx, uint8_t val)
{
  int32_t ret;
  ret = lis331dlh_write_reg(ctx, LIS331DLH_REFERENCE, (uint8_t*)&val, 1);
  return ret;
}

//узнать эталонное значение ускорения в регистре reference   
int32_t lis331dlh_hp_reference_value_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_REFERENCE, val, 1);
  return ret;
}

//                                        Блок функций для последовательного управления интрерфйсами

//Установить SPI 3- или 4-проводной SPI интрерфейс
//val значение бита sim в регистреCTRL_REG4
int32_t lis331dlh_spi_mode_set(stmdev_ctx_t *ctx, lis331dlh_sim_t val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  if(ret == 0) {
    ctrl_reg4.sim = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);
  }
  return ret;
}

//узнать 3- или 4-проводной интрерфейс SPI
int32_t lis331dlh_spi_mode_get(stmdev_ctx_t *ctx, lis331dlh_sim_t *val)
{
  lis331dlh_ctrl_reg4_t ctrl_reg4;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG4, (uint8_t*)&ctrl_reg4, 1);

  switch ( ctrl_reg4.sim )
  {
    case LIS331DLH_SPI_4_WIRE:
      *val = LIS331DLH_SPI_4_WIRE;
      break;
    case LIS331DLH_SPI_3_WIRE:
      *val = LIS331DLH_SPI_3_WIRE;
      break;
    default:
      *val = LIS331DLH_SPI_4_WIRE;
      break;
  }

  return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                               Блок функция для прерывания контакта

//установить контрольные биты сигнала данных на выходе первого прерывания
//val значение бита i1_cfg в регистре CTRL_REG3
int32_t lis331dlh_pin_int1_route_set(stmdev_ctx_t *ctx,lis331dlh_i1_cfg_t val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  if(ret == 0) {
    ctrl_reg3.i1_cfg = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  }
  return ret;
}

//узнать контрольные биты сигнала данных на выходе первого прерывания
int32_t lis331dlh_pin_int1_route_get(stmdev_ctx_t *ctx,
                                     lis331dlh_i1_cfg_t *val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);

  switch ( ctrl_reg3.i1_cfg )
  {
    case LIS331DLH_PAD1_INT1_SRC:
      *val = LIS331DLH_PAD1_INT1_SRC;
      break;
    case LIS331DLH_PAD1_INT1_OR_INT2_SRC:
      *val = LIS331DLH_PAD1_INT1_OR_INT2_SRC;
      break;
    case LIS331DLH_PAD1_DRDY:
      *val = LIS331DLH_PAD1_DRDY;
      break;
    case LIS331DLH_PAD1_BOOT:
      *val = LIS331DLH_PAD1_BOOT;
      break;
    default:
      *val = LIS331DLH_PAD1_INT1_SRC;
      break;
  }

  return ret;
}

//установка запрос на прерывание в регистре INT1_SRC с помощью INT1_SRC регистр очищается чтением регистра INT1_SRC
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of lir1 in reg CTRL_REG3
  *
  */
int32_t lis331dlh_int1_notification_set(stmdev_ctx_t *ctx,
                                        lis331dlh_lir1_t val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  if(ret == 0) {
    ctrl_reg3.lir1 = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG3,
                              (uint8_t*)&ctrl_reg3, 1);
  }
  return ret;
}

/**
  * @brief  Latch interrupt request on INT1_SRC register, with INT1_SRC
  *         register cleared by reading INT1_SRC register.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of lir1 in reg CTRL_REG3
  *
  */
int32_t lis331dlh_int1_notification_get(stmdev_ctx_t *ctx,
                                        lis331dlh_lir1_t *val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);

  switch ( ctrl_reg3.lir1 )
  {
    case LIS331DLH_INT1_PULSED:
      *val = LIS331DLH_INT1_PULSED;
      break;
    case LIS331DLH_INT1_LATCHED:
      *val = LIS331DLH_INT1_LATCHED;
      break;
    default:
      *val = LIS331DLH_INT1_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  Data signal on INT 2 pad control bits.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of i2_cfg in reg CTRL_REG3
  *
  */
int32_t lis331dlh_pin_int2_route_set(stmdev_ctx_t *ctx,
                                     lis331dlh_i2_cfg_t val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  if(ret == 0) {
    ctrl_reg3.i2_cfg = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG3,
                              (uint8_t*)&ctrl_reg3, 1);
  }
  return ret;
}

/**
  * @brief  Data signal on INT 2 pad control bits.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of i2_cfg in reg CTRL_REG3
  *
  */
int32_t lis331dlh_pin_int2_route_get(stmdev_ctx_t *ctx,
                                     lis331dlh_i2_cfg_t *val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);

  switch ( ctrl_reg3.i2_cfg )
  {
    case LIS331DLH_PAD2_INT2_SRC:
      *val = LIS331DLH_PAD2_INT2_SRC;
      break;
    case LIS331DLH_PAD2_INT1_OR_INT2_SRC:
      *val = LIS331DLH_PAD2_INT1_OR_INT2_SRC;
      break;
    case LIS331DLH_PAD2_DRDY:
      *val = LIS331DLH_PAD2_DRDY;
      break;
    case LIS331DLH_PAD2_BOOT:
      *val = LIS331DLH_PAD2_BOOT;
      break;
    default:
      *val = LIS331DLH_PAD2_INT2_SRC;
      break;
  }

  return ret;
}

/**
  * @brief  Latch interrupt request on INT2_SRC register, with INT2_SRC
  *         register cleared by reading INT2_SRC itself.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of lir2 in reg CTRL_REG3
  *
  */
int32_t lis331dlh_int2_notification_set(stmdev_ctx_t *ctx,
                                        lis331dlh_lir2_t val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  if(ret == 0) {
    ctrl_reg3.lir2 = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG3,
                              (uint8_t*)&ctrl_reg3, 1);
  }
  return ret;
}

/**
  * @brief  Latch interrupt request on INT2_SRC register, with INT2_SRC
  *         register cleared by reading INT2_SRC itself.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of lir2 in reg CTRL_REG3
  *
  */
int32_t lis331dlh_int2_notification_get(stmdev_ctx_t *ctx,
                                        lis331dlh_lir2_t *val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);

  switch ( ctrl_reg3.lir2 )
  {
    case LIS331DLH_INT2_PULSED:
      *val = LIS331DLH_INT2_PULSED;
      break;
    case LIS331DLH_INT2_LATCHED:
      *val = LIS331DLH_INT2_LATCHED;
      break;
    default:
      *val = LIS331DLH_INT2_PULSED;
      break;
  }

  return ret;
}

/**
  * @brief  Push-pull/open drain selection on interrupt pads.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of pp_od in reg CTRL_REG3
  *
  */
int32_t lis331dlh_pin_mode_set(stmdev_ctx_t *ctx, lis331dlh_pp_od_t val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  if(ret == 0) {
    ctrl_reg3.pp_od = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG3,
                              (uint8_t*)&ctrl_reg3, 1);
  }
  return ret;
}

/**
  * @brief  Push-pull/open drain selection on interrupt pads.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of pp_od in reg CTRL_REG3
  *
  */
int32_t lis331dlh_pin_mode_get(stmdev_ctx_t *ctx, lis331dlh_pp_od_t *val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);

  switch ( ctrl_reg3.pp_od )
  {
    case LIS331DLH_PUSH_PULL:
      *val = LIS331DLH_PUSH_PULL;
      break;
    case LIS331DLH_OPEN_DRAIN:
      *val = LIS331DLH_OPEN_DRAIN;
      break;
    default:
      *val = LIS331DLH_PUSH_PULL;
      break;
  }

  return ret;
}

/**
  * @brief  Interrupt active-high/low.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ihl in reg CTRL_REG3
  *
  */
int32_t lis331dlh_pin_polarity_set(stmdev_ctx_t *ctx, lis331dlh_ihl_t val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);
  if(ret == 0) {
    ctrl_reg3.ihl = (uint8_t)val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG3,
                              (uint8_t*)&ctrl_reg3, 1);
  }
  return ret;
}

/**
  * @brief  Interrupt active-high/low.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of ihl in reg CTRL_REG3
  *
  */
int32_t lis331dlh_pin_polarity_get(stmdev_ctx_t *ctx, lis331dlh_ihl_t *val)
{
  lis331dlh_ctrl_reg3_t ctrl_reg3;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG3, (uint8_t*)&ctrl_reg3, 1);

  switch ( ctrl_reg3.ihl )
  {
    case LIS331DLH_ACTIVE_HIGH:
      *val = LIS331DLH_ACTIVE_HIGH;
      break;
    case LIS331DLH_ACTIVE_LOW:
      *val = LIS331DLH_ACTIVE_LOW;
      break;
    default:
      *val = LIS331DLH_ACTIVE_HIGH;
      break;
  }

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LIS331DLH_interrupt_on_threshold
  * @brief       This section groups all the functions that manage
  *              the interrupt on threshold event generation.
  * @{
  *
  */

/**
  * @brief  Configure the interrupt 1 threshold sign.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         enable sign and axis for interrupt on threshold
  *
  */
int32_t lis331dlh_int1_on_threshold_conf_set(stmdev_ctx_t *ctx,
                                              int1_on_th_conf_t val)
{
  lis331dlh_int1_cfg_t int1_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);
  if(ret == 0) {
    int1_cfg.xlie  = val.int1_xlie;
    int1_cfg.xhie  = val.int1_xhie;
    int1_cfg.ylie  = val.int1_ylie;
    int1_cfg.yhie  = val.int1_yhie;
    int1_cfg.zlie  = val.int1_zlie;
    int1_cfg.zhie  = val.int1_zhie;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT1_CFG,
                              (uint8_t*)&int1_cfg, 1);
  }
  return ret;
}

/**
  * @brief   Configure the interrupt 1 threshold sign.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         enable sign and axis for interrupt on threshold
  *
  */
int32_t lis331dlh_int1_on_threshold_conf_get(stmdev_ctx_t *ctx,
                                             int1_on_th_conf_t *val)
{
  lis331dlh_int1_cfg_t int1_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);
  val->int1_xlie = int1_cfg.xlie;
  val->int1_xhie = int1_cfg.xhie;
  val->int1_ylie = int1_cfg.ylie;
  val->int1_yhie = int1_cfg.yhie;
  val->int1_zlie = int1_cfg.zlie;
  val->int1_zhie = int1_cfg.zhie;

  return ret;
}

/**
  * @brief  AND/OR combination of Interrupt 1 events.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of aoi in reg INT1_CFG
  *
  */
int32_t lis331dlh_int1_on_threshold_mode_set(stmdev_ctx_t *ctx,
                                             lis331dlh_int1_aoi_t val)
{
  lis331dlh_int1_cfg_t int1_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);
  if(ret == 0) {
    int1_cfg.aoi = (uint8_t) val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT1_CFG,
                              (uint8_t*)&int1_cfg, 1);
  }
  return ret;
}

/**
  * @brief   AND/OR combination of Interrupt 1 events.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of aoi in reg INT1_CFG
  *
  */
int32_t lis331dlh_int1_on_threshold_mode_get(stmdev_ctx_t *ctx,
                                             lis331dlh_int1_aoi_t *val)
{
  lis331dlh_int1_cfg_t int1_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);

  switch ( int1_cfg.aoi )
  {
    case LIS331DLH_INT1_ON_THRESHOLD_OR:
      *val = LIS331DLH_INT1_ON_THRESHOLD_OR;
      break;
    case LIS331DLH_INT1_ON_THRESHOLD_AND:
      *val = LIS331DLH_INT1_ON_THRESHOLD_AND;
      break;
    default:
      *val = LIS331DLH_INT1_ON_THRESHOLD_OR;
      break;
  }

  return ret;
}

/**
  * @brief  Interrupt generator 1 on threshold source register.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         registers INT1_SRC
  *
  */
int32_t lis331dlh_int1_src_get(stmdev_ctx_t *ctx,
                               lis331dlh_int1_src_t *val)
{
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_SRC, (uint8_t*) val, 1);
  return ret;
}

/**
  * @brief  Interrupt 1 threshold.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT1_THS
  *
  */
int32_t lis331dlh_int1_treshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_int1_ths_t int1_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_THS, (uint8_t*)&int1_ths, 1);
  if(ret == 0) {
    int1_ths.ths = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT1_THS,
                              (uint8_t*)&int1_ths, 1);
  }
  return ret;
}

/**
  * @brief  Interrupt 1 threshold.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT1_THS
  *
  */
int32_t lis331dlh_int1_treshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_int1_ths_t int1_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_THS, (uint8_t*)&int1_ths, 1);
  *val = int1_ths.ths;

  return ret;
}

/**
  * @brief  Duration value for interrupt 1 generator.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of d in reg INT1_DURATION
  *
  */
int32_t lis331dlh_int1_dur_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_int1_duration_t int1_duration;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_DURATION,
                           (uint8_t*)&int1_duration, 1);
  if(ret == 0) {
    int1_duration.d = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT1_DURATION,
                              (uint8_t*)&int1_duration, 1);
  }
  return ret;
}

/**
  * @brief  Duration value for interrupt 1 generator.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of d in reg INT1_DURATION
  *
  */
int32_t lis331dlh_int1_dur_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_int1_duration_t int1_duration;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_DURATION,
                           (uint8_t*)&int1_duration, 1);
  *val = int1_duration.d;

  return ret;
}

/**
  * @brief  Configure the interrupt 2 threshold sign.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         enable sign and axis for interrupt on threshold
  *
  */
int32_t lis331dlh_int2_on_threshold_conf_set(stmdev_ctx_t *ctx,
                                              int2_on_th_conf_t val)
{
  lis331dlh_int2_cfg_t int2_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_CFG,
                           (uint8_t*)&int2_cfg, 1);
  if(ret == 0) {
    int2_cfg.xlie  = val.int2_xlie;
    int2_cfg.xhie  = val.int2_xhie;
    int2_cfg.ylie  = val.int2_ylie;
    int2_cfg.yhie  = val.int2_yhie;
    int2_cfg.zlie  = val.int2_zlie;
    int2_cfg.zhie  = val.int2_zhie;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT2_CFG,
                              (uint8_t*)&int2_cfg, 1);
  }
  return ret;
}

/**
  * @brief  Configure the interrupt 2 threshold sign.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         enable sign and axis for interrupt on threshold
  *
  */
int32_t lis331dlh_int2_on_threshold_conf_get(stmdev_ctx_t *ctx,
                                              int2_on_th_conf_t *val)
{
  lis331dlh_int2_cfg_t int2_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_CFG, (uint8_t*)&int2_cfg, 1);
  val->int2_xlie = int2_cfg.xlie;
  val->int2_xhie = int2_cfg.xhie;
  val->int2_ylie = int2_cfg.ylie;
  val->int2_yhie = int2_cfg.yhie;
  val->int2_zlie = int2_cfg.zlie;
  val->int2_zhie = int2_cfg.zhie;

  return ret;
}

/**
  * @brief  AND/OR combination of Interrupt 2 events.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of aoi in reg INT2_CFG
  *
  */
int32_t lis331dlh_int2_on_threshold_mode_set(stmdev_ctx_t *ctx,
                                             lis331dlh_int2_aoi_t val)
{
  lis331dlh_int2_cfg_t int2_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_CFG, (uint8_t*)&int2_cfg, 1);
  if(ret == 0) {
    int2_cfg.aoi = (uint8_t) val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT2_CFG,
                              (uint8_t*)&int2_cfg, 1);
  }
  return ret;
}

/**
  * @brief   AND/OR combination of Interrupt 2 events.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of aoi in reg INT2_CFG
  *
  */
int32_t lis331dlh_int2_on_threshold_mode_get(stmdev_ctx_t *ctx,
                                             lis331dlh_int2_aoi_t *val)
{
  lis331dlh_int2_cfg_t int2_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_CFG, (uint8_t*)&int2_cfg, 1);

  switch ( int2_cfg.aoi )
  {
    case LIS331DLH_INT2_ON_THRESHOLD_OR:
      *val = LIS331DLH_INT2_ON_THRESHOLD_OR;
      break;
    case LIS331DLH_INT2_ON_THRESHOLD_AND:
      *val = LIS331DLH_INT2_ON_THRESHOLD_AND;
      break;
    default:
      *val = LIS331DLH_INT2_ON_THRESHOLD_OR;
      break;
  }

  return ret;
}

/**
  * @brief  Interrupt generator 1 on threshold source register.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         registers INT2_SRC
  *
  */
int32_t lis331dlh_int2_src_get(stmdev_ctx_t *ctx,
                               lis331dlh_int2_src_t *val)
{
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_SRC, (uint8_t*) val, 1);
  return ret;
}

/**
  * @brief  Interrupt 2 threshold.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT2_THS
  *
  */
int32_t lis331dlh_int2_treshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_int2_ths_t int2_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_THS, (uint8_t*)&int2_ths, 1);
  if(ret == 0) {
    int2_ths.ths = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT2_THS,
                              (uint8_t*)&int2_ths, 1);
  }
  return ret;
}

/**
  * @brief  Interrupt 2 threshold.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT2_THS
  *
  */
int32_t lis331dlh_int2_treshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_int2_ths_t int2_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_THS, (uint8_t*)&int2_ths, 1);
  *val = int2_ths.ths;

  return ret;
}

/**
  * @brief  Duration value for interrupt 2 generator.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of d in reg INT2_DURATION
  *
  */
int32_t lis331dlh_int2_dur_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_int2_duration_t int2_duration;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_DURATION,
                           (uint8_t*)&int2_duration, 1);
  if(ret == 0) {
    int2_duration.d = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT2_DURATION,
                              (uint8_t*)&int2_duration, 1);
  }
  return ret;
}

/**
  * @brief    Duration value for interrupt 2 generator.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of d in reg INT2_DURATION
  *
  */
int32_t lis331dlh_int2_dur_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_int2_duration_t int2_duration;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_DURATION,
                           (uint8_t*)&int2_duration, 1);
  *val = int2_duration.d;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LIS331DLH_Wake_Up_Event
  * @brief       This section groups all the functions that manage the
  *              Wake Up event generation.
  * @{
  *
  */

/**
  * @brief  Turn-on mode selection for sleep to wake function.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of turnon in reg CTRL_REG5
  *
  */
int32_t lis331dlh_wkup_to_sleep_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG5, (uint8_t*)&ctrl_reg5, 1);
  if(ret == 0) {
    ctrl_reg5.turnon = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_CTRL_REG5,
                              (uint8_t*)&ctrl_reg5, 1);
  }
  return ret;
}

/**
  * @brief  Turn-on mode selection for sleep to wake function.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of turnon in reg CTRL_REG5
  *
  */
int32_t lis331dlh_wkup_to_sleep_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_ctrl_reg5_t ctrl_reg5;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_CTRL_REG5, (uint8_t*)&ctrl_reg5, 1);
  *val = ctrl_reg5.turnon;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @defgroup    LIS331DLH_Six_Position_Detection
  * @brief       This section groups all the functions concerning six
  *              position detection (6D).
  * @{
  *
  */

/**
  * @brief  Configure the 6d on interrupt 1 generator.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of 6d in reg INT1_CFG
  *
  */
int32_t lis331dlh_int1_6d_mode_set(stmdev_ctx_t *ctx,
                                   lis331dlh_int1_6d_t val)
{
  lis331dlh_int1_cfg_t int1_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);
  if(ret == 0) {
    int1_cfg._6d = (uint8_t)val & 0x01U;
    int1_cfg.aoi = ((uint8_t)val & 0x02U) >> 1;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);
  }
  return ret;
}

/**
  * @brief  Configure the 6d on interrupt 1 generator.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of 6d in reg INT1_CFG
  *
  */
int32_t lis331dlh_int1_6d_mode_get(stmdev_ctx_t *ctx,
                                   lis331dlh_int1_6d_t *val)
{
  lis331dlh_int1_cfg_t int1_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_CFG, (uint8_t*)&int1_cfg, 1);

  switch ((int1_cfg.aoi << 1) + int1_cfg._6d)
  {
    case LIS331DLH_6D_INT1_DISABLE:
      *val = LIS331DLH_6D_INT1_DISABLE;
      break;
    case LIS331DLH_6D_INT1_MOVEMENT:
      *val = LIS331DLH_6D_INT1_MOVEMENT;
      break;
     case LIS331DLH_6D_INT1_POSITION:
      *val = LIS331DLH_6D_INT1_POSITION;
      break;
    default:
      *val = LIS331DLH_6D_INT1_DISABLE;
      break;
  }

  return ret;
}

/**
  * @brief  6D on interrupt generator 1 source register.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         registers INT1_SRC
  *
  */
int32_t lis331dlh_int1_6d_src_get(stmdev_ctx_t *ctx,
                                  lis331dlh_int1_src_t *val)
{
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_SRC, (uint8_t*) val, 1);
  return ret;
}

/**
  * @brief  Interrupt 1 threshold.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT1_THS
  *
  */
int32_t lis331dlh_int1_6d_treshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_int1_ths_t int1_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_THS, (uint8_t*)&int1_ths, 1);
  if(ret == 0) {
    int1_ths.ths = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT1_THS, (uint8_t*)&int1_ths, 1);
  }
  return ret;
}

/**
  * @brief  Interrupt 1 threshold.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT1_THS
  *
  */
int32_t lis331dlh_int1_6d_treshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_int1_ths_t int1_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT1_THS, (uint8_t*)&int1_ths, 1);
  *val = int1_ths.ths;

  return ret;
}

/**
  * @brief  Configure the 6d on interrupt 2 generator.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of 6d in reg INT2_CFG
  *
  */
int32_t lis331dlh_int2_6d_mode_set(stmdev_ctx_t *ctx,
                                   lis331dlh_int2_6d_t val)
{
  lis331dlh_int2_cfg_t int2_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_CFG, (uint8_t*)&int2_cfg, 1);
  if(ret == 0) {
    int2_cfg._6d = (uint8_t)val & 0x01U;
    int2_cfg.aoi = ((uint8_t)val & 0x02U) >> 1;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT2_CFG,
                              (uint8_t*)&int2_cfg, 1);
  }
  return ret;
}

/**
  * @brief  Configure the 6d on interrupt 2 generator.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         Get the values of 6d in reg INT2_CFG
  *
  */
int32_t lis331dlh_int2_6d_mode_get(stmdev_ctx_t *ctx,
                                   lis331dlh_int2_6d_t *val)
{
  lis331dlh_int2_cfg_t int2_cfg;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_CFG, (uint8_t*)&int2_cfg, 1);

  switch ((int2_cfg.aoi << 1) + int2_cfg._6d)
  {
    case LIS331DLH_6D_INT2_DISABLE:
      *val = LIS331DLH_6D_INT2_DISABLE;
      break;
    case LIS331DLH_6D_INT2_MOVEMENT:
      *val = LIS331DLH_6D_INT2_MOVEMENT;
      break;
     case LIS331DLH_6D_INT2_POSITION:
      *val = LIS331DLH_6D_INT2_POSITION;
      break;
    default:
      *val = LIS331DLH_6D_INT2_DISABLE;
      break;
  }

  return ret;
}

/**
  * @brief  6D on interrupt generator 2 source register.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         registers INT2_SRC
  *
  */
int32_t lis331dlh_int2_6d_src_get(stmdev_ctx_t *ctx,
                                  lis331dlh_int2_src_t *val)
{
  int32_t ret;
  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_SRC, (uint8_t*) val, 1);
  return ret;
}

/**
  * @brief  Interrupt 2 threshold.[set]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT2_THS
  *
  */
int32_t lis331dlh_int2_6d_treshold_set(stmdev_ctx_t *ctx, uint8_t val)
{
  lis331dlh_int2_ths_t int2_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_THS, (uint8_t*)&int2_ths, 1);
  if(ret == 0) {
    int2_ths.ths = val;
    ret = lis331dlh_write_reg(ctx, LIS331DLH_INT2_THS,
                              (uint8_t*)&int2_ths, 1);
  }
  return ret;
}

/**
  * @brief  Interrupt 2 threshold.[get]
  *
  * @param  ctx         read / write interface definitions(ptr)
  * @param  val         change the values of ths in reg INT2_THS
  *
  */
int32_t lis331dlh_int2_6d_treshold_get(stmdev_ctx_t *ctx, uint8_t *val)
{
  lis331dlh_int2_ths_t int2_ths;
  int32_t ret;

  ret = lis331dlh_read_reg(ctx, LIS331DLH_INT2_THS, (uint8_t*)&int2_ths, 1);
  *val = int2_ths.ths;

  return ret;
}

/**
  * @}
  *
  */

/**
  * @}
  *
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
