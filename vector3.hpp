#ifndef __VECTOR3_H
#define __VECTOR3_H
#include <stdint.h>

class vector3{
public:
  int x;
  int y;
  int z;
  vector3(int x,int y,int z){
    this->x = x;
    this->y = y;
    this->z = z;
  }
  vector3(int x,int y){
    this->x = x;
    this->y = y;
    this->z = 0;
  }
  vector3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }

  vector3& operator*=(int p){
    this->x = ((int64_t)this->x)*p;
    this->y = ((int64_t)this->y)*p;
    this->z = ((int64_t)this->z)*p;
    return *this;
  }
  vector3& operator+=(const vector3& v){
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
  }
  
  vector3& operator-=(const vector3& v){
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
  }

  vector3 operator+(){return *this;}
  vector3 operator-(){
    vector3 v;
    v.x = -this->x;
    v.y = -this->y;
    v.z = -this->z;
    return v;
  }
  int abs();
  void print();
};

static inline
vector3 cross(vector3 p1,vector3 p2){
  vector3 v;

  v.x = int64_t(p1.y)*p2.z/65536 - int64_t(p1.z)*p2.y/65536;
  v.y = int64_t(p1.z)*p2.x/65536 - int64_t(p1.x)*p2.z/65536;
  v.z = int64_t(p1.x)*p2.y/65536 - int64_t(p1.y)*p2.x/65536;
  return v;
}

static inline
vector3 operator+(const vector3 &v1,const vector3& v2){
  vector3 w;

  w.x = v1.x+v2.x;
  w.y = v1.y+v2.y;
  w.z = v1.z+v2.z;
  return w;
}

static inline
vector3 operator*(const vector3 &v1,int n){
  vector3 w;

  w.x = v1.x*n;
  w.y = v1.y*n;
  w.z = v1.z*n;
  return w;
}

static inline
vector3 operator-(const vector3 &v1,const vector3& v2){
  vector3 w;

  w.x = v1.x-v2.x;
  w.y = v1.y-v2.y;
  w.z = v1.z-v2.z;
  return w;
}

#endif
