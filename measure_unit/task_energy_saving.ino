//省エネタスク
void energy_saving_task(void *param){  
  //タスクは無限ループにする
  while(true){
    //通信タスクから通知を受けるまで待機する
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

    //ディープスリープモードに移行する
    esp_sleep_enable_timer_wakeup(SLEEP_TIME);
    esp_deep_sleep_start();    
  }
}
