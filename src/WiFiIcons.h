#ifndef WIFI_ICONS_H
#define WIFI_ICONS_H

//WiFi Signal Icon (8x8 pixels)
const unsigned char wifi_icon_bmp[] PROGMEM = {
  0b00111100,
  0b01000010,
  0b10011001,
  0b00100100,
  0b01000010,
  0b00011000,
  0b00100100,
  0b00011000
};

//Checkmark Icon (8x8 pixels)
const unsigned char checkmark_bmp[] PROGMEM = {
  0b00000000,
  0b00000010,
  0b00000110,
  0b00001100,
  0b10011000,
  0b11110000,
  0b01100000,
  0b00000000
};

//X Icon (8x8 pixels, if not connected)
const unsigned char xmark_bmp[] PROGMEM = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};

#endif