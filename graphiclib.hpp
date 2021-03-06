/*
 * graphic library using SDL2
 *
 */

#include <stdint.h>
#include "vector2.hpp"
#include "vector3.hpp"
#include <assert.h>
#include <SDL2/SDL.h>

extern int frame;
const static int window_width = 256;
const static int window_height = 220;

extern const int window_width;
extern const int window_height;

template <typename T>
T min(T v1 , T v2) {
  if (v1 < v2) return v1;
  else return v2;
}

template <typename T>
T abs(T v) {
  if (v > 0) return v;
  else return -v;
}

template <typename T>
T max(T v1 , T v2) {
  if (v1 > v2) return v1;
  else return v2;
}

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
  int getheight(){return height;}
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
  void Line(const vector3 &vin1,const vector3 &vin2);
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


inline void graphiclib::Hline(int x,int y,int w){
  assert(y >= 0 && y < height);
  assert(x >= 0 && x+w <= width);
  
  for(int i=0;i<w;i++){
    vram[y*width+x+i] = strokecolor;
  }
}

inline void graphiclib::Hlinefill(int x,int y,int w){
  assert(0 >= y && y < height);
  assert(0 >= x && x+w < width);
  for(int i=0;i<w;i++){
    vram[y*width+x+i] = fillcolor;
  }
}

inline void graphiclib::putsubdot(int x,int y){
  assert(x >= 0&&x < width);
  assert(y >= 0&&y < height);
  if(fillcolor!=255){
    vram[y*width+x] = fillcolor;
  }
}

inline void graphiclib::putsubdot(const vector2& v){
  assert(v.x >= 0&&v.x < width);
  assert(v.y >= 0&&v.y < height);
  if(fillcolor!=255){
    vram[v.y*width+v.x] = fillcolor;
  }
}

inline void graphiclib::putdot(int x,int y){
  assert(x >= 0&&x < width);
  assert(y >= 0&&y < height);
  vram[y*width+x] = strokecolor;
}

inline void graphiclib::putdot(const vector2& v){
  assert(v.x >= 0&&v.x < width);
  assert(v.y >= 0&&v.y < height);
  vram[v.y*width+v.x] = strokecolor;
}
int isKeyPressed();
int getkey();
