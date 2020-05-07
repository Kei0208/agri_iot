#ifndef INCLUDED_measure_h_
#define INCLUDED_measure_h_
  
/*-------ESP-WROOM-32入出力GPIOピン-------*/
#define W_T_READ_PIN   16
#define RED_LED_PIN    19
#define GREEN_LED_PIN  18
#define BLUE_LED_PIN   23
#define SDA_PIN        21
#define SCL_PIN        22
#define W_L_READ_PIN1  4
#define W_L_READ_PIN2  13
#define W_L_READ_PIN3  14
#define W_L_READ_PIN4  27
#define W_L_READ_PIN5  26
#define W_L_READ_PIN6  25
#define W_L_READ_PIN7  33
#define W_L_READ_PIN8  32
#define W_L_READ_PIN9  35
#define W_L_READ_PIN10 34

/*-------ハードウェアUARTボーレート-------*/
#define UART_SPEED 9600

/*-------ネットワーク設定-------*/
#define ESSID         "hama_net"
#define PASSWORD      "hama_system"
#define MY_PORT       7000
#define OPPONENT_PORT 7000

/*-------SPIFFSファイル-------*/
#define FILE_NAME "/node_number.txt"

/*-------タスク設定-------*/
#define STACK_SIZE 8192
#define USE_CORE      1
#define MEASURE_TASK_LEVEL       2
#define COMMUNICATION_TASK_LEVEL 3
#define ENERGY_SAVING_TASK_LEVEL 4

/*-------キュー設定-------*/
#define MEASURE_QUEUE_SIZE 15
#define W_L_QUEUE_SIZE     12
#define WAIT_TIME           0

/*-------フルカラーLED色彩設定-------*/
#define RED    1
#define GREEN  2
#define BLUE   3
#define PURPLE 4
#define YELLOW 5
#define SKY    6
#define WHITE  7
#define OFF    0

/*-------水温センサ測定ビット設定-------*/
#define MEASUREMENT_MODE 9

/*-------気温，湿度，大気圧センサのI2Cアドレス-------*/
#define BME_ADDRESS 0x76

/*-------エラー発生時のループタスク優先度設定-------*/
#define ERROR_SET_PRIORITY 9

/*-------ディープスリープモードからの復帰時間[μs]-------*/
#define SLEEP_TIME 900000000

#endif
