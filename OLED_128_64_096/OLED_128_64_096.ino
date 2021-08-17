#include <Arduino.h>
#include <U8g2lib.h>
 
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const int TEMP = 2;
const int RPM = 3;
const int MHZ = 4;
const int GHZ = 5;
const int PERPCENT = 6;

char TEMP_c[3] = " C";
char RPM_c[4] = "rpm";
char MHZ_c[4] = "MHz";
char GHZ_c[4] = "GHz";
char PERCENT_c[2] = "%";

const int MAX_W = 128;
const int MAX_H = 64;
const int PAGE_HEAD_PIXEL_H = 8;  // heading pixel height in page mode 
const int PAGE_VAL_PIXEL_H = 20;  // value pixel height in page mode
const int PAGE_VAL_PIXEL_W = 16;  // my estimation of pixel width in page mode
 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C
 
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);    //Low spped I2C



void setup(void) {
//  Serial.begin(9600);
  u8g2.begin();
}

class LCD_DISP {
  public:
    int lang = 0;

  int draw_page_contents(char heading[], char value_n_postfix[], int val_x_ofst, int val_y_ofst) {
    u8g2.setFont(u8g2_font_ncenB08_tr);   // choose a suitable font
    u8g2.drawStr(0,10,heading);    // write something to the internal memory
    u8g2.setFont(u8g2_font_fub20_tr);
    u8g2.drawStr(val_x_ofst,val_y_ofst,value_n_postfix);
    return 0;
    }

  int find_x_ofst(char value[], char postfx_char[]) {
    int len_val = strlen(value) - 1;
    int len_pfx = strlen(postfx_char);
    Serial.println(len_val);
    Serial.println(len_pfx);
    int x_start_pixel = (MAX_W - (len_val + len_pfx) * PAGE_VAL_PIXEL_W) / 2;  // cal starting of x pixel for value disp
    // Serial.println(x_start_pixel);
    if (x_start_pixel < 0 || x_start_pixel > MAX_W) {
      x_start_pixel = 20;
    }
    return x_start_pixel;
    } 

  void show_page(char value[], int page_type) {
    char val[8];
    u8g2.clearBuffer();                   // clear the internal memory
    switch (page_type) {
      case TEMP:
        strcpy(val, value);
        strcat(val, TEMP_c);
        draw_page_contents("CPU Temp:", val, find_x_ofst(value, TEMP_c), 40);
        break;
      case RPM:
        strcpy(val, value);
        strcat(val, RPM_c);
        draw_page_contents("CPU .RPM", val, find_x_ofst(value, RPM_c), 40);
        break;
      case GHZ:
        strcpy(val, value);
        strcat(val, GHZ_c);
        draw_page_contents("CPU Freq", val, find_x_ofst(value, GHZ_c), 40);
        break;
      default:
        break;
    }
    u8g2.sendBuffer();                    // transfer internal memory to the display
  }

  void idle_dot_left(int y) {
    u8g2.clearBuffer();  
    u8g2.drawBox(MAX_H - y*2, 40, 3, 3);
    u8g2.drawBox(MAX_H - y*3, 40, 3, 3);
    u8g2.drawBox(MAX_H - y*4, 40, 3, 3);
    u8g2.sendBuffer();   
    } 

  void idle_dot_right(int y) {
    u8g2.clearBuffer();  
    u8g2.drawBox(MAX_H + y*2, 40, 3, 3);
    u8g2.drawBox(MAX_H + y*3, 40, 3, 3);
    u8g2.drawBox(MAX_H + y*4, 40, 3, 3);
    u8g2.sendBuffer();   
    } 

  void show_idle(int x_half=MAX_H/2) {
    int i;
    for(i=x_half; i>0; i--){
      idle_dot_left(i);
    }
    for(i=0; i<x_half; i++){
      idle_dot_right(i);
    }
    u8g2.clearBuffer();  
    u8g2.sendBuffer();   
    delay(1000);
  }
};


void loop(void) {
  // show_page("1333", RPM);
  // delay(3000);  
  // show_page("40", TEMP);
  // delay(3000);  
  // show_page("4.92", GHZ);
  // delay(3000);  
  // draw_dot(1);
  LCD_DISP display;
  display.show_idle();
  // display.show_page("53", TEMP);
  // delay(3000);  
}
