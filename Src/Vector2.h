//Author: Sören Pirk
//Date: 22.01.2013

#ifndef VECTOR2
#define VECTOR2

#include <math.h>
#include <stdio.h>

#include "Global.h"

class Vector2;

float cross(const Vector2 &a, const Vector2 &b);
Vector2 operator *(float s, const Vector2 &v);
Vector2 normalize(Vector2 &v);
float   dot(const Vector2 &a, const Vector2 &b);
float   length(const Vector2 &v);

class Vector2
{   

public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2 &v);
    Vector2(const float *v);
    ~Vector2();

    float length();
    float dot(const Vector2 &vec);
    float angle(Vector2 &vec);
    float cross(const Vector2 &vec); 
    Vector2 normalized();
    void normalize();
    void set(float x, float y);
    void set(float *vec);

    Vector2 mulMatrix(float *matrix);
    Vector2 &get();

    void print();

    //Assignment
    Vector2 &operator =  (const Vector2 &a);
    Vector2 &operator += (const Vector2 &a);
    Vector2 &operator += (float s);
    Vector2 &operator -= (const Vector2 &a);
    Vector2 &operator -= (float s);
    Vector2 &operator *= (const Vector2 &a);
    Vector2 &operator *= (float s);    
    Vector2 &operator /= (const Vector2 &a);
    Vector2 &operator /= (float s);
	  
    //Arithmetic
    friend Vector2 operator + (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator + (const Vector2 &a, float s);	
    friend Vector2 operator + (float s, const Vector2 &a);	
    friend Vector2 operator - (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator - (const Vector2 &a, float s);
    friend Vector2 operator - (const Vector2 &a);
	friend Vector2 operator * (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator * (const Vector2 &a, float s);
    friend Vector2 operator * (float s, const Vector2 &a);
    friend Vector2 operator / (const Vector2 &a, const Vector2 &b);
    friend Vector2 operator / (const Vector2 &a, float s);

    //Comparison
    friend bool operator == (const Vector2 &a, const Vector2 &b);
    friend bool operator != (const Vector2 &a, const Vector2 &b);
    friend bool operator <= (const Vector2 &a, const Vector2 &b);
    friend bool operator <  (const Vector2 &a, const Vector2 &b);
    friend bool operator >= (const Vector2 &a, const Vector2 &b);
    friend bool operator >  (const Vector2 &a, const Vector2 &b);	
    
    float x;
    float y;   
};


#endif
