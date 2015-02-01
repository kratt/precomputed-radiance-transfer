//Author: Sören Pirk
//Date: 22.01.2013

#ifndef MATRIX3X3
#define MATRIX3X3

#include <math.h>
#include <stdio.h>

#include "Global.h"
#include "Vector3.h"

class Matrix3x3;
class Matrix4x4;

Matrix3x3 operator *(float s, const Matrix3x3 &m);
Vector3 operator *(const Vector3 &v, const Matrix3x3 &m);
Vector3 operator *(const Matrix3x3 &m, const Vector3 &v);
Matrix3x3 transpose(const Matrix3x3 &m);
Matrix3x3 inverse(const Matrix3x3 &m);

class Matrix3x3
{   

public:
    Matrix3x3();
    Matrix3x3(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33);
    Matrix3x3(const Matrix3x3 &m);
    Matrix3x3(const Matrix4x4 &m);
    Matrix3x3(const float *vec);
    ~Matrix3x3();

    void set(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33);
    void set(float *mat);

    Matrix3x3 &get();
    void data(float *vec) const ;

    Matrix3x3 inverse();
    Matrix3x3 transpose();

    void setToIdentity();
    void setToZero();

    float determinant();

    void print();

    //Assignment
    Matrix3x3 &operator =  (const Matrix3x3 &a);
    Matrix3x3 &operator += (const Matrix3x3 &a);
    Matrix3x3 &operator += (float s);
    Matrix3x3 &operator -= (const Matrix3x3 &a);
    Matrix3x3 &operator -= (float s);
    Matrix3x3 &operator *= (const Matrix3x3 &m);
    Matrix3x3 &operator *= (float s);    
    Matrix3x3 &operator /= (float s);
	  
    //Arithmetic
    friend Matrix3x3 operator + (const Matrix3x3 &a, const Matrix3x3 &b);
    friend Matrix3x3 operator + (const Matrix3x3 &a, float s);	
    friend Matrix3x3 operator + (float s, const Matrix3x3 &a);	
    friend Matrix3x3 operator - (const Matrix3x3 &a, const Matrix3x3 &b);
    friend Matrix3x3 operator - (const Matrix3x3 &a, float s);
    friend Matrix3x3 operator - (const Matrix3x3 &a);
	friend Matrix3x3 operator * (const Matrix3x3 &a, const Matrix3x3 &b);    
    friend Matrix3x3 operator * (const Matrix3x3 &a, float s);
    friend Matrix3x3 operator * (float s, const Matrix3x3 &a);
    friend Matrix3x3 operator / (const Matrix3x3 &a, float s);

    //Comparison
    friend bool operator == (const Matrix3x3 &a, const Matrix3x3 &b);
    friend bool operator != (const Matrix3x3 &a, const Matrix3x3 &b);
    
    float a11;
    float a21;
    float a31;    

    float a12;
    float a22;
    float a32;

    float a13;
    float a23;
    float a33;

private:

};


#endif
