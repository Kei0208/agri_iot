#ifndef INCLUDED_iot_h_
#define INCLUDED_iot_h_
  
/*-------ESP-WROOM-32入出力GPIOピン-------*/
#define D1_CTRL_PIN    32
#define D2_CTRL_PIN    33
#define D3_CTRL_PIN    25
#define D4_CTRL_PIN    26
#define D5_CTRL_PIN    27
#define D6_CTRL_PIN    14
#define D7_CTRL_PIN    12
#define D8_CTRL_PIN    13
#define ACLK1_CTRL_PIN  4
#define ACLK2_CTRL_PIN 16
#define BCLK_CTRL_PIN  17
#define CCLK_CTRL_PIN  18
#define DCLK_CTRL_PIN  19
#define ECLK_CTRL_PIN  21
#define FCLK_CTRL_PIN  22
#define GCLK_CTRL_PIN  23

/*-------ハードウェアUARTボーレート-------*/
#define UART_SPEED 9600

/*-------ネットワーク設定-------*/
#define ESSID     "hama_net"
#define PASSWORD  "hama_system"
#define DATA_PASS "/agri_iot.php"
#define MY_PORT       7000
#define OPPONENT_PORT 8000

/*-------SPIFFSファイル-------*/
#define FILE_NAME "/node_number.txt"

/*-------タスク設定-------*/
#define TASK_STACK_SIZE_1  8192
#define TASK_STACK_SIZE_2 16384
#define USE_CORE_0            0
#define USE_CORE_1            1
#define PLANT_LED_TASK_LEVEL     3
#define WATER_POMP_TASK_LEVEL    4
#define WATER_PIPE_TASK_LEVEL    5
#define HTTP_ACCESS_TASK_LEVEL   6
#define COMMUNICATION_TASK_LEVEL 2

/*-------キュー設定-------*/
#define PLANT_QUEUE_SIZE  10
#define PIPE_QUEUE_SIZE   20
#define HTTP_QUEUE_SIZE  155
#define WAIT_TIME          0

/*-------無線データ受信バッファサイズ設定-------*/
#define UDP_BUFFER_SIZE 155

/*-------水道管制御判定データ-------*/
#define STRAT_WATER_INJECTION '1'
#define STRAT_WATER_DRAIN     '2'
#define STOP_WATER_INJECTION  '3'
#define STOP_WATER_DRAIN      '4'

/*-------フルカラーLED色彩設定-------*/
#define RED    1
#define GREEN  2
#define BLUE   3
#define PURPLE 4
#define YELLOW 5
#define SKY    6
#define WHITE  7
#define OFF    0

/*-------HTTPデータ送信時の分割バッファサイズ設定-------*/
#define URL_SIZE           50
#define NODE_NAME_SIZE     30
#define WATER_LEVEL_SIZE   80
#define WATER_TEMP_SIZE    80
#define AIR_TEMP_SIZE      10
#define AIR_HUMI_SIZE      10
#define AIR_PRESS_SIZE     10
#define MEASURE_DATA_SIZE 155

/*-------エラー発生時のループタスク優先度設定-------*/
#define ERROR_SET_PRIORITY 9

//UDP_BUFFER_SIZE，HTTP_QUEUE_SIZE，MEASURE_DATA_SIZEは同じサイズにすること

#endif
