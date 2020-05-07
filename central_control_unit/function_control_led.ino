//フルカラーLED制御シーケンス
void control_led(uint8_t order){
  const uint8_t *data_pin_p = uint8_t_data_pin;
  uint8_t *cout_state_p = uint8_t_cout_state;
  uint8_t *color_p;

  uint8_t uint8_t_full_color_red[]    = {1, 1, 1, 1, 0, 1, 1, 1};   //赤色
  uint8_t uint8_t_full_color_green[]  = {1, 1, 1, 0, 1, 1, 1, 1};   //緑色
  uint8_t uint8_t_full_color_blue[]   = {1, 1, 0, 1, 1, 1, 1, 1};   //青色
  uint8_t uint8_t_full_color_purple[] = {1, 1, 0, 1, 0, 1, 1, 1};   //紫色
  uint8_t uint8_t_full_color_yellow[] = {1, 1, 1, 0, 0, 1, 1, 1};   //黄色
  uint8_t uint8_t_full_color_sky[]    = {1, 1, 0, 0, 1, 1, 1, 1};   //空色
  uint8_t uint8_t_full_color_white[]  = {1, 1, 0, 0, 0, 1, 1, 1};   //白色
  uint8_t uint8_t_full_color_off[]    = {1, 1, 1, 1, 1, 1, 1, 1};   //消灯

  switch(order){
    case RED:
      color_p = uint8_t_full_color_red;
      break;
      
    case GREEN:
      color_p = uint8_t_full_color_green;
      break;
      
    case BLUE:
      color_p = uint8_t_full_color_blue;
      break;
      
    case PURPLE:
      color_p = uint8_t_full_color_purple;
      break;
      
    case YELLOW:
      color_p = uint8_t_full_color_yellow;
      break;
      
    case SKY:
      color_p = uint8_t_full_color_sky;
      break;
      
    case WHITE:
      color_p = uint8_t_full_color_white;
      break;
      
    case OFF:
      color_p = uint8_t_full_color_off;
      break;
      
    default:
      Serial.println("FULLCOLOR LED LIGHTING ERROR!!");
      return;
  }

  //COUTピンのフルカラーLEDに関する部位を初期化する
  for(uint8_t uint8_t_loop = 2 ; uint8_t_loop < 5 ; uint8_t_loop++){
    *(cout_state_p + uint8_t_loop) = *(cout_state_p + uint8_t_loop) & 0;
  }

  //COUTピンのフルカラーLEDに関する部位を更新する
  for(uint8_t uint8_t_loop = 2 ; uint8_t_loop < 5 ; uint8_t_loop++){
    *(cout_state_p + uint8_t_loop) = *(cout_state_p + uint8_t_loop) | *(color_p + uint8_t_loop);
  }

  //色変更をフルカラーLEDに反映させる
  for(uint8_t uint8_t_loop = 2 ; uint8_t_loop < 5 ; uint8_t_loop++){
    digitalWrite(*(data_pin_p + uint8_t_loop), *(cout_state_p + uint8_t_loop));
  }
  digitalWrite(CCLK_CTRL_PIN, LOW);
  vTaskDelay(1);
  digitalWrite(CCLK_CTRL_PIN, HIGH);
  
  return;
}
