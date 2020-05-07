//通信タスク
void communication_task(void *param){
  //UDPソケット通信インスタンス
  WiFiUDP udp;
  
  //UDP通信を開始する
  udp.begin(MY_PORT);
  udp.flush();

  //受信用バッファ
  char char_buf[UDP_BUFFER_SIZE];
  char *b_p = char_buf;

  //タスクは無限ループにする
  while(true){
    //UDPバッファに受信データがあるか確認する
    uint32_t uint32_t_size_packet = udp.parsePacket();

    //受信データがあり、受信バッファサイズを超えていない場合に取り込む
    if(uint32_t_size_packet && (uint32_t_size_packet <= UDP_BUFFER_SIZE)){
      udp.read(char_buf, uint32_t_size_packet);    //受信バッファに受信サイズぶんのデータを取り込む
      char_buf[uint32_t_size_packet] = '\0';       //NULL終端処理

      //受信バッファの1文字目のデータで処理を分岐する
      switch(char_buf[0]){
        //計測データを受信した場合の処理
        case '0':
          for(uint8_t uint8_t_loop = 1 ; uint8_t_loop < uint32_t_size_packet ; uint8_t_loop++){
            xQueueSend(http_queue, &char_buf[uint8_t_loop], WAIT_TIME);
          }
          //キューにデータの格納が完了したことをHTTPアクセスタスクに通知する
          xTaskNotify(taskHandle[4], uint32_t_size_packet, eSetBits);
          break;
          
        //「注水開始」，「注水停止」，「排水開始」，「排水停止」を受信した場合の処理
        case '1':   //「注水開始」
        case '2':   //「排水開始」
        case '3':   //「注水停止」
        case '4':   //「排水停止」
          for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 17 ; uint8_t_loop++){
            xQueueSend(pipe_queue, &char_buf[uint8_t_loop], WAIT_TIME);
          }
          //キューにデータの格納が完了したことを水道管制御タスクに通知する
          xTaskNotify(taskHandle[3], NULL, eNoAction);
          break;
          
        //植物LED制御を受信した場合の処理
        case '5':
          for(uint8_t uint8_t_loop = 1 ; uint8_t_loop < 3 ; uint8_t_loop++){
            xQueueSend(plant_queue, &char_buf[uint8_t_loop], WAIT_TIME);
          }
          //キューにデータの格納が完了したことを水道管制御タスクに通知する
          xTaskNotify(taskHandle[1], NULL, eNoAction);
          break;

        //ESPの遠隔リセット処理
        case '9':
          ESP.restart();
          break;
          
        //予期せぬデータを受信した場合の処理
        default:
          Serial.println("ERROR!!");
          break;        
      }    
    udp.flush();    //UDPバッファのデータを破棄する
    }
  }
}
