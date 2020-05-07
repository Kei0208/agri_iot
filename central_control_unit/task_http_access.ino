//HTTP通信シーケンス
void http_access_task(void *param){

  const IPAddress server_ip(192, 168, 1, 150);    //管理サーバ（RaspberryPi3）のIPアドレス
  const String string_data_pass = DATA_PASS;       //取得データを保存するPHPスクリプトのパス

  //HTTP通信を行うためのインスタンス
  HTTPClient http;
  
  char char_buf[2];                         //文字判別用バッファ

  char char_url[URL_SIZE];                  //接続先URL情報格納用変数
  char *url_p = char_url;                   //接続先URLポインタ

  char char_node_name[NODE_NAME_SIZE];      //ノード名情報格納用変数
  char *node_name_p = char_node_name;       //ノード名ポインタ

  char char_water_level[WATER_LEVEL_SIZE];  //水位情報格納用変数
  char *water_level_p = char_water_level;   //水位ポインタ

  char char_water_temp[WATER_TEMP_SIZE];    //水温情報格納用変数
  char *water_temp_p = char_water_temp;     //水温ポインタ

  char char_air_temp[AIR_TEMP_SIZE];        //気温情報格納用変数
  char *air_temp_p = char_air_temp;         //気温ポインタ

  char char_air_humi[AIR_HUMI_SIZE];        //湿度情報格納用変数
  char *air_humi_p = char_air_humi;         //湿度ポインタ

  char char_air_press[AIR_PRESS_SIZE];      //気圧情報格納用変数
  char *air_press_p = char_air_press;       //気圧ポインタ
  
  char char_measure_data[MEASURE_DATA_SIZE];    //測定データを格納する変数
  char *measure_p = char_measure_data;          //測定データポインタ
  
  uint32_t uint32_t_size_data = 0;    //測定データの長さを格納する変数
  int32_t int32_t_http_code = 0;      //HTTPステータスコードを格納する変数

  uint8_t uint8_t_loop1 = 0;    //node_name_p，measure_pの位置決め用変数
  uint8_t uint8_t_loop2 = 0;    //water_level_p，water_temp_p, air_temp_p, air_humi_p, air_press_pの位置決め用変数
  uint8_t uint8_t_count = 0;    //計測データ内の「,」カウント用変数

  //接続先URLを組み立てる
  String string_url = "http://";
  string_url.concat(server_ip.toString());
  string_url.concat(string_data_pass);
  string_url.toCharArray(url_p, URL_SIZE);


  //タスクは無限ループにする
  while(true){
    //ループタスクから通知を受けるまで待機する
    xTaskNotifyWait(0, 0, &uint32_t_size_data, portMAX_DELAY);

    //キューから測定データを取り出す
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < uint32_t_size_data ; uint8_t_loop++){
      xQueueReceive(http_queue, &char_measure_data[uint8_t_loop], WAIT_TIME);
      char_measure_data[uint8_t_loop + 1] = '\0';
    }

    //データ分割（水位）
    while(true){
      char_buf[0] = *(measure_p + uint8_t_loop1);
      if(char_buf[0] == ','){
        uint8_t_count++;
        
        //10個目の「,」を見つけた場合は、水位データの分割を終了する
        if(uint8_t_count == 10){
          uint8_t_loop1++;
          uint8_t_loop2 = 0;
          uint8_t_count = 0;
          break;
        }        
      }
      *(water_level_p + uint8_t_loop2) = char_buf[0];
      *(water_level_p + uint8_t_loop2 + 1) = '\0';
      uint8_t_loop1++;
      uint8_t_loop2++;
    }

    //データ分割（水温）
    while(true){
      char_buf[0] = *(measure_p + uint8_t_loop1);
      if(char_buf[0] == ','){
        uint8_t_count++;
        
        //10個目の「,」を見つけた場合は、水位データの分割を終了する
        if(uint8_t_count == 10){
          uint8_t_loop1++;
          uint8_t_loop2 = 0;
          uint8_t_count = 0;
          break;
        }        
      }
      *(water_temp_p + uint8_t_loop2) = char_buf[0];
      *(water_temp_p + uint8_t_loop2 + 1) = '\0';
      uint8_t_loop1++;
      uint8_t_loop2++;
    }

    //データ分割（気温）
    while(true){
      char_buf[0] = *(measure_p + uint8_t_loop1);
      if(char_buf[0] == ','){
        uint8_t_loop1++;
        uint8_t_loop2 = 0;
        break;
      }else{
        *(air_temp_p + uint8_t_loop2) = char_buf[0];
        *(air_temp_p + uint8_t_loop2 + 1) = '\0';
      }
      uint8_t_loop1++;
      uint8_t_loop2++;
    }

    //データ分割（湿度）
    while(true){
      char_buf[0] = *(measure_p + uint8_t_loop1);
      if(char_buf[0] == ','){
        uint8_t_loop1++;
        uint8_t_loop2 = 0;
        break;
      }else{
        *(air_humi_p + uint8_t_loop2) = char_buf[0];
        *(air_humi_p + uint8_t_loop2 + 1) = '\0';
      }
      uint8_t_loop1++;
      uint8_t_loop2++;
    }

    //データ分割（気圧）
    while(true){
      char_buf[0] = *(measure_p + uint8_t_loop1);
      if(char_buf[0] == ','){
        uint8_t_loop1 = 0;
        uint8_t_loop2 = 0;
        break;
      }else{
        *(air_press_p + uint8_t_loop2) = char_buf[0];
        *(air_press_p + uint8_t_loop2 + 1) = '\0';
      }
      uint8_t_loop1++;
      uint8_t_loop2++;
    }

    //ノードIDをテキストファイルから読み取る
    //ファイル入出力インタフェースを開始する
    SPIFFS.begin();
    File read_file = SPIFFS.open(FILE_NAME, "r");
    if(!read_file){
      Serial.println("SPIFFS OPEN FAILED!!");
      error_loop();
    }
    while(read_file.available()){
      *(node_name_p + uint8_t_loop1) = read_file.read();
      *(node_name_p + uint8_t_loop1 + 1) = '\0';
      uint8_t_loop1++;
    }
    uint8_t_loop1 = 0;
    read_file.close();
    SPIFFS.end();

    //HTTP通信の接続先を指定する
    http.begin(url_p);

    //HTTPヘッダ情報を追加する
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    //送信データの情報を組み立てる
    String string_post_param = "node_id=";
    string_post_param.concat(node_name_p);        //ノードID
    string_post_param.concat("&water_level=");
    string_post_param.concat(water_level_p);      //水位情報
    string_post_param.concat("&water_temp=");
    string_post_param.concat(water_temp_p);       //水温情報
    string_post_param.concat("&air_temp=");
    string_post_param.concat(air_temp_p);         //気温情報
    string_post_param.concat("&air_humi=");
    string_post_param.concat(air_humi_p);         //湿度情報
    string_post_param.concat("&air_press=");
    string_post_param.concat(air_press_p);        //気圧情報

    //POST通信を開始する
    int32_t_http_code = http.POST(string_post_param);

    //通信結果を目視報告する
    switch(int32_t_http_code){
      //通信が正常に終了した（緑色点灯）
      case 200:
        control_led(GREEN);
        Serial.println("SUCCESS!!");
        break;

      //PHPファイルへのアクセス権がなく処理が中断した（赤色点灯）
      case 403:
        control_led(RED);
        Serial.println("403 FORBIDDEN ERROR!!");
        break;

      //サーバダウンまたはPHPファイルがなかった（黄色点灯）
      case 404:
        control_led(YELLOW);
        Serial.println("404 NOT FOUND ERROR!!");
        break;

      //PHPファイルのプログラムが原因で処理が中断した（白色点灯）
      case 500:
        control_led(WHITE);
        Serial.println("500 INTERNAL SERVER ERROR!!");
        break;

      //アクセス過多による処理の制限を受け、処理が中断した（紫色点灯）
      case 503:
        control_led(PURPLE);
        Serial.println("503 SERVICE UNAVAILABLE ERROR!!");
        break;

      //その他のエラー
      default:
        control_led(OFF);
        Serial.print("ERROR CODE : ");
        Serial.println(int32_t_http_code);
        break;
    }
    vTaskDelay(1);
  
    //HTTP通信を行うインスタンスを解放する
    http.end();

    //各バッファのデータを消去する
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 10 ; uint8_t_loop++){
      *(air_temp_p + uint8_t_loop)   = '\0';
      *(air_humi_p + uint8_t_loop)   = '\0';
      *(air_press_p + uint8_t_loop)  = '\0';
    }
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < WATER_LEVEL_SIZE ; uint8_t_loop++){
      *(water_level_p + uint8_t_loop) = '\0';
    }
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < WATER_TEMP_SIZE ; uint8_t_loop++){
      *(water_temp_p + uint8_t_loop) = '\0';
    }
    for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < MEASURE_DATA_SIZE ; uint8_t_loop++){
      *(measure_p + uint8_t_loop) = '\0';
    }
  }
}
