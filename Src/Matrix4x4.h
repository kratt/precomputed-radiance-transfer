//Author: Sören Pirk
//Date: 22.01.2013

#ifndef MATRIX4X4
#define MATRIX4X4

#include <math.h>
#include <stdio.h>

#include "Global.h"
#include "Vector4.h"

class Matrix4x4;
class Matrix3x3;

Matrix4x4 operator *(float s, const Matrix4x4 &m);
Vector4 operator *(const Vector4 &v, const Matrix4x4 &m);
Vector4 operator *(const Matrix4x4 &m, const Vector4 &v);
Matrix4x4 transpose(const Matrix4x4 &m);
Matrix4x4 inverse(const Matrix4x4 &m);

class Matrix4x4
{   

public:
    Matrix4x4();
    Matrix4x4(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, 
              float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44);
    Matrix4x4(const Matrix4x4 &m);
    Matrix4x4(const Matrix3x3 &m);

    Matrix4x4(const float *vec);
    ~Matrix4x4();

    void set(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, 
             float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44);
    void set(float *mat);

    Matrix4x4 &get();
    void data(float *vec) const ;

    Matrix4x4 inverse();
    Matrix4x4 transpose();

    void setToIdentity();
    void setToZero();

    float determinant();

    void print();

    //Assignment
    Matrix4x4 &operator =  (const Matrix4x4 &a);
    Matrix4x4 &operator += (const Matrix4x4 &a);
    Matrix4x4 &operator += (float s);
    Matrix4x4 &operator -= (const Matrix4x4 &a);
    Matrix4x4 &operator -= (float s);
    Matrix4x4 &operator *= (const Matrix4x4 &m);
    Matrix4x4 &operator *= (float s);    
    Matrix4x4 &operator /= (float s);
	  
    //Arithmetic
    friend Matrix4x4 operator + (const Matrix4x4 &a, const Matrix4x4 &b);
    friend Matrix4x4 operator + (const Matrix4x4 &a, float s);	
    friend Matrix4x4 operator + (float s, const Matrix4x4 &a);	
    friend Matrix4x4 operator - (const Matrix4x4 &a, const Matrix4x4 &b);
    friend Matrix4x4 operator - (const Matrix4x4 &a, float s);
    friend Matrix4x4 operator - (const Matrix4x4 &a);
	friend Matrix4x4 operator * (const Matrix4x4 &a, const Matrix4x4 &b);    
    friend Matrix4x4 operator * (const Matrix4x4 &a, float s);
    friend Matrix4x4 operator * (float s, const Matrix4x4 &a);
    friend Matrix4x4 operator / (const Matrix4x4 &a, float s);

    //Comparison
    friend bool operator == (const Matrix4x4 &a, const Matrix4x4 &b);
    friend bool operator != (const Matrix4x4 &a, const Matrix4x4 &b);
    
    
    //OpenGL
    static const Matrix4x4 zero();
    static const Matrix4x4 identitiy();
    static const Matrix4x4 rotateX(float angle);
    static const Matrix4x4 rotateY(float angle);
    static const Matrix4x4 rotateZ(float angle);
    static const Matrix4x4 rotate(float angle, float x, float y, float z);
    static const Matrix4x4 rotate(float angle, Vector3 &n);
    static const Matrix4x4 scale(const Vector3 &s);
    static const Matrix4x4 scale(float x, float y, float z);
    static const Matrix4x4 translate(const Vector3 &t);
    static const Matrix4x4 translate(float x, float y, float z);
    static const Matrix4x4 orthographic(float left, float right, float bottom, float top, float zNear, float zFar);
    static const Matrix4x4 perspective(float fov, float ratio, float zNear, float zFar);
    static const Matrix4x4 lookAt(const Vector3 &position, const Vector3 &center, const Vector3 &up);
    static const Matrix4x4 lookAt(float px, float py, float pz, float cx, float cy, float cz, float ux, float uy, float uz);    
    static const Matrix3x3 normalMatrix(const Matrix4x4 &model);

    float a11;
    float a21;
    float a31;  
    float a41;

    float a12;
    float a22;
    float a32;
    float a42;

    float a13;
    float a23;
    float a33;
    float a43;

    float a14;
    float a24;
    float a34;
    float a44;

private:
  
};


#endif
