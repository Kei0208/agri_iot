//フルカラーLED制御シーケンス
void control_led(uint8_t order){

  uint8_t *color_p;
  uint8_t *pin_p = uint8_t_full_color_led_pin;

  uint8_t uint8_t_full_color_red[]    = {0, 1, 0};   //赤色
  uint8_t uint8_t_full_color_green[]  = {1, 0, 0};   //緑色
  uint8_t uint8_t_full_color_blue[]   = {0, 0, 1};   //青色
  uint8_t uint8_t_full_color_purple[] = {0, 1, 1};   //紫色
  uint8_t uint8_t_full_color_yellow[] = {1, 1, 0};   //黄色
  uint8_t uint8_t_full_color_sky[]    = {1, 0, 1};   //空色
  uint8_t uint8_t_full_color_white[]  = {1, 1, 1};   //白色
  uint8_t uint8_t_full_color_off[]    = {0, 0, 0};   //消灯

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

  //1度すべて消す
  for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 3 ; uint8_t_loop++){
    digitalWrite(*(pin_p + uint8_t_loop), LOW);
  }

  //色を反映させる
  for(uint8_t uint8_t_loop = 0 ; uint8_t_loop < 3 ; uint8_t_loop++){
    digitalWrite(*(pin_p + uint8_t_loop), *(color_p + uint8_t_loop));
  }
  return;
}
