//水道管制御タスク
void water_pipe_task(void *param){
  //水道管電磁リレーの状態を保持する（1：稼働中，0：停止中）
  uint32_t uint32_t_sort_pipe_state[] = {
    1, 1, 1, 1, 1, 1, 1, 1,  /*G0,G1,G2,G3,G4,G5,G6,G7,G8*/
    1, 1, 1, 1, 1, 1, 1, 1   /*F0,F1,F2,F3,F4,F5,F6,F7,F8*/
  };
  char char_pipe_state[17] = "";

  char char_temp[2];    //ソート時の一時退避用変数
  char char_order[2];   //命令種別格納用変数
  
  uint32_t *pipe_state_p = uint32_t_sort_pipe_state;   //水道管電磁リレーの状態を保持する変数へのポインタ
  const uint8_t *data_pin_p = uint8_t_data_pin;        //デジタルピンの番号を格納する変数へのポインタ

  //タスクは無限ループにする
  while(true){
    //通信タスクから通知を受けるまで待機する
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    
    //キューから水道管を制御するデータを取り出す
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 17 ; uint8_t_loop++){
      if(uint8_t_loop == 0){
        //キューの先頭にある命令種別は別の領域に格納する
        xQueueReceive(pipe_queue, &char_order, WAIT_TIME);
      }else{
        xQueueReceive(pipe_queue, &char_pipe_state[uint8_t_loop-1], WAIT_TIME);
      }
    }

    //キューから取り出したデータをchar型からuint32_t型に変換する
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 16 ; uint8_t_loop++){
      sprintf(char_temp, "%c", char_pipe_state[uint8_t_loop]);
      uint32_t_sort_pipe_state[uint8_t_loop] = atoi(char_temp);
    }
    
    //「注水開始」または「排水開始」の通知を受けた
    if(char_order[0] == STRAT_WATER_INJECTION || char_order[0] == STRAT_WATER_DRAIN){
      
      //GOUTピン（電磁リレー）を制御する
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        digitalWrite(*(data_pin_p + uint8_t_loop), *(pipe_state_p + uint8_t_loop));
      }
      digitalWrite(GCLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(GCLK_CTRL_PIN, HIGH); 

      //FOUTピン（電磁リレー）を制御する
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        digitalWrite(*(data_pin_p + uint8_t_loop), *(pipe_state_p + uint8_t_loop + 8));
      }
      digitalWrite(FCLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(FCLK_CTRL_PIN, HIGH); 

      //キューから取り出したデータをchar型からuint32_t型に変換するとともに
      //LED点灯用の並びにソートする（ハードウェア側の制約があるため）
      sprintf(char_temp, "%c", char_pipe_state[6]);
      uint32_t_sort_pipe_state[0] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[7]);
      uint32_t_sort_pipe_state[1] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[4]);
      uint32_t_sort_pipe_state[2] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[5]);
      uint32_t_sort_pipe_state[3] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[2]);
      uint32_t_sort_pipe_state[4] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[3]);
      uint32_t_sort_pipe_state[5] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[0]);
      uint32_t_sort_pipe_state[6] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[1]);
      uint32_t_sort_pipe_state[7] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[14]);
      uint32_t_sort_pipe_state[8] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[15]);
      uint32_t_sort_pipe_state[9] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[12]);
      uint32_t_sort_pipe_state[10] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[13]);
      uint32_t_sort_pipe_state[11] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[10]);
      uint32_t_sort_pipe_state[12] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[11]);
      uint32_t_sort_pipe_state[13] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[8]);
      uint32_t_sort_pipe_state[14] = atoi(char_temp);
      sprintf(char_temp, "%c", char_pipe_state[9]);
      uint32_t_sort_pipe_state[15] = atoi(char_temp);

      //DOUTピン（LED）を制御する
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        digitalWrite(*(data_pin_p + uint8_t_loop), *(pipe_state_p + uint8_t_loop));
      }
      digitalWrite(DCLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(DCLK_CTRL_PIN, HIGH); 

      //EOUTピン（LED）を制御する
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        digitalWrite(*(data_pin_p + uint8_t_loop), *(pipe_state_p + uint8_t_loop + 8));
      }
      digitalWrite(ECLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(ECLK_CTRL_PIN, HIGH); 
      
      //「注水開始」を水中ポンプ制御タスクに通知する
      if(char_order[0] == STRAT_WATER_INJECTION){
        xTaskNotify(taskHandle[2], 0x01, eSetBits);
      }

    //「注水停止」または「排水停止」の通知を受けた
    }else if(char_order[0] == STOP_WATER_INJECTION || char_order[0] == STOP_WATER_DRAIN){
      
      //「注水停止」を水中ポンプ制御タスクに通知する
      if(char_order[0] == STOP_WATER_INJECTION){
        xTaskNotify(taskHandle[2], 0x02, eSetBits);
        vTaskDelay(30);
      }

      //電磁リレーを全て閉じるとともに、LEDを全て消灯する
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
        digitalWrite(*(data_pin_p + uint8_t_loop), HIGH);
      }
      digitalWrite(GCLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(GCLK_CTRL_PIN, HIGH);
      vTaskDelay(1);
      digitalWrite(FCLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(FCLK_CTRL_PIN, HIGH);
      vTaskDelay(1);
      digitalWrite(DCLK_CTRL_PIN, LOW);
      vTaskDelay(1);
      digitalWrite(DCLK_CTRL_PIN, HIGH); 
      vTaskDelay(1);
      digitalWrite(ECLK_CTRL_PIN, LOW);    
      vTaskDelay(1);
      digitalWrite(ECLK_CTRL_PIN, HIGH);      
    }
  }
}
