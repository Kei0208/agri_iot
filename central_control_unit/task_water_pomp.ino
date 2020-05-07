//水中ポンプ制御タスク
void water_pomp_task(void *param){
  const uint8_t *data_pin_p = uint8_t_data_pin;   //データピンの制御用デジタルピンの状態を保持する変数へのポインタ
  uint8_t *cout_state_p = uint8_t_cout_state;     //COUTピンの状態を保持する変数へのポインタ

  //通知内容を記憶する変数
  uint32_t uint32_t_pomp_state = 0;

  //タスクは無限ループにする
  while(true){
    //水道管制御タスクから通知を受けるまで待機する
    xTaskNotifyWait(0, 0x03, &uint32_t_pomp_state, portMAX_DELAY);

    //通知された内容によって処理を分岐する
    if(uint32_t_pomp_state == 1){
      //水中ポンプを作動させる
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 2 ; uint8_t_loop++){
        *(cout_state_p + uint8_t_loop) = *(cout_state_p + uint8_t_loop) & 0;
      }
    }else if(uint32_t_pomp_state == 2){
      //水中ポンプを停止させる
      for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 2 ; uint8_t_loop++){
        *(cout_state_p + uint8_t_loop) = *(cout_state_p + uint8_t_loop) | 1;
      }
    }

    //変更を反映させる
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
      digitalWrite(*(data_pin_p + uint8_t_loop), *(cout_state_p + uint8_t_loop));
    }
    digitalWrite(CCLK_CTRL_PIN, LOW);
    vTaskDelay(1);
    digitalWrite(CCLK_CTRL_PIN, HIGH);
  }
}
