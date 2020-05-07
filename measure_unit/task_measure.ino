//計測タスク
void measure_task(void *param){

  //水温センサを識別するアドレス情報と参照用ポインタ
  DeviceAddress DA_sensor_address[] = {
    {0x28,0xFF,0x5D,0x54,0xB0,0x16,0x04,0x7C},
    {0x28,0xFF,0xEE,0x21,0xB0,0x16,0x04,0x54},
    {0x28,0xFF,0x7F,0x0E,0x00,0x17,0x03,0x3F},
    {},
    {},
    {},
    {},
    {},  
    {},
    {}  
  };
  DeviceAddress *da_address_p = DA_sensor_address;

  //水温情報を格納する変数と参照用ポインタ
  float float_water_temp[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  };
  float *water_temp = float_water_temp;

  //水位レベルを計測するデジタルピン情報と参照用ポインタ
  uint8_t uint8_t_water_level_pin[] = {
    W_L_READ_PIN1, W_L_READ_PIN2, W_L_READ_PIN3,
    W_L_READ_PIN4, W_L_READ_PIN5, W_L_READ_PIN6,
    W_L_READ_PIN7, W_L_READ_PIN8, W_L_READ_PIN9,
    W_L_READ_PIN10
  };
  uint8_t *w_l_pin_p = uint8_t_water_level_pin;

  //水位レベルを格納する変数と参照用ポインタ
  uint32_t uint32_t_measure_data[]{
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
  };
  uint32_t *measure_p = uint32_t_measure_data;

  float float_air_temp = 0;   //気温情報を格納する変数
  float float_air_humi = 0;   //湿度情報を格納する変数
  float float_air_pres = 0;   //大気圧情報を格納する変数

  //測定用インスタンス
  OneWire oneWire(W_T_READ_PIN);                            //水温計測インスタンス
  DallasTemperature DallasTemperature_sensors(&oneWire);    //水温保持用
  Adafruit_BME280 Adafruit_BME280_bme;                      //気温，湿度，大気圧計測インスタンス

  //I2C通信を開始する
  Wire.begin(SDA_PIN, SCL_PIN);

  //BME280と通信を開始する
  Adafruit_BME280_bme.begin(BME_ADDRESS);

  //9ビットモードで水温を測定するように設定する
  DallasTemperature_sensors.setResolution(MEASUREMENT_MODE);
  DallasTemperature_sensors.begin();
  
  //タスクは無限ループにする
  while(true){
    
    //水温取得要求
    DallasTemperature_sensors.requestTemperatures();
    vTaskDelay(300);
    //水温[℃]を記憶する
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      *(water_temp + uint8_t_loop) = DallasTemperature_sensors.getTempC(*(da_address_p + uint8_t_loop));
    }

    //気温，湿度，気圧を測定する
    float_air_temp = Adafruit_BME280_bme.readTemperature();         //BME280から気温[℃]を読み取る
    float_air_humi = Adafruit_BME280_bme.readHumidity();            //BME280から湿度[%]を読み取る
    float_air_pres = Adafruit_BME280_bme.readPressure() / 100.0F;   //BME280から大気圧[hPa]を読み取る

    //水位レベルを測定する
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      *(measure_p + uint8_t_loop) = analogRead(*(w_l_pin_p + uint8_t_loop));
    }

    //計測データをキューに格納する
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      xQueueSend(w_l_queue, &uint32_t_measure_data[uint8_t_loop], WAIT_TIME);
      xQueueSend(measure_queue, &float_water_temp[uint8_t_loop], WAIT_TIME);
    }
    xQueueSend(measure_queue, &float_air_temp, WAIT_TIME);
    xQueueSend(measure_queue, &float_air_humi, WAIT_TIME);
    xQueueSend(measure_queue, &float_air_pres, WAIT_TIME);
        
    //キューにデータの格納が完了したことを通信タスクに通知する
    xTaskNotify(taskHandle[2], NULL, eNoAction);

    while(true){}
  }
}
