#include <stdio.h>
#include <stdint.h>

typedef unsigned int uint;

const uint8_t fonthn12[]={
#include "fonthn12.h"
};

const uint8_t fonthn16[]={
#include "fonthn16.h"
};

void setPixel(uint x,uint y,uint col);

uint8_t get_fontwidth(const uint8_t *font){
  return font[14];
}
uint8_t get_fontheight(const uint8_t *font){
  return font[15];
}

const uint8_t *get_font(const uint8_t *font,char code){
  uint fsz;
  fsz = (get_fontwidth(font) + 7) / 8 * get_fontheight(font);  /* Get font size */
  
  if (font[16] == 0) {  /* Single byte code font */
    return &font[17 + code * fsz];
  }
  return NULL;
}

void _putcharactor(const uint8_t *font,uint size,uint px,uint py,char code,uint col){
  uint x,y;
  const uint8_t *gotfont;
  uint8_t idx;
  uint8_t readbit;

  gotfont = get_font(font,code);
  for(y=0;y<get_fontheight(font)*size;y++){
    for(x=0;x<get_fontwidth(font)*size;x++){
      idx     = (x/size / 8) + y/size * ((get_fontwidth(font) + 7) / 8);
      readbit = 7 - (x/size % 8);
      if(gotfont[idx]&0x01<<readbit){
	setPixel(x+px,y+py,col);
      }
      else{
	setPixel(x+px,y+py,!col);
      }
    }
  }
}


void putcharactor(uint8_t size,uint px,uint py,char code,uint col){
  const uint8_t *font;
  uint expa;
  switch(size){
  case 12:
    font = fonthn12;
    expa = 1;
    break;
  case 16:
    font = fonthn16;
    expa = 1;
    break;
  case 24:
    font = fonthn12;
    expa = 2;
    break;
  case 32:
    font = fonthn16;
    expa = 2;
    break;
  }
  _putcharactor(font, expa, px, py, code, col);
}

void print_string(uint8_t size,uint px,uint py,char *str,uint col){
  while(*str){
    putcharactor(size,px,py,*str,col);
    str++;
    px+=size/2;
  }
}
