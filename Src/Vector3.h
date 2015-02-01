//Author: Sören Pirk
//Date: 22.01.2013

#ifndef VECTOR3
#define VECTOR3

#include <math.h>
#include <stdio.h>

#include "Global.h"

class Vector3;

Vector3 cross(const Vector3 &a, const Vector3 &b);
Vector3 operator *(float s, const Vector3 &v);
Vector3 normalize(Vector3 &v);
float   dot(const Vector3 &a, const Vector3 &b);
float   length(const Vector3 &v);
Vector3 rnd(float low, float high);

class Vector3
{   

public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3 &v);
    Vector3(const float *v);
    Vector3(float x);
    ~Vector3();

    float length();
    float dot(const Vector3 &vec);
    float angle(Vector3 &vec);
    Vector3 cross(const Vector3 &vec); 
    Vector3 normalized();
    void normalize();
    void set(float x, float y, float z);
    void set(float *vec);

    Vector3 mulMatrix(float *matrix);

    Vector3 &get();

    void print();

    //Assignment
    Vector3 &operator =  (const Vector3 &a);
    Vector3 &operator += (const Vector3 &a);
    Vector3 &operator += (float s);
    Vector3 &operator -= (const Vector3 &a);
    Vector3 &operator -= (float s);
    Vector3 &operator *= (const Vector3 &a);
    Vector3 &operator *= (float s);    
    Vector3 &operator /= (const Vector3 &a);
    Vector3 &operator /= (float s);
	  
    //Arithmetic
    friend Vector3 operator + (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator + (const Vector3 &a, float s);	
    friend Vector3 operator + (float s, const Vector3 &a);	
    friend Vector3 operator - (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator - (const Vector3 &a, float s);
    friend Vector3 operator - (const Vector3 &a);
	friend Vector3 operator * (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator * (const Vector3 &a, float s);
    friend Vector3 operator * (float s, const Vector3 &a);
    friend Vector3 operator / (const Vector3 &a, const Vector3 &b);
    friend Vector3 operator / (const Vector3 &a, float s);

    //Comparison
    friend bool operator == (const Vector3 &a, const Vector3 &b);
    friend bool operator != (const Vector3 &a, const Vector3 &b);
    friend bool operator <= (const Vector3 &a, const Vector3 &b);
    friend bool operator <  (const Vector3 &a, const Vector3 &b);
    friend bool operator >= (const Vector3 &a, const Vector3 &b);
    friend bool operator >  (const Vector3 &a, const Vector3 &b);	
    
    float x;
    float y;
    float z;    
};


#endif
