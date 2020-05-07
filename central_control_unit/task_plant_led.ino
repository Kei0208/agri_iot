//植物用LED制御タスク
void plant_led_task(void *param)
{
  boolean boolean_judgment_flag = true;      //点消灯判定フラグ
  boolean boolean_first_time_flag = true;    //初回判定フラグ
  
  uint32_t uint32_t_plant_power[] = {1, 1};                                  //{制御対象ユニット番号, 点消灯指示}
  uint32_t uint32_t_last_time_state[] = {1, 1, 1, 1, 1, 1, 1, 1};            //前回の変更内容を記憶する変数
  const uint32_t uint32_t_plant_led_state_on1[] = {0, 1, 1, 1, 1, 1, 1, 0};  //1番ユニット
  const uint32_t uint32_t_plant_led_state_on2[] = {1, 0, 1, 1, 1, 1, 0, 1};  //2番ユニット
  const uint32_t uint32_t_plant_led_state_on3[] = {1, 1, 0, 1, 1, 0, 1, 1};  //3番ユニット
  const uint32_t uint32_t_plant_led_state_on4[] = {1, 1, 1, 0, 0, 1, 1, 1};  //4番ユニット

  uint32_t *last_time_p = uint32_t_last_time_state;   //前回の変更内容を記憶する変数へのポインタ
  uint32_t *bout_state_p = uint32_t_bout_state;       //植物LEDユニットの状態を保持する変数へのポインタ
  const uint32_t *change_state_p;                     //植物LEDユニットの状態を変更する変数へのポインタ
  const uint8_t *data_pin_p = uint8_t_data_pin;       //デジタルピンの番号を格納する変数へのポインタ
  
  char char_temp[2];              //型変換時の一時退避用変数
  char char_plant_state[3] = "";  //キューのデータを格納する変数

  //タスクは無限ループにする
  while(true){
    //通信タスクから通知を受けるまで待機する
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    //キューから植物LEDユニットを制御するデータを取り出す
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 2 ; uint8_t_loop++){
      xQueueReceive(plant_queue, &char_plant_state[uint8_t_loop], WAIT_TIME);
    }

    //キューから取り出したデータをchar型からuint32_t型に変換する
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 2 ; uint8_t_loop++){
      sprintf(char_temp, "%c", char_plant_state[uint8_t_loop]);
      uint32_t_plant_power[uint8_t_loop] = atoi(char_temp);
    }    

    //制御対象を決定する
    switch(uint32_t_plant_power[0]){
      case 1:
        change_state_p = uint32_t_plant_led_state_on1;
        break;
      
      case 2:
        change_state_p = uint32_t_plant_led_state_on2;
        break;
      
      case 3:
        change_state_p = uint32_t_plant_led_state_on3;
        break;
      
      case 4:
        change_state_p = uint32_t_plant_led_state_on4;
        break;
      
      default:
        Serial.println("PLANT_ORDER_ERROR!!");
        uint32_t_plant_power[1] = 9;
    }

    //2ビット目のデータによって点消灯を判断する（1：点灯，0：消灯）
    if(uint32_t_plant_power[1] == 1){
      //制御対象の植物LEDユニットを点灯させる
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        *(bout_state_p + uint8_t_loop) = *(bout_state_p + uint8_t_loop) & *(change_state_p + uint8_t_loop);
      }
      boolean_judgment_flag = false;      //点消灯判定フラグを折る
      boolean_first_time_flag = false;    //初回判定フラグを折る

    //2ビット目が消灯指示かつ初回以外の点灯指示の場合に入る
    }else if(uint32_t_plant_power[1] == 0 && boolean_first_time_flag != true){
      //前回と今回の変更内容を比較し、異なる場合は点消灯判定フラグを折る
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        if((*(last_time_p + uint8_t_loop)) != (*(change_state_p + uint8_t_loop))){
          boolean_judgment_flag = false;
          break;
        }
      }

      if(boolean_judgment_flag){
        for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
          *(bout_state_p + uint8_t_loop) = *(bout_state_p + uint8_t_loop) & 1;
        }
      }else{
        for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
          *(bout_state_p + uint8_t_loop) = !(*(bout_state_p + uint8_t_loop) ^ *(change_state_p + uint8_t_loop));
        }
        boolean_judgment_flag = true;
      }

      //今回変更した内容を記憶する
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        *(last_time_p + uint8_t_loop) = *(change_state_p + uint8_t_loop);
      }      
    }

    //変更を反映させる
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
      digitalWrite(*(data_pin_p + uint8_t_loop), *(bout_state_p + uint8_t_loop));
    }
    digitalWrite(BCLK_CTRL_PIN, HIGH);
    delay(1);
    digitalWrite(BCLK_CTRL_PIN, LOW);
  }
}
