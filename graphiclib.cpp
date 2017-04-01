#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "matrix4.hpp"
#include "graphiclib.hpp"
#include "triangle.hpp"

int frame;

Matrix4 loadPerspective(int fovy, int aspect,int zNear, int zFar);
void print(Matrix4 m);
void rawPoly(graphiclib g);

int graphicSDL::init(int g){
  int ret;

  gain = g;
  ret = SDL_Init(SDL_INIT_EVERYTHING);
  if(ret==-1){
    return ret;
  }

  window = SDL_CreateWindow( "game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width*g, window_height*g,SDL_WINDOW_SHOWN);
  
  if(window==NULL){
    return ret;
  }

  screen = SDL_GetWindowSurface(window);
  drawarea = SDL_CreateRGBSurface(0,window_width,window_height,32,0x00FF0000,0x0000FF00,0x000000FF,0);

  for(int i=0;i<6*6*6;i++){
    palette[i]=(i%6)*256/6;
    palette[i]|=(((i/6)%6)*256/6)<<8;
    palette[i]|=(((i/6)/6)*256/6)<<16;
  }

  for(int i=6*6*6;i<256;i++){
    palette[i]=(i-224)*256/(16);
    palette[i]|=((i-224)*256/(16))<<8;
    palette[i]|=((i-224)*256/(16))<<16;
  }
  return 0;
}

void graphicSDL::close(){
  SDL_DestroyWindow(window);    
}

SDL_Event event={
  .type = 0
};

void graphicSDL::flush(){
  // SDL_Rect src,dist;
  // src.x = 0;
  // src.y = 0;
  // src.w = window_width;
  // src.h = window_height;
  // dist.x = 0;
  // dist.y = 0;
  // dist.w = gain*window_width;
  // dist.h = gain*window_height;
  
  for(int y=0;y<window_height;y++){
    for(int x=0;x<window_width;x++){
      ((uint32_t*)drawarea->pixels)[y*window_width+x] = palette[vram[x+y*window_width]];
    }
  }

  SDL_SetSurfaceBlendMode(drawarea,SDL_BLENDMODE_NONE);
  SDL_SetSurfaceBlendMode(screen,SDL_BLENDMODE_NONE);

  SDL_BlitScaled(drawarea,NULL,screen,NULL);

  SDL_UpdateWindowSurface(window);

  SDL_PollEvent( &event );

  if(event.type==SDL_QUIT)exit(0);
}

int isKeyPressed(){
  return event.type == SDL_KEYDOWN;
}
int getkey(){
  return event.key.keysym.sym;
}
