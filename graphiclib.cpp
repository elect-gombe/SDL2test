#include <SDL2/SDL.h>
#include "vector2.hpp"
#include <iostream>
#include "vector3.hpp"
#include "vector4.hpp"
#include "matrix4.hpp"
#include "graphiclib.hpp"

class triangle{
public:
  vector3 p[3];
  color_t col;
  int ymin;
  int ymax;
  int delta[2][2];
  int entryx[2];

  triangle(vector3 px[3],color_t col){
    int delta_top_mid;
    int delta_top_btm;
    int delta_mid_btm;
    int top_mid_x;
    int top_btm_x;
    int delta_zx;
    int delta_zy;
    vector3 t;
    vector3 cr;

    vector3 a = px[2]-px[0];
    vector3 b = px[1]-px[0];

    printf("a=(%d,%d,%d)\n",a.x,a.y,a.z);
    printf("b=(%d,%d,%d)\n",b.x,b.y,b.z);
    a*=65536;
    b*=65536;
    cr = cross(a,b);
 
    if(cr.z!=0){
      delta_zx = int64_t(cr.x)*65536/cr.z;
      delta_zy = int64_t(cr.y)*65536/cr.z;
      printf("(dz/dx,dz/dy)=(%f,%f)\n",delta_zx/65536.,delta_zy/65536.);
    }else{
      printf("cr.z is zero(%d,%d,%d)\n",cr.x,cr.y,cr.z);
    }
    
    for(int i=0;i<3;i++){
      p[i]=px[i];
    }
    if(p[0].y > p[1].y) {
      t=p[0];
      p[0] = p[1];
      p[1]=p[0];
    }
    if(p[0].y > p[2].y){
      t=p[0];
      p[0]=p[2];
      p[2]=t;
    }
    if(p[1].y > p[2].y){
      t=p[1];
      p[1]=p[2];
      p[2]=t;
    }
    // std::cout << "("<<p[0].x<<","<<p[0].y<<")"<<std::endl;
    // std::cout << "("<<p[1].x<<","<<p[1].y<<")"<<std::endl;
    // std::cout << "("<<p[2].x<<","<<p[2].y<<")"<<std::endl;

    this->col = col;
    ymin = cymin();
    ymax = cymax();

    top_mid_x = p[0].x;
    top_btm_x = p[0].x;

    if (ymin < 0) {
      ymin = 0;
      if (p[1].y >= 0) {
	if(p[0].y != p[1].y)
	  top_mid_x = (p[1].x - p[0].x) * int64_t(p[1].y) / (p[0].y - p[1].y) + p[1].x;
      }else{
	if(p[1].y != p[2].y)
	  top_mid_x = (p[2].x - p[1].x) * int64_t(p[2].y) / (p[1].y - p[2].y) + p[2].x;
      }
      if(p[0].y != p[2].y)
	top_btm_x = (p[2].x - p[0].x ) * int64_t(p[2].y) / (p[0].y - p[2].y ) + p[2].x;
    }
  
    if(p[2].y!=p[1].y){
      delta_top_mid = int64_t(65536)*(p[1].x-p[2].x)/(p[1].y-p[2].y);
    }else{
      delta_top_mid = 0;
    }
  
    if(p[1].y!=p[0].y){
      delta_mid_btm = int64_t(65536)*(p[1].x-p[0].x)/(p[1].y-p[0].y);
    }else{
      delta_mid_btm = 0;
    }

    if(p[2].y!=p[0].y){
      delta_top_btm = int64_t(65536)*(p[2].x-p[0].x)/(p[2].y-p[0].y);
    }else{
      delta_top_btm = 0;
    }
    printf("delta %f %f %f\n",delta_top_btm/65536.,delta_top_mid/65536.,delta_mid_btm/65536.);

    int split_x;
    if(p[2].y != p[1].y){
      split_x = (p[2].x-p[0].x)*(p[1].y-p[2].y)/(p[2].y-p[0].y)+p[2].x;
    }else{
      split_x = p[0].x;
    }
    if(p[1].x > split_x){
      entryx[0] = top_mid_x;
      entryx[1] = top_btm_x;
      delta[1][0]=delta_top_mid;
      delta[1][1]=delta_top_btm;
      delta[0][0]=delta_mid_btm;
      delta[0][1]=delta_top_btm;
    }else{
      entryx[0] = top_btm_x;
      entryx[1] = top_mid_x;
      delta[1][0]=delta_top_btm;
      delta[1][1]=delta_top_mid;
      delta[0][0]=delta_top_btm;
      delta[0][1]=delta_mid_btm;
    }
    printf("start @%d @%d\n",entryx[0],entryx[1]);
    printf("delta %f %f %f %f\n",delta[0][0]/65536.,delta[0][1]/65536.,delta[1][0]/65536.,delta[1][1]/65536.);
  }
  
  int cymin(){
    return p[0].y;
  }

  int cymax(){
    return p[2].y;
  }
};

Matrix4 loadPerspective(int fovy, int aspect,int zNear, int zFar);
void print(Matrix4 m);
void drawPoly(graphiclib g);

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

void graphicSDL::close(){
  SDL_DestroyWindow(window);    
}

void graphicSDL::flush(){
  // SDL_Rect src,dist;
  // src.x = 0;
  // src.y = 0;
  // src.w = width;
  // src.h = height;
  // dist.x = 0;
  // dist.y = 0;
  // dist.w = gain*width;
  // dist.h = gain*height;
  
  for(int y=0;y<height;y++){
    for(int x=0;x<width;x++){
      ((uint32_t*)drawarea->pixels)[y*width+x] = palette[vram[x+y*width]];
    }
  }

  SDL_SetSurfaceBlendMode(drawarea,SDL_BLENDMODE_NONE);
  SDL_SetSurfaceBlendMode(screen,SDL_BLENDMODE_NONE);

  SDL_BlitScaled(drawarea,NULL,screen,NULL);

  SDL_UpdateWindowSurface(window);

  SDL_Event event;
  SDL_PollEvent( &event );

  if(event.type==SDL_QUIT)exit(0);
}

vector3 pv[12][3];

int main(void){
    Matrix4 m;
    int i=0;

    pv[i][0] =  vector3(0,0,0)*65536;
    pv[i++][1] = vector3(1,0,0)*65536;
    pv[i][0] = vector3(0,0,0)*65536;
    pv[i++][1] = vector3(0,1,0)*65536;
    pv[i][0] = vector3(1,1,0)*65536;
    pv[i++][1] = vector3(1,0,0)*65536;
    pv[i][0] = vector3(1,1,0)*65536;
    pv[i++][1] = vector3(0,1,0)*65536;
  
    pv[i][0] = vector3(0,0,1)*65536;
    pv[i++][1] = vector3(1,0,1)*65536;
    pv[i][0] = vector3(0,0,1)*65536;
    pv[i++][1] = vector3(0,1,1)*65536;
    pv[i][0] = vector3(1,1,1)*65536;
    pv[i++][1] = vector3(1,0,1)*65536;
    pv[i][0] = vector3(1,1,1)*65536;
    pv[i++][1] = vector3(0,1,1)*65536;

    pv[i][0] = vector3(0,0,0)*65536;
    pv[i++][1] = vector3(0,0,1)*65536;
    pv[i][0] = vector3(0,1,0)*65536;
    pv[i++][1] = vector3(0,1,1)*65536;
    pv[i][0] = vector3(1,0,0)*65536;
    pv[i++][1] = vector3(1,0,1)*65536;
    pv[i][0] = vector3(1,1,0)*65536;
    pv[i++][1] = vector3(1,1,1)*65536;

    for(int i=0;i<12;i++){
      for(int j=0;j<2;j++){
	pv[i][j]+=vector3(-65536/2,-65536/2,-65536/2);
      }
    }
    
    //Matrix4 loadPerspective(int fovy, int aspect,int zNear, int zFar){
    int c=0;
    graphiclib sdl(160,120);

    sdl.init(2);
    {
      vector3 pv[3]={vector3(30,10,3),vector3(40,40,5),vector3(10,30,1)};
      triangle t(pv,0);
    }
    while(1){
      m=loadPerspective(10000,1,65536,65536*20,0,0);
      vector2 mouse;

      SDL_GetMouseState(&mouse.x,&mouse.y);
      mouse.x-=160;
      mouse.y-=160;
      mouse*=65536*10;
      m=m*lookat(vector3(0,0,0),vector3(0,mouse.y,mouse.x-120000));

      //print(m);

      //      m=m*translation(vector3(0,0,65536*3));

      m=m*rotatey(c++*800);
      // m=m*rotatex(5461);
      // m=m*rotatez(5461);

      m=m*magnify(1);
      print(m);
      
      vector3 v[2];
      vector2 v1,v2;
      for(int i=0;i<12;i++){
	sdl.setbrush(i*7+1);
	for(int j=0;j<2;j++){
	  v[j] = m.applyit(pv[i][j]);
	  std::cout<<"poly"<<pv[i][j].x/65536.<<","<<pv[i][j].y/65536.<<","<<pv[i][j].z/65536.<<std::endl;
	  std::cout<<v[j].x/65536.<<","<<v[j].y/65536.<<","<<v[j].z/65536.<<std::endl;
	}
	if(v[0].z < 0)break;
	if(v[1].z < 0)break;
	v1.x=v[0].x/(65536/160);v1.y=v[0].y/(65536/120);
	v2.x=v[1].x/(65536/160);v2.y=v[1].y/(65536/120);
	//      std::cout<<v1.x
	sdl.Line(v1+vector2(160/2,120/2),v2+vector2(160/2,120/2));
      }

      for(int i=0;i<2;i++){
	sdl.flush();
	sdl.delay(25);
      }
      sdl.screenclear();
    }
}
