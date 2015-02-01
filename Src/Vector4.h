//Author: Sören Pirk
//Date: 22.01.2013

#ifndef VECTOR4
#define VECTOR4

#include <math.h>
#include <stdio.h>

#include "Global.h"

class Vector4;
class Vector3;

Vector4 cross(const Vector4 &a, const Vector4 &b);
Vector4 operator *(float s, const Vector4 &v);
Vector4 normalize(Vector4 &v);
float   dot(const Vector4 &a, const Vector4 &b);
float   length(const Vector4 &v);

class Vector4
{   

public:
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector4 &vec);
    Vector4(const Vector3 &vec);
    Vector4(const float *vec);
    Vector4(float x);
    ~Vector4();

    float length();
    float dot(const Vector4 &vec);
    float angle(Vector4 &vec);
    Vector4 cross(const Vector4 &vec); 
    Vector4 normalized();
    void normalize();
    void set(float x, float y, float z, float w);
    void set(float *vec);

    Vector4 &get();

    void print();

    //Assignment
    Vector4 &operator =  (const Vector4 &a);
    Vector4 &operator += (const Vector4 &a);
    Vector4 &operator += (float s);
    Vector4 &operator -= (const Vector4 &a);
    Vector4 &operator -= (float s);
    Vector4 &operator *= (const Vector4 &a);
    Vector4 &operator *= (float s);    
    Vector4 &operator /= (const Vector4 &a);
    Vector4 &operator /= (float s);
	  
    //Arithmetic
    friend Vector4 operator + (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator + (const Vector4 &a, float s);	
    friend Vector4 operator + (float s, const Vector4 &a);	
    friend Vector4 operator - (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator - (const Vector4 &a, float s);
    friend Vector4 operator - (const Vector4 &a);
	friend Vector4 operator * (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator * (const Vector4 &a, float s);
    friend Vector4 operator * (float s, const Vector4 &a);
    friend Vector4 operator / (const Vector4 &a, const Vector4 &b);
    friend Vector4 operator / (const Vector4 &a, float s);

    //Comparison
    friend bool operator == (const Vector4 &a, const Vector4 &b);
    friend bool operator != (const Vector4 &a, const Vector4 &b);
    friend bool operator <= (const Vector4 &a, const Vector4 &b);
    friend bool operator <  (const Vector4 &a, const Vector4 &b);
    friend bool operator >= (const Vector4 &a, const Vector4 &b);
    friend bool operator >  (const Vector4 &a, const Vector4 &b);	
    
    float x;
    float y;
    float z;  
    float w;
};


#endif
