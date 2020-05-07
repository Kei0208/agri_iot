#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h> 
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "measure.h"

QueueHandle_t w_l_queue, measure_queue;     //キューハンドル
TaskHandle_t taskHandle[3];                 //タスクハンドル
extern TaskHandle_t loopTaskHandle;         //ループタスクハンドル

//関数に関するプロトタイプ宣言
extern void wifi_connection(void);  //WiFi接続シーケンス
extern void control_led(uint8_t);   //フルカラーLED制御シーケンス
extern void error_loop(void);       //エラーループシーケンス

//タスクに関するプロトタイプ宣言
extern void energy_saving_task(void*);
extern void measure_task(void*);
extern void communication_task(void*);

//フルカラーLEDのピンアレイ
uint8_t uint8_t_full_color_led_pin[] = {
  RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN
};


//ループタスク
void setup() {
  //ハードウェアUARTを開始する
  Serial.begin(UART_SPEED);

  //各デジタルピンの割当てと初期化
  for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 3 ; uint8_t_loop++){
    pinMode(uint8_t_full_color_led_pin[uint8_t_loop], OUTPUT);
    digitalWrite(uint8_t_full_color_led_pin[uint8_t_loop], LOW);
  }

  //WiFi接続シーケンスを起動する
  wifi_connection();

  //キューを生成する
  measure_queue = xQueueCreate(MEASURE_QUEUE_SIZE, sizeof(float));
  w_l_queue = xQueueCreate(W_L_QUEUE_SIZE, sizeof(uint32_t));

  //measure_queueの生成が正常に生成されたかチェックする
  if(measure_queue == NULL){
    Serial.println("CREATE MEASURE_QUEUE FAILED!!");
    error_loop();
  }

  //w_l_queueの生成が正常に生成されたかチェックする
  if(w_l_queue == NULL){
    Serial.println("CREATE W_L_QUEUE FAILED!!");
    error_loop();
  }

  //各タスクを生成する（ループタスクのタスク優先度は「1」）
  //計測タスク
  if((xTaskCreatePinnedToCore(measure_task, "MEASURE", STACK_SIZE, NULL, MEASURE_TASK_LEVEL, &taskHandle[0], USE_CORE)) != pdPASS){
    Serial.println("CREATE MEASURE_TASK FAILED!!");
    error_loop();
  }
  //省エネタスク
  if((xTaskCreatePinnedToCore(energy_saving_task, "ENERGY_SAVING", STACK_SIZE, NULL, ENERGY_SAVING_TASK_LEVEL, &taskHandle[1], USE_CORE)) != pdPASS){
    Serial.println("CREATE ENERGY_SAVING_TASK FAILED!!");
    error_loop();
  }
  //通信タスク
  if((xTaskCreatePinnedToCore(communication_task, "COMMUNICATION", STACK_SIZE, NULL, COMMUNICATION_TASK_LEVEL, &taskHandle[2], USE_CORE)) != pdPASS){
    Serial.println("CREATE COMMUNICATION_TASK FAILED!!");
    error_loop();
  }
  return;
}


//ループタスク
void loop() {
  while(true){}
}
