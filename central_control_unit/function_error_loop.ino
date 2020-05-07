//エラーループシーケンス
void error_loop(){
  //フルカラーLEDを赤点灯して無限ループに入る
  control_led(RED);

  //ループタスクの優先度をMAXまで引き上げ、他のタスクを実行させない
  vTaskPrioritySet(NULL, ERROR_SET_PRIORITY);
  while(true){}
  return;
}
