#pragma once

//полезные структуры для работы с векторами, матрицами

#include <math.h>
#include <stdlib.h>
#include <memory>
#include <vector>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace LiteMath 
{

  struct float2
  {
    float2() :x(0), y(0) {}
    float2(float a, float b) : x(a), y(b) {}

    float x, y;
  };

  struct float3
  {
    float3() :x(0), y(0), z(0) {}
    float3(float a, float b, float c) : x(a), y(b), z(c) {}
    float3(const float* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[0]) {}

    float x, y, z;
  };

  struct float4
  {
    float4() : x(0), y(0), z(0), w(0) {}
    float4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}

    float x, y, z, w;
  };

  struct int3
  {
    int3() :x(0), y(0), z(0) {}
    int3(int a, int b, int c) : x(a), y(b), z(c) {}
    int3(const int* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[0]) {}

    int x, y, z;
  };


  struct float4x4
  {
    float4x4() { identity(); }

    float4x4(const float arr[16])
    {
      row[0] = float4(arr[0], arr[1], arr[2], arr[3]);
      row[1] = float4(arr[4], arr[5], arr[6], arr[7]);
      row[2] = float4(arr[8], arr[9], arr[10], arr[11]);
      row[3] = float4(arr[12], arr[13], arr[14], arr[15]);
    }

    void identity()
    {
      row[0] = float4(1, 0, 0, 0);
      row[1] = float4(0, 1, 0, 0);
      row[2] = float4(0, 0, 1, 0);
      row[3] = float4(0, 0, 0, 1);
    }

    float& M(int x, int y)       { return ((float*)row)[y * 4 + x]; }
    float  M(int x, int y) const { return ((float*)row)[y * 4 + x]; }

    float* L()             { return (float*)row; }
    const float* L() const { return (float*)row; }

    float4 row[4];
  };

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

  struct uchar4
  {
    uchar4() :x(0), y(0), z(0), w(0) {}
    uchar4(unsigned char a, unsigned char b, unsigned char c, unsigned char d) : x(a), y(b), z(c), w(d) {}

    unsigned char x, y, z, w;
  };

  struct uint4
  {
    uint4() :x(0), y(0), z(0), w(0) {}
    uint4(unsigned int a, unsigned int b, unsigned int c, unsigned int d) : x(a), y(b), z(c), w(d) {}

    unsigned int x, y, z, w;
  };

  struct int4
  {
    int4() :x(0), y(0), z(0), w(0) {}
    int4(int a, int b, int c, int d) : x(a), y(b), z(c), w(d) {}

    int x, y, z, w;
  };

  static inline int4 make_int4(int a, int b, int c, int d) { int4 res; res.x = a; res.y = b; res.z = c; res.w = d; return res; }

  struct ushort2
  {
    ushort2() : x(0), y(0) {}
    ushort2(unsigned short a, unsigned short b) : x(a), y(b) {}

    unsigned short x, y;
  };

  struct ushort4
  {
    ushort4() :x(0), y(0), z(0), w(0) {}
    ushort4(unsigned short a, unsigned short b, unsigned short c, unsigned short d) : x(a), y(b), z(c), w(d) {}

    unsigned short x, y, z, w;
  };

  struct int2
  {
    int2() : x(0), y(0) {}
    int2(int a, int b) : x(a), y(b) {}

    int x, y;
  };

  struct uint2
  {
    uint2() : x(0), y(0) {}
    uint2(unsigned int a, unsigned int b) : x(a), y(b) {}

    unsigned int x, y;
  };

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


  inline float rnd(float s, float e)
  {
    float t = (float)(rand()) / (float)RAND_MAX;
    return s + t*(e - s);
  }

  static inline float clamp(float u, float a, float b) { float r = fmax(a, u); return fmin(r, b); }
  static inline int   clamp(int u, int a, int b) { int r = (a > u) ? a : u; return (r < b) ? r : b; }

  static inline int max(int a, int b) { return a > b ? a : b; }
  static inline int min(int a, int b) { return a < b ? a : b; }


  #define SQR(x) ((x)*(x))

  static inline float4 make_float4(float a, float b, float c, float d) { return float4(a, b, c, d); }
  static inline float3 make_float3(float a, float b, float c) { return float3(a, b, c); }
  static inline float2 make_float2(float a, float b) { return float2(a, b); }

  static inline float2 to_float2(float4 v) { return make_float2(v.x, v.y); }
  static inline float2 to_float2(float3 v) { return make_float2(v.x, v.y); }
  static inline float3 to_float3(float4 v) { return make_float3(v.x, v.y, v.z); }
  static inline float4 to_float4(float3 v, float w) { return make_float4(v.x, v.y, v.z, w); }

  //**********************************************************************************
  // float4 operators and functions
  //**********************************************************************************
  static inline float4 operator * (const float4 & u, float v) { return make_float4(u.x * v, u.y * v, u.z * v, u.w * v); }
  static inline float4 operator / (const float4 & u, float v) { return make_float4(u.x / v, u.y / v, u.z / v, u.w / v); }
  static inline float4 operator * (float v, const float4 & u) { return make_float4(v * u.x, v * u.y, v * u.z, v * u.w); }
  static inline float4 operator / (float v, const float4 & u) { return make_float4(v / u.x, v / u.y, v / u.z, v / u.w); }

  static inline float4 operator + (const float4 & u, const float4 & v) { return make_float4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w); }
  static inline float4 operator - (const float4 & u, const float4 & v) { return make_float4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); }
  static inline float4 operator * (const float4 & u, const float4 & v) { return make_float4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w); }
  static inline float4 operator / (const float4 & u, const float4 & v) { return make_float4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w); }

  static inline float4 & operator += (float4 & u, const float4 & v) { u.x += v.x; u.y += v.y; u.z += v.z; u.w += v.w; return u; }
  static inline float4 & operator -= (float4 & u, const float4 & v) { u.x -= v.x; u.y -= v.y; u.z -= v.z; u.w -= v.w; return u; }
  static inline float4 & operator *= (float4 & u, const float4 & v) { u.x *= v.x; u.y *= v.y; u.z *= v.z; u.w *= v.w; return u; }
  static inline float4 & operator /= (float4 & u, const float4 & v) { u.x /= v.x; u.y /= v.y; u.z /= v.z; u.w /= v.w; return u; }

  static inline float4 & operator += (float4 & u, float v) { u.x += v; u.y += v; u.z += v; u.w += v; return u; }
  static inline float4 & operator -= (float4 & u, float v) { u.x -= v; u.y -= v; u.z -= v; u.w -= v; return u; }
  static inline float4 & operator *= (float4 & u, float v) { u.x *= v; u.y *= v; u.z *= v; u.w *= v; return u; }
  static inline float4 & operator /= (float4 & u, float v) { u.x /= v; u.y /= v; u.z /= v; u.w /= v; return u; }

  static inline float4   operator - (const float4 & v) { return make_float4(-v.x, -v.y, -v.z, -v.w); }

  static inline float4 catmullrom(const float4 & P0, const float4 & P1, const float4 & P2, const float4 & P3, float t)
  {
    const float ts = t * t;
    const float tc = t * ts;

    return (P0 * (-tc + 2.0f * ts - t) + P1 * (3.0f * tc - 5.0f * ts + 2.0f) + P2 * (-3.0f * tc + 4.0f * ts + t) + P3 * (tc - ts)) * 0.5f;
  }

  static inline float4 lerp(const float4 & u, const float4 & v, float t) { return u + t * (v - u); }
  static inline float  dot(const float4 & u, const float4 & v) { return (u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w); }
  static inline float  dot3(const float4 & u, const float4 & v) { return (u.x*v.x + u.y*v.y + u.z*v.z); }
  static inline float  dot3(const float4 & u, const float3 & v) { return (u.x*v.x + u.y*v.y + u.z*v.z); }

  static inline float4 clamp(const float4 & u, float a, float b) { return make_float4(clamp(u.x, a, b), clamp(u.y, a, b), clamp(u.z, a, b), clamp(u.w, a, b)); }

  static inline float  length3(const float4 & u) { return sqrtf(SQR(u.x) + SQR(u.y) + SQR(u.z)); }
  static inline float  length(const float4 & u) { return sqrtf(SQR(u.x) + SQR(u.y) + SQR(u.z) + SQR(u.w)); }

  //inline float4 sqrt   (const float4 & u) { make_float4( sqrt(u.x), sqrt(u.y), sqrt(u.z), sqrt(u.w) ); }

  //**********************************************************************************
  // float3 operators and functions
  //**********************************************************************************
  static inline float3 operator * (const float3 & u, float v) { return make_float3(u.x * v, u.y * v, u.z * v); }
  static inline float3 operator / (const float3 & u, float v) { return make_float3(u.x / v, u.y / v, u.z / v); }
  static inline float3 operator * (float v, const float3 & u) { return make_float3(v * u.x, v * u.y, v * u.z); }
  static inline float3 operator / (float v, const float3 & u) { return make_float3(v / u.x, v / u.y, v / u.z); }

  static inline float3 operator + (const float3 & u, const float3 & v) { return make_float3(u.x + v.x, u.y + v.y, u.z + v.z); }
  static inline float3 operator - (const float3 & u, const float3 & v) { return make_float3(u.x - v.x, u.y - v.y, u.z - v.z); }
  static inline float3 operator * (const float3 & u, const float3 & v) { return make_float3(u.x * v.x, u.y * v.y, u.z * v.z); }
  static inline float3 operator / (const float3 & u, const float3 & v) { return make_float3(u.x / v.x, u.y / v.y, u.z / v.z); }

  static inline float3 operator - (const float3 & u) { return make_float3(-u.x, -u.y, -u.z); }

  static inline float3 & operator += (float3 & u, const float3 & v) { u.x += v.x; u.y += v.y; u.z += v.z; return u; }
  static inline float3 & operator -= (float3 & u, const float3 & v) { u.x -= v.x; u.y -= v.y; u.z -= v.z; return u; }
  static inline float3 & operator *= (float3 & u, const float3 & v) { u.x *= v.x; u.y *= v.y; u.z *= v.z; return u; }
  static inline float3 & operator /= (float3 & u, const float3 & v) { u.x /= v.x; u.y /= v.y; u.z /= v.z; return u; }

  static inline float3 & operator += (float3 & u, float v) { u.x += v; u.y += v; u.z += v; return u; }
  static inline float3 & operator -= (float3 & u, float v) { u.x -= v; u.y -= v; u.z -= v; return u; }
  static inline float3 & operator *= (float3 & u, float v) { u.x *= v; u.y *= v; u.z *= v; return u; }
  static inline float3 & operator /= (float3 & u, float v) { u.x /= v; u.y /= v; u.z /= v; return u; }


  static inline float3 catmullrom(const float3 & P0, const float3 & P1, const float3 & P2, const float3 & P3, float t)
  {
    const float ts = t * t;
    const float tc = t * ts;

    return (P0 * (-tc + 2.0f * ts - t) + P1 * (3.0f * tc - 5.0f * ts + 2.0f) + P2 * (-3.0f * tc + 4.0f * ts + t) + P3 * (tc - ts)) * 0.5f;
  }

  static inline float3 lerp(const float3 & u, const float3 & v, float t) { return u + t * (v - u); }
  static inline float  dot(const float3 & u, const float3 & v) { return (u.x*v.x + u.y*v.y + u.z*v.z); }
  static inline float3 cross(const float3 & u, const float3 & v) { return make_float3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
  //inline float3 mul       (const float3 & u, const float3 & v) { return make_float3( u.x*v.x, u.y*v.y, u.z*v.z} ; return r; }
  static inline float3 clamp(const float3 & u, float a, float b) { return make_float3(clamp(u.x, a, b), clamp(u.y, a, b), clamp(u.z, a, b)); }

  static inline float  triple(const float3 & a, const float3 & b, const float3 & c) { return dot(a, cross(b, c)); }
  static inline float  length(const float3 & u) { return sqrtf(SQR(u.x) + SQR(u.y) + SQR(u.z)); }
  static inline float  lengthSquare(const float3 u) { return u.x*u.x + u.y*u.y + u.z*u.z; }
  static inline float3 normalize(const float3 & u) { return u / length(u); }
  static inline float  coordSumm(const float3 u) { return u.x* +u.y + u.z; }
  //static inline float  coordAbsMax (const float3 u) { return max(max(abs(u.x), abs(u.y)), abs(u.z)); }

  static inline float  maxcomp(const float3 & u) { return fmax(u.x, fmax(u.y, u.z)); }
  static inline float  mincomp(const float3 & u) { return fmin(u.x, fmin(u.y, u.z)); }


  //**********************************************************************************
  // float2 operators and functions
  //**********************************************************************************

  static inline float2 operator * (const float2 & u, float v) { return make_float2(u.x * v, u.y * v); }
  static inline float2 operator / (const float2 & u, float v) { return make_float2(u.x / v, u.y / v); }
  static inline float2 operator * (float v, const float2 & u) { return make_float2(v * u.x, v * u.y); }
  static inline float2 operator / (float v, const float2 & u) { return make_float2(v / u.x, v / u.y); }

  static inline float2 operator + (const float2 & u, const float2 & v) { return make_float2(u.x + v.x, u.y + v.y); }
  static inline float2 operator - (const float2 & u, const float2 & v) { return make_float2(u.x - v.x, u.y - v.y); }
  static inline float2 operator * (const float2 & u, const float2 & v) { return make_float2(u.x * v.x, u.y * v.y); }
  static inline float2 operator / (const float2 & u, const float2 & v) { return make_float2(u.x / v.x, u.y / v.y); }

  static inline float2   operator - (const float2 & v) { return make_float2(-v.x, -v.y); }

  static inline float2 & operator += (float2 & u, const float2 & v) { u.x += v.x; u.y += v.y; return u; }
  static inline float2 & operator -= (float2 & u, const float2 & v) { u.x -= v.x; u.y -= v.y; return u; }
  static inline float2 & operator *= (float2 & u, const float2 & v) { u.x *= v.x; u.y *= v.y; return u; }
  static inline float2 & operator /= (float2 & u, const float2 & v) { u.x /= v.x; u.y /= v.y; return u; }

  static inline float2 & operator += (float2 & u, float v) { u.x += v; u.y += v; return u; }
  static inline float2 & operator -= (float2 & u, float v) { u.x -= v; u.y -= v; return u; }
  static inline float2 & operator *= (float2 & u, float v) { u.x *= v; u.y *= v; return u; }
  static inline float2 & operator /= (float2 & u, float v) { u.x /= v; u.y /= v; return u; }

  static inline float2 catmullrom(const float2 & P0, const float2 & P1, const float2 & P2, const float2 & P3, float t)
  {
    const float ts = t * t;
    const float tc = t * ts;

    return (P0 * (-tc + 2.0f * ts - t) + P1 * (3.0f * tc - 5.0f * ts + 2.0f) + P2 * (-3.0f * tc + 4.0f * ts + t) + P3 * (tc - ts)) * 0.5f;
  }

  static inline float2 lerp(const float2 & u, const float2 & v, float t) { return u + t * (v - u); }
  static inline float  dot(const float2 & u, const float2 & v) { return (u.x*v.x + u.y*v.y); }
  static inline float2 clamp(const float2 & u, float a, float b) { return make_float2(clamp(u.x, a, b), clamp(u.y, a, b)); }


  static inline float  length(const float2 & u) { return sqrtf(SQR(u.x) + SQR(u.y)); }
  static inline float2 normalize(const float2 & u) { return u / length(u); }


  static inline float lerp(float u, float v, float t) { return u + t * (v - u); }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  static inline bool IntersectBoxBox(float2 box1Min, float2 box1Max, float2 box2Min, float2 box2Max)
  {
    return box1Min.x <= box2Max.x && box2Min.x <= box1Max.x &&
           box1Min.y <= box2Max.y && box2Min.y <= box1Max.y;
  }

  static inline bool IntersectBoxBox(int2 box1Min, int2 box1Max, int2 box2Min, int2 box2Max)
  {
    return box1Min.x <= box2Max.x && box2Min.x <= box1Max.x &&
           box1Min.y <= box2Max.y && box2Min.y <= box1Max.y;
  }

  static inline float4 mul(float4x4 m, float4 v)
  {
    float4 res;
    res.x = m.row[0].x*v.x + m.row[0].y*v.y + m.row[0].z*v.z + m.row[0].w*v.w;
    res.y = m.row[1].x*v.x + m.row[1].y*v.y + m.row[1].z*v.z + m.row[1].w*v.w;
    res.z = m.row[2].x*v.x + m.row[2].y*v.y + m.row[2].z*v.z + m.row[2].w*v.w;
    res.w = m.row[3].x*v.x + m.row[3].y*v.y + m.row[3].z*v.z + m.row[3].w*v.w;
    return res;
  }

  static inline float3 mul(float4x4 m, float3 v)
  {
    float3 res;
    res.x = m.row[0].x*v.x + m.row[0].y*v.y + m.row[0].z*v.z + m.row[0].w;
    res.y = m.row[1].x*v.x + m.row[1].y*v.y + m.row[1].z*v.z + m.row[1].w;
    res.z = m.row[2].x*v.x + m.row[2].y*v.y + m.row[2].z*v.z + m.row[2].w;
    return res;
  }


  static inline float3 mul4x3(float4x4 m, float3 v)
  {
    float3 res;
    res.x = m.row[0].x*v.x + m.row[0].y*v.y + m.row[0].z*v.z + m.row[0].w;
    res.y = m.row[1].x*v.x + m.row[1].y*v.y + m.row[1].z*v.z + m.row[1].w;
    res.z = m.row[2].x*v.x + m.row[2].y*v.y + m.row[2].z*v.z + m.row[2].w;
    return res;
  }

  static inline float3 mul3x3(float4x4 m, float3 v)
  {
    float3 res;
    res.x = m.row[0].x*v.x + m.row[0].y*v.y + m.row[0].z*v.z;
    res.y = m.row[1].x*v.x + m.row[1].y*v.y + m.row[1].z*v.z;
    res.z = m.row[2].x*v.x + m.row[2].y*v.y + m.row[2].z*v.z;
    return res;
  }


  static inline float4x4 make_float4x4_by_columns(float4 a, float4 b, float4 c, float4 d)
  {
    float4x4 m;

    m.row[0].x = a.x;
    m.row[1].x = a.y;
    m.row[2].x = a.z;
    m.row[3].x = a.w;

    m.row[0].y = b.x;
    m.row[1].y = b.y;
    m.row[2].y = b.z;
    m.row[3].y = b.w;

    m.row[0].z = c.x;
    m.row[1].z = c.y;
    m.row[2].z = c.z;
    m.row[3].z = c.w;

    m.row[0].w = d.x;
    m.row[1].w = d.y;
    m.row[2].w = d.z;
    m.row[3].w = d.w;

    return m;
  }

  static inline float4x4 transpose4x4(float4x4 m)
  {
    return make_float4x4_by_columns(m.row[0], m.row[1], m.row[2], m.row[3]);
  }

  static inline float4x4 mul(float4x4 m1, float4x4 m2)
  {
    const float4 column1 = mul(m1, make_float4(m2.row[0].x, m2.row[1].x, m2.row[2].x, m2.row[3].x));
    const float4 column2 = mul(m1, make_float4(m2.row[0].y, m2.row[1].y, m2.row[2].y, m2.row[3].y));
    const float4 column3 = mul(m1, make_float4(m2.row[0].z, m2.row[1].z, m2.row[2].z, m2.row[3].z));
    const float4 column4 = mul(m1, make_float4(m2.row[0].w, m2.row[1].w, m2.row[2].w, m2.row[3].w));

    return make_float4x4_by_columns(column1, column2, column3, column4);
  }

  static inline float4x4 translate4x4(float3 t)
  {
    const float4 column1 = make_float4(1.0f, 0.0f, 0.0f, 0.0f);
    const float4 column2 = make_float4(0.0f, 1.0f, 0.0f, 0.0f);
    const float4 column3 = make_float4(0.0f, 0.0f, 1.0f, 0.0f);
    const float4 column4 = make_float4( t.x,  t.y,  t.z, 1.0f);

    return make_float4x4_by_columns(column1, column2, column3, column4);
  }

  static inline float4x4 scale4x4(float3 t)
  {
    const float4 column1 = make_float4( t.x, 0.0f, 0.0f, 0.0f);
    const float4 column2 = make_float4(0.0f,  t.y, 0.0f, 0.0f);
    const float4 column3 = make_float4(0.0f, 0.0f,  t.z, 0.0f);
    const float4 column4 = make_float4(0.0f, 0.0f, 0.0f, 1.0f);

    return make_float4x4_by_columns(column1, column2, column3, column4);
  }

  static inline float4x4 rotate_X_4x4(float phi)
  {
    const float4 column1 = make_float4(1.0f,      0.0f,       0.0f, 0.0f);
    const float4 column2 = make_float4(0.0f, +cos(phi),  +sin(phi), 0.0f);
    const float4 column3 = make_float4(0.0f, -sin(phi),  +cos(phi), 0.0f);
    const float4 column4 = make_float4(0.0f,      0.0f,       0.0f, 1.0f);

    return make_float4x4_by_columns(column1, column2, column3, column4);
  }

  static inline float4x4 rotate_Y_4x4(float phi)
  {
    const float4 column1 = make_float4(+cos(phi), 0.0f, -sin(phi), 0.0f);
    const float4 column2 = make_float4(     0.0f, 1.0f,      0.0f, 0.0f);
    const float4 column3 = make_float4(+sin(phi), 0.0f, +cos(phi), 0.0f);
    const float4 column4 = make_float4(     0.0f, 0.0f,      0.0f, 1.0f);

    return make_float4x4_by_columns(column1, column2, column3, column4);
  }

  static inline float4x4 rotate_Z_4x4(float phi)
  {
    const float4 column1 = make_float4(+cos(phi), sin(phi), 0.0f, 0.0f);
    const float4 column2 = make_float4(-sin(phi), cos(phi), 0.0f, 0.0f);
    const float4 column3 = make_float4(     0.0f,     0.0f, 1.0f, 0.0f);
    const float4 column4 = make_float4(     0.0f,     0.0f, 0.0f, 1.0f);

    return make_float4x4_by_columns(column1, column2, column3, column4);
  }

  static inline float4x4 inverse4x4(float4x4 m1)
  {
    float tmp[12]; // temp array for pairs
    float4x4 m;

    // calculate pairs for first 8 elements (cofactors)
    //
    tmp[0] = m1.row[2].z * m1.row[3].w;
    tmp[1] = m1.row[2].w * m1.row[3].z;
    tmp[2] = m1.row[2].y * m1.row[3].w;
    tmp[3] = m1.row[2].w * m1.row[3].y;
    tmp[4] = m1.row[2].y * m1.row[3].z;
    tmp[5] = m1.row[2].z * m1.row[3].y;
    tmp[6] = m1.row[2].x * m1.row[3].w;
    tmp[7] = m1.row[2].w * m1.row[3].x;
    tmp[8] = m1.row[2].x * m1.row[3].z;
    tmp[9] = m1.row[2].z * m1.row[3].x;
    tmp[10] = m1.row[2].x * m1.row[3].y;
    tmp[11] = m1.row[2].y * m1.row[3].x;

    // calculate first 8 m1.rowents (cofactors)
    //
    m.row[0].x = tmp[0] * m1.row[1].y + tmp[3] * m1.row[1].z + tmp[4] * m1.row[1].w;
    m.row[0].x -= tmp[1] * m1.row[1].y + tmp[2] * m1.row[1].z + tmp[5] * m1.row[1].w;
    m.row[1].x = tmp[1] * m1.row[1].x + tmp[6] * m1.row[1].z + tmp[9] * m1.row[1].w;
    m.row[1].x -= tmp[0] * m1.row[1].x + tmp[7] * m1.row[1].z + tmp[8] * m1.row[1].w;
    m.row[2].x = tmp[2] * m1.row[1].x + tmp[7] * m1.row[1].y + tmp[10] * m1.row[1].w;
    m.row[2].x -= tmp[3] * m1.row[1].x + tmp[6] * m1.row[1].y + tmp[11] * m1.row[1].w;
    m.row[3].x = tmp[5] * m1.row[1].x + tmp[8] * m1.row[1].y + tmp[11] * m1.row[1].z;
    m.row[3].x -= tmp[4] * m1.row[1].x + tmp[9] * m1.row[1].y + tmp[10] * m1.row[1].z;
    m.row[0].y = tmp[1] * m1.row[0].y + tmp[2] * m1.row[0].z + tmp[5] * m1.row[0].w;
    m.row[0].y -= tmp[0] * m1.row[0].y + tmp[3] * m1.row[0].z + tmp[4] * m1.row[0].w;
    m.row[1].y = tmp[0] * m1.row[0].x + tmp[7] * m1.row[0].z + tmp[8] * m1.row[0].w;
    m.row[1].y -= tmp[1] * m1.row[0].x + tmp[6] * m1.row[0].z + tmp[9] * m1.row[0].w;
    m.row[2].y = tmp[3] * m1.row[0].x + tmp[6] * m1.row[0].y + tmp[11] * m1.row[0].w;
    m.row[2].y -= tmp[2] * m1.row[0].x + tmp[7] * m1.row[0].y + tmp[10] * m1.row[0].w;
    m.row[3].y = tmp[4] * m1.row[0].x + tmp[9] * m1.row[0].y + tmp[10] * m1.row[0].z;
    m.row[3].y -= tmp[5] * m1.row[0].x + tmp[8] * m1.row[0].y + tmp[11] * m1.row[0].z;

    // calculate pairs for second 8 m1.rowents (cofactors)
    //
    tmp[0] = m1.row[0].z * m1.row[1].w;
    tmp[1] = m1.row[0].w * m1.row[1].z;
    tmp[2] = m1.row[0].y * m1.row[1].w;
    tmp[3] = m1.row[0].w * m1.row[1].y;
    tmp[4] = m1.row[0].y * m1.row[1].z;
    tmp[5] = m1.row[0].z * m1.row[1].y;
    tmp[6] = m1.row[0].x * m1.row[1].w;
    tmp[7] = m1.row[0].w * m1.row[1].x;
    tmp[8] = m1.row[0].x * m1.row[1].z;
    tmp[9] = m1.row[0].z * m1.row[1].x;
    tmp[10] = m1.row[0].x * m1.row[1].y;
    tmp[11] = m1.row[0].y * m1.row[1].x;

    // calculate second 8 m1 (cofactors)
    //
    m.row[0].z = tmp[0] * m1.row[3].y + tmp[3] * m1.row[3].z + tmp[4] * m1.row[3].w;
    m.row[0].z -= tmp[1] * m1.row[3].y + tmp[2] * m1.row[3].z + tmp[5] * m1.row[3].w;
    m.row[1].z = tmp[1] * m1.row[3].x + tmp[6] * m1.row[3].z + tmp[9] * m1.row[3].w;
    m.row[1].z -= tmp[0] * m1.row[3].x + tmp[7] * m1.row[3].z + tmp[8] * m1.row[3].w;
    m.row[2].z = tmp[2] * m1.row[3].x + tmp[7] * m1.row[3].y + tmp[10] * m1.row[3].w;
    m.row[2].z -= tmp[3] * m1.row[3].x + tmp[6] * m1.row[3].y + tmp[11] * m1.row[3].w;
    m.row[3].z = tmp[5] * m1.row[3].x + tmp[8] * m1.row[3].y + tmp[11] * m1.row[3].z;
    m.row[3].z -= tmp[4] * m1.row[3].x + tmp[9] * m1.row[3].y + tmp[10] * m1.row[3].z;
    m.row[0].w = tmp[2] * m1.row[2].z + tmp[5] * m1.row[2].w + tmp[1] * m1.row[2].y;
    m.row[0].w -= tmp[4] * m1.row[2].w + tmp[0] * m1.row[2].y + tmp[3] * m1.row[2].z;
    m.row[1].w = tmp[8] * m1.row[2].w + tmp[0] * m1.row[2].x + tmp[7] * m1.row[2].z;
    m.row[1].w -= tmp[6] * m1.row[2].z + tmp[9] * m1.row[2].w + tmp[1] * m1.row[2].x;
    m.row[2].w = tmp[6] * m1.row[2].y + tmp[11] * m1.row[2].w + tmp[3] * m1.row[2].x;
    m.row[2].w -= tmp[10] * m1.row[2].w + tmp[2] * m1.row[2].x + tmp[7] * m1.row[2].y;
    m.row[3].w = tmp[10] * m1.row[2].z + tmp[4] * m1.row[2].x + tmp[9] * m1.row[2].y;
    m.row[3].w -= tmp[8] * m1.row[2].y + tmp[11] * m1.row[2].z + tmp[5] * m1.row[2].x;

    // calculate matrix inverse
    //
    float k = 1.0f / (m1.row[0].x * m.row[0].x + m1.row[0].y * m.row[1].x + m1.row[0].z * m.row[2].x + m1.row[0].w * m.row[3].x);

    for (int i = 0; i < 4; i++)
    {
      m.row[i].x *= k;
      m.row[i].y *= k;
      m.row[i].z *= k;
      m.row[i].w *= k;
    }

    return m;
  }

  // Look At matrix creation
  // return the transposed view matrix
  //
  static inline float4x4 lookAtTransposed(float3 eye, float3 center, float3 up)
  {
    float3 x, y, z; // basis; will make a rotation matrix

    z.x = eye.x - center.x;
    z.y = eye.y - center.y;
    z.z = eye.z - center.z;
    z = normalize(z);

    y.x = up.x;
    y.y = up.y;
    y.z = up.z;

    x = cross(y, z); // X vector = Y cross Z
    y = cross(z, x); // Recompute Y = Z cross X

    // cross product gives area of parallelogram, which is < 1.0 for
    // non-perpendicular unit-length vectors; so normalize x, y here
    x = normalize(x);
    y = normalize(y);

    float4x4 M;
    M.row[0].x = x.x; M.row[1].x = x.y; M.row[2].x = x.z; M.row[3].x = -x.x * eye.x - x.y * eye.y - x.z*eye.z;
    M.row[0].y = y.x; M.row[1].y = y.y; M.row[2].y = y.z; M.row[3].y = -y.x * eye.x - y.y * eye.y - y.z*eye.z;
    M.row[0].z = z.x; M.row[1].z = z.y; M.row[2].z = z.z; M.row[3].z = -z.x * eye.x - z.y * eye.y - z.z*eye.z;
    M.row[0].w = 0.0; M.row[1].w = 0.0; M.row[2].w = 0.0; M.row[3].w = 1.0;
    return M;
  }

   static inline void _my_frustumf3(float *matrix, float left, float right, float bottom, float top, float znear, float zfar)
   {
     float temp, temp2, temp3, temp4;
     temp = 2.0f * znear;
     temp2 = right - left;
     temp3 = top - bottom;
     temp4 = zfar - znear;
     matrix[0] = temp / temp2;
     matrix[1] = 0.0;
     matrix[2] = 0.0;
     matrix[3] = 0.0;
     matrix[4] = 0.0;
     matrix[5] = temp / temp3;
     matrix[6] = 0.0;
     matrix[7] = 0.0;
     matrix[8] = (right + left) / temp2;
     matrix[9] = (top + bottom) / temp3;
     matrix[10] = (-zfar - znear) / temp4;
     matrix[11] = -1.0;
     matrix[12] = 0.0;
     matrix[13] = 0.0;
     matrix[14] = (-temp * zfar) / temp4;
     matrix[15] = 0.0;
   }
   
   static inline float4x4 projectionMatrixTransposed(float fovy, float aspect, float zNear, float zFar)
   {
     float4x4 res;
     const float ymax = zNear * tanf(fovy * 3.14159265358979323846f / 360.0f);
     const float xmax = ymax * aspect;
     _my_frustumf3(res.L(), -xmax, xmax, -ymax, ymax, zNear, zFar);
     return res;
   }

   static inline float4x4 transpose(const float4x4 a_mat)
   {
     float4x4 res;
     res.row[0].x = a_mat.row[0].x;
     res.row[0].y = a_mat.row[1].x;
     res.row[0].z = a_mat.row[2].x;
     res.row[0].w = a_mat.row[3].x;
     res.row[1].x = a_mat.row[0].y;
     res.row[1].y = a_mat.row[1].y;
     res.row[1].z = a_mat.row[2].y;
     res.row[1].w = a_mat.row[3].y;
     res.row[2].x = a_mat.row[0].z;
     res.row[2].y = a_mat.row[1].z;
     res.row[2].z = a_mat.row[2].z;
     res.row[2].w = a_mat.row[3].z;
     res.row[3].x = a_mat.row[0].w;
     res.row[3].y = a_mat.row[1].w;
     res.row[3].z = a_mat.row[2].w;
     res.row[3].w = a_mat.row[3].w;
     return res;
   }


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  static inline int RealColorToUint32_BGRA(float4 real_color)
  {
    const float r = real_color.x*255.0f;
    const float g = real_color.y*255.0f;
    const float b = real_color.z*255.0f;
    const float a = real_color.w*255.0f;
  
    const unsigned char red = (unsigned char)r;
    const unsigned char green = (unsigned char)g;
    const unsigned char blue = (unsigned char)b;
    const unsigned char alpha = (unsigned char)a;
  
    return blue | (green << 8) | (red << 16) | (alpha << 24);
  }

  static inline int RealColorToUint32_RGBA(float4 real_color)
  {
    const float r = real_color.x*255.0f;
    const float g = real_color.y*255.0f;
    const float b = real_color.z*255.0f;
    const float a = real_color.w*255.0f;
  
    const unsigned char red   = (unsigned char)r;
    const unsigned char green = (unsigned char)g;
    const unsigned char blue  = (unsigned char)b;
    const unsigned char alpha = (unsigned char)a;
  
    return red | (green << 8) | (blue << 16) | (alpha << 24);
  }

  static constexpr float DEG_TO_RAD = 3.1415926535897932384626433832795f / 180.0f;

 
}; // namespace LiteMath 
