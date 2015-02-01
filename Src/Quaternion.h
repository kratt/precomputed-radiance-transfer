//Author: Sören Pirk
//Date: 22.01.2013

#ifndef QUATERNION
#define QUATERNION

#include <math.h>
#include <stdio.h>

#include "Misc.h"

#define QUATERNION_PI 3.141592654
#define QUATERNION_ERROR 1e-6

class Quaternion
{   

public:
    Quaternion();
    Quaternion(const float x, const float y, const float z, const float w);
    Quaternion(const Vector3 &v, const float s);
    Quaternion(const float *f);
    Quaternion(const Quaternion &q);
    ~Quaternion();

    float length();
    float length2();
    void normalize();
    Quaternion normalized();

    void set(const float x, const float y, const float z, const float w);
    void set(const Vector3 &v, const float s);
    void set(const float *f);
    void set(const Quaternion &q);

    Quaternion &setEuler(const float Yaw, const float Pitch, const float Roll);
    Quaternion slerp(const Quaternion &from, const Quaternion &to, const float t); 
    
    void toMatrix(float *m);
    void fromAxisAngle(float x, float y, float z, float degrees);

    Quaternion &get();

    void print();

    //Assignment
    Quaternion &operator =  (const Quaternion &q);
    Quaternion &operator += (const Quaternion &q);
    Quaternion &operator -= (const Quaternion &q);
    Quaternion &operator *= (const Quaternion &q);
    Quaternion &operator *= (float s);    
    Quaternion &operator /= (float s);
	  
    //Arithmetic
    Quaternion operator *(const Quaternion &q);

    friend Quaternion operator - (const Quaternion &a);
    friend Quaternion operator + (const Quaternion &a, const Quaternion &b);
    friend Quaternion operator - (const Quaternion &a, const Quaternion &b);    
    friend Quaternion operator * (const Quaternion &a, const float f);
    friend Quaternion operator * (const float f, const Quaternion &a);
    friend Quaternion operator * (const Quaternion &a, const Quaternion &b);
    friend Quaternion operator / (const Quaternion &a, const float f);

    //Comparison
    friend bool operator == (const Quaternion &a, const Quaternion &b);
    friend bool operator != (const Quaternion &a, const Quaternion &b);
    
    float x;
    float y;
    float z;
    float w;

private:
   
  
};


#endif
