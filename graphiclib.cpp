#include <SDL2/SDL.h>
#include "vector2.h"
#include <iostream>

#include "graphiclib.hpp"

int graphicSDL::init(int g){
  int ret;

  gain = g;
  ret = SDL_Init(SDL_INIT_EVERYTHING);
  if(ret==-1){
    std::cerr << "error" << __LINE__ << std::endl;
    return ret;
  }

  window = SDL_CreateWindow( "game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*g, height*g,SDL_WINDOW_SHOWN);
  
  if(window==NULL){
    std::cerr << "error" << __LINE__ << std::endl;
    return ret;
  }

  screen = SDL_GetWindowSurface(window);
  drawarea = SDL_CreateRGBSurface(0,width,height,32,0x00FF0000,0x0000FF00,0x000000FF,0);

  for(int i=0;i<256;i++){
    palette[i]=i*0x4F7AC4B6;
  }
  return 0;
}

void graphicSDL::flush(){
  SDL_Rect src,dist;
  src.x = 0;
  src.y = 0;
  src.w = width;
  src.h = height;
  dist.x = 0;
  dist.y = 0;
  dist.w = gain*width;
  dist.h = gain*height;
  
  for(int y=0;y<height;y++){
    for(int x=0;x<width;x++){
      ((uint32_t*)drawarea->pixels)[y*width+x] = palette[vram[x+y*width]];
    }
  }

  SDL_SetSurfaceBlendMode(drawarea,SDL_BLENDMODE_NONE);
  SDL_SetSurfaceBlendMode(screen,SDL_BLENDMODE_NONE);

  SDL_BlitScaled(drawarea,NULL,screen,NULL);

  SDL_UpdateWindowSurface(window);
}

int main(void){
  vector2 v(50,50);
  graphiclib sdl(160,120);
  sdl.init(5);
  for(int i=0;i<10;i++){
    sdl.setbrush(i);
    // std::cout << "i:" << i;
    sdl.drawLine(vector2(0,0),v);
    v+=vector2(5+i,0);
    sdl.drawcircle(v-vector2(10,i*3),15);
  }
  sdl.flush();
  SDL_Delay(5000);
}
