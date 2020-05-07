//WiFi接続シーケンス
void wifi_connection(){

  const char *char_ssid = ESSID;                  //無線APのESSID
  const char *char_password = PASSWORD;           //無線AP接続パスワード

  const IPAddress my_ip(192, 168, 1, 110);        //ESP-WROOM-32（自身）のIPアドレス
  const IPAddress gateway(192,168, 1, 1);         //デフォルトゲートウェイ
  const IPAddress subnetmask(255, 255, 255, 0);   //サブネットマスク
  const IPAddress dns_ip(192, 168, 1, 1);         //DNSサーバのIPアドレス

  //接続完了まで通信状態目視用LEDを黄色点灯させる
  control_led(YELLOW);
  
  //ステーションモードに設定する
  WiFi.mode(WIFI_STA);

  //ネットワークの設定を行う
  WiFi.config(my_ip, gateway, subnetmask);
  vTaskDelay(50);

  //無線APに接続する
  WiFi.begin(char_ssid, char_password);

  //接続が成功するまで待機する
  while(WiFi.status() != WL_CONNECTED) {
      vTaskDelay(100);
  }

  //接続が完了したので通信状態目視用LEDを緑色点灯させる
  control_led(GREEN);
  
  return;
}
