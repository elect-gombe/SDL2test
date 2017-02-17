/*
 * graphic library using SDL2
 *
 */

#include <stdint.h>
#include "vector2.h"

typedef uint8_t color_t;

class graphicSDL {
private:
  SDL_Surface *screen;
  SDL_Surface *drawarea;
  SDL_Window *window;
protected:
  color_t *vram;
  uint32_t palette[256];
  int width;
  int height;
  int gain;
public:
  graphicSDL(int w,int h){
    width = w;
    height = h;
    vram = new color_t[w*h]();
  }
  ~graphicSDL(){
    delete[] vram;
  }
  int init(int g);
  color_t *getvram(){ return vram;   }
  void flush();
};

class graphiclib :public graphicSDL{
private:
  vector2 pv;
  color_t brushcolor;
  color_t backcolor;
public:
  graphiclib(int w,int h): graphicSDL(w,h) {}
  void setbrush(color_t col){brushcolor = col;}
  void drawLine(const vector2& v){drawLine(v,pv);pv=v;}
  void drawLine(const vector2& v1,const vector2& v2);
  void drawLine(vector2& v1,vector2& v2);
  void putdot(const vector2& v);
  void putdot(int x,int y);
  void drawcircle(const vector2& vin,int r);
};
