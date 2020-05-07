#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include <FS.h>
#include <SPIFFS.h>
#include "central.h"

QueueHandle_t plant_queue, pipe_queue, http_queue;  //キューハンドル
TaskHandle_t taskHandle[5];                         //タスクハンドル
extern TaskHandle_t loopTaskHandle;                 //ループタスクハンドル

//関数に関するプロトタイプ宣言
extern void wifi_connection(void); //WiFi接続シーケンス
extern void control_led(uint8_t);  //フルカラーLED制御シーケンス
extern void error_loop(void);      //エラーループシーケンス

//タスクに関するプロトタイプ宣言
extern void communication_task(void*);
extern void plant_led_task(void*);
extern void water_pipe_task(void*);
extern void water_pomp_task(void*);
extern void http_access_task(void*);

//データピンの制御用デジタルピン
const uint8_t uint8_t_data_pin[8] = {
  D1_CTRL_PIN, D2_CTRL_PIN, D3_CTRL_PIN, D4_CTRL_PIN,
  D5_CTRL_PIN, D6_CTRL_PIN, D7_CTRL_PIN, D8_CTRL_PIN
};

//クロックピンの制御用デジタルピン
const uint8_t uint8_t_clk_pin[8] = {
  ACLK1_CTRL_PIN, ACLK2_CTRL_PIN, BCLK_CTRL_PIN, CCLK_CTRL_PIN,
  DCLK_CTRL_PIN , ECLK_CTRL_PIN , FCLK_CTRL_PIN, GCLK_CTRL_PIN
};

uint32_t uint32_t_bout_state[] = {1, 1, 1, 1, 1, 1, 1, 1};  /*BOUTピンの状態　B0,B1,B2,B3,B4,B5,B6,B7*/
uint8_t uint8_t_cout_state[]   = {1, 1, 1, 1, 1, 1, 1, 1};  /*COUTピンの状態　C0,C1,C2,C3,C4,C5,C6,C7*/

//ループタスク
void setup() {
  //ハードウェアUARTを開始する
  Serial.begin(UART_SPEED);

  //各デジタルピンの割当てと初期化
  for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 8 ; uint8_t_loop++){
    pinMode(uint8_t_data_pin[uint8_t_loop], OUTPUT);
    pinMode(uint8_t_clk_pin[uint8_t_loop], OUTPUT);
    digitalWrite(uint8_t_data_pin[uint8_t_loop], HIGH);
    digitalWrite(uint8_t_clk_pin[uint8_t_loop], HIGH);
  }

  //WiFi接続シーケンスを起動する
  wifi_connection();

  //キューを生成する
  plant_queue = xQueueCreate(PLANT_QUEUE_SIZE, sizeof(char));
  pipe_queue  = xQueueCreate(PIPE_QUEUE_SIZE, sizeof(char));
  http_queue  = xQueueCreate(HTTP_QUEUE_SIZE, sizeof(char));

  //plant_queueの生成が正常に生成されたかチェックする
  if(plant_queue == NULL){
    Serial.println("CREATE PLANT_QUEUE FAILED!!");
    error_loop();
  }

  //pipe_queueの生成が正常に生成されたかチェックする
  if(pipe_queue == NULL){
    Serial.println("CREATE PIPE_QUEUE FAILED!!");
    error_loop();
  }

  //http_queueの生成が正常に生成されたかチェックする
  if(http_queue == NULL){
    Serial.println("CREATE HTTP_QUEUE FAILED!!");
    error_loop();
  }

  //各タスクを生成する（ループタスクの優先度は「1」）
  //植物用LED制御タスク
  if((xTaskCreatePinnedToCore(plant_led_task, "PLANT_LED", TASK_STACK_SIZE_1, NULL, PLANT_LED_TASK_LEVEL, &taskHandle[1], USE_CORE_1)) != pdPASS){
    Serial.println("CREATE PLANT_LED_TASK FAILED!!");
    error_loop();
  }
  //水中ポンプ制御タスク
  if((xTaskCreatePinnedToCore(water_pomp_task, "WATER_POMP", TASK_STACK_SIZE_1, NULL, WATER_POMP_TASK_LEVEL, &taskHandle[2], USE_CORE_1)) != pdPASS){
    Serial.println("CREATE WATER_POMP_TASK FAILED!!");
    error_loop();
  }
  //水道管制御タスク
  if((xTaskCreatePinnedToCore(water_pipe_task, "WATER_PIPE", TASK_STACK_SIZE_1, NULL, WATER_PIPE_TASK_LEVEL, &taskHandle[3], USE_CORE_1)) != pdPASS){
    Serial.println("CREATE WATER_PIPE_TASK FAILED!!");
    error_loop();
  }
  //HTTP接続タスク
  if((xTaskCreatePinnedToCore(http_access_task, "HTTP_ACCESS", TASK_STACK_SIZE_2, NULL, HTTP_ACCESS_TASK_LEVEL, &taskHandle[4], USE_CORE_0)) != pdPASS){
    Serial.println("CREATE HTTP_ACCESS_TASK FAILED!!");
    error_loop();
  }
  //通信タスク
  if((xTaskCreatePinnedToCore(communication_task, "COMMUNICATION", TASK_STACK_SIZE_1, NULL, COMMUNICATION_TASK_LEVEL, &taskHandle[0], USE_CORE_1)) != pdPASS){
    Serial.println("CREATE COMMUNICATION_TASK FAILED!!");
    error_loop();
  }
  return;
}


//ループタスク
void loop() {
  while(true){}
}
