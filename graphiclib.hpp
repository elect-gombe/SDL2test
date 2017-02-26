/*
 * graphic library using SDL2
 *
 */

#include <stdint.h>
#include "vector2.hpp"

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
  void delay(int ms){SDL_Delay(ms);}
  void close();
};

class graphiclib :public graphicSDL{
private:
  vector2 pv;
  color_t strokecolor;
  color_t fillcolor;
  const uint8_t *fontp;
  unsigned int fontexpa;
  unsigned int fontsize;
  inline uint8_t get_fontwidth();
  inline uint8_t get_fontheight();
  const uint8_t *get_font(char code);
  int calc_seq_code(vector2 c);
  int calc_intsec_x( vector2 c0, vector2 c1, int clip_x, vector2& c );
  int calc_intsec_y( vector2 c0, vector2 c1, int clip_y, vector2& c );
  int calc_clipped_point( int code, vector2 c0, vector2 c1, vector2& c );
public:
  graphiclib(int w,int h): graphicSDL(w,h) {}
  void setbrush(color_t col){strokecolor = col;}
  void setfill(color_t col){fillcolor = col;}
  void Line(const vector2& v){Line(v,pv);pv=v;}
  void Line(vector2 v1,vector2 v2);
  inline void putdot(const vector2& v);
  inline void putdot(int x,int y);
  inline void putsubdot(const vector2& v);
  inline void putsubdot(int x,int y);
  void circle(const vector2& vin,int r);
  void fillcircle(const vector2& vin,int r);
  inline void Hline(int x,int y,int w);
  inline void Hlinefill(int x,int y,int w);
  int clipping( vector2& c0, vector2& c1 );

  
  int textSize(int size);
  void drawFont(const vector2 &v,char m);
  void print_string(const char *str);
  void setPoint(vector2 v);
  void screenclear(void);
};

