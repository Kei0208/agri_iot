//通信タスク
void communication_task(void *param){

  String string_send_data = "0";
  const IPAddress central_control_unit_ip(192, 168, 1, 100);        //中央制御ユニットのIPアドレス

  //水位レベルを格納する変数と参照用ポインタ
  uint32_t uint32_t_measure_water_level[]{
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
  };
  uint32_t *measure_level_p = uint32_t_measure_water_level;

  //水温情報を格納する変数と参照用ポインタ
  float float_measure_water_temp[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  };
  float *measure_temp_p = float_measure_water_temp;

  float float_air_temp = 0;   //気温情報を格納する変数
  float float_air_humi = 0;   //湿度情報を格納する変数
  float float_air_pres = 0;   //大気圧情報を格納する変数
  
  //UDPソケット通信インスタンス
  WiFiUDP udp;
  
  //UDP通信を開始する
  udp.begin(MY_PORT);
  udp.flush();

  
  //タスクは無限ループにする
  while(true){
    //計測タスクから通知を受けるまで待機する
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    //キューから計測した水温データ，水位レベルデータ，気温データ，湿度データ，気圧データを取り出す
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      xQueueReceive(w_l_queue, &uint32_t_measure_water_level[uint8_t_loop], WAIT_TIME);
      xQueueReceive(measure_queue, &float_measure_water_temp[uint8_t_loop], WAIT_TIME);
    }
    xQueueReceive(measure_queue, &float_air_temp, WAIT_TIME);
    xQueueReceive(measure_queue, &float_air_humi, WAIT_TIME);
    xQueueReceive(measure_queue, &float_air_pres, WAIT_TIME);

    //送信データを組み立てる（水位レベル）
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      string_send_data.concat(*(measure_level_p + uint8_t_loop));
      string_send_data.concat(",");
    }

    //送信データを組み立てる（水温）
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      string_send_data.concat(*(measure_temp_p + uint8_t_loop));
      string_send_data.concat(",");
    }

    //送信データを組み立てる（気温，湿度，気圧）
    string_send_data.concat(float_air_temp);
    string_send_data.concat(",");
    string_send_data.concat(float_air_humi);
    string_send_data.concat(",");
    string_send_data.concat(float_air_pres);

    //データ送信先を設定する
    udp.beginPacket(central_control_unit_ip, OPPONENT_PORT);

    //送信するデータを設定する
    udp.print(string_send_data);

    //設定したデータを送信する
    udp.endPacket();  
    udp.flush();    //UDPバッファのデータを破棄する
    vTaskDelay(3000);

    //UDP通信が完了したことを省エネタスクに通知する
    xTaskNotify(taskHandle[1], NULL, eNoAction);
  }
}
