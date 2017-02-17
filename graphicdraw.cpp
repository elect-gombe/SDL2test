#include <SDL2/SDL.h>
#include <iostream>
#include "graphiclib.hpp"
#include <assert.h>

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

void graphiclib::putdot(int x,int y){
  //  std::cout << "(" << v.x<<","<<v.y << ")"<<std::endl;
  //  std::cout << "brush" << int(brushcolor);
  assert(x >= 0&&x < width);
  assert(y >= 0&&y < height);
  vram[y*width+x] = brushcolor;
}

void graphiclib::putdot(const vector2& v){
  //  std::cout << "(" << v.x<<","<<v.y << ")"<<std::endl;
  //  std::cout << "brush" << int(brushcolor);
  assert(v.x >= 0&&v.x < width);
  assert(v.y >= 0&&v.y < height);
  vram[v.y*width+v.x] = brushcolor;
}

void graphiclib::drawcircle(const vector2& vin,int r){
  vector2 v;
  v.x = r;
  int f = -2 * r + 3;

  while ( v.x >= v.y ) {
    putdot( vin.x + v.x, vin.y + v.y );
    putdot( vin.x - v.x, vin.y + v.y );
    putdot( vin.x + v.x, vin.y - v.y );
    putdot( vin.x - v.x, vin.y - v.y );
    putdot( vin.x + v.y, vin.y + v.x );
    putdot( vin.x - v.y, vin.y + v.x );
    putdot( vin.x + v.y, vin.y - v.x );
    putdot( vin.x - v.y, vin.y - v.x );
    if ( f >= 0 ) {
      v.x--;
      f -= 4 * v.x;
    }
    v.y++;
    f += 4 * v.y + 2;
  }
}

void graphiclib::drawLine(const vector2& vin1,const vector2& vin2){
  vector2 s,d;
  vector2 v1,v2;
  v1 = vin1;
  v2 = vin2;
  int i,e;
 
  //std::cout << vin2.x <<"," << vin2.y << std::endl;

  if(v1.x<v2.x){
    d.x=v2.x-v1.x;
    s.x=1;
  }
  else{
    d.x=v1.x-v2.x;
    s.x=-1;
  }
  if(v2.y>v1.y){
    d.y=v2.y-v1.y;
    s.y=1;
  }
  else{
    d.y=v1.y-v2.y;
    s.y=-1;
  }

  if(d.x>=d.y){
    e=-d.x;
    for(i=0;i<=d.x;i++){
      putdot(v1);
      v1.x+=s.x;
      e+=d.y*2;
      if(e>=0){
	v1.y+=s.y;
	e-=d.x*2;
      }
    }
  }
  else{
    e=-d.y;
    for(i=0;i<=d.y;i++){
      putdot(v1);
      v1.y+=s.y;
      e+=d.x*2;
      if(e>=0){
	v1.x+=s.x;
	e-=d.y*2;
      }
    }
  }
}
