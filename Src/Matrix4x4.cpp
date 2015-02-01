//Author: Sören Pirk
//Date: 22.01.2013

#include "Matrix4x4.h"
#include "Matrix3x3.h"

Matrix4x4::Matrix4x4() 
: a11(0.0f),
  a21(0.0f),
  a31(0.0f),
  a41(0.0f),
  a12(0.0f),
  a22(0.0f),
  a32(0.0f),
  a42(0.0f),
  a13(0.0f),
  a23(0.0f),
  a33(0.0f),
  a43(0.0f),
  a14(0.0f),
  a24(0.0f),
  a34(0.0f),
  a44(0.0f)    
{
}

Matrix4x4::Matrix4x4(float a11, float a12, float a13, float a14, 
                     float a21, float a22, float a23, float a24, 
                     float a31, float a32, float a33, float a34, 
                     float a41, float a42, float a43, float a44)
{
   this->a11 = a11; this->a12 = a12; this->a13 = a13; this->a14 = a14;
   this->a21 = a21; this->a22 = a22; this->a23 = a23; this->a24 = a24;
   this->a31 = a31; this->a32 = a32; this->a33 = a33; this->a34 = a34;
   this->a41 = a41; this->a42 = a42; this->a43 = a43; this->a44 = a44;
}

Matrix4x4::Matrix4x4(const Matrix4x4 &m)
{
   this->a11 = m.a11; this->a12 = m.a12; this->a13 = m.a13; this->a14 = m.a14;
   this->a21 = m.a21; this->a22 = m.a22; this->a23 = m.a23; this->a24 = m.a24;
   this->a31 = m.a31; this->a32 = m.a32; this->a33 = m.a33; this->a34 = m.a34;
   this->a41 = m.a41; this->a42 = m.a42; this->a43 = m.a43; this->a44 = m.a44;
}

Matrix4x4::Matrix4x4(const Matrix3x3 &m)
{
   this->a11 = m.a11; this->a12 = m.a12; this->a13 = m.a13; this->a14 = 0.0f;
   this->a21 = m.a21; this->a22 = m.a22; this->a23 = m.a23; this->a24 = 0.0f;
   this->a31 = m.a31; this->a32 = m.a32; this->a33 = m.a33; this->a34 = 0.0f;
   this->a41 = 0.0f;  this->a42 = 0.0f;  this->a43 = 0.0f;  this->a44 = 1.0f;
}

Matrix4x4::Matrix4x4(const float *vec)
{
   this->a11 = vec[0];  this->a12 = vec[1];  this->a13 = vec[2];  this->a14 = vec[3];
   this->a21 = vec[4];  this->a22 = vec[5];  this->a23 = vec[6];  this->a24 = vec[7];
   this->a31 = vec[8];  this->a32 = vec[9];  this->a33 = vec[10]; this->a34 = vec[11];
   this->a41 = vec[12]; this->a42 = vec[13]; this->a43 = vec[14]; this->a44 = vec[15];
}

Matrix4x4::~Matrix4x4() 
{
}

void Matrix4x4::set(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, 
                    float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
{
   this->a11 = a11; this->a12 = a12; this->a13 = a13; this->a14 = a14;
   this->a21 = a21; this->a22 = a22; this->a23 = a23; this->a24 = a24;
   this->a31 = a31; this->a32 = a32; this->a33 = a33; this->a34 = a34;
   this->a41 = a41; this->a42 = a42; this->a43 = a43; this->a44 = a44;
}

void Matrix4x4::set(float *vec)
{
   this->a11 = vec[0];  this->a12 = vec[1];  this->a13 = vec[2];  this->a14 = vec[3];
   this->a21 = vec[4];  this->a22 = vec[5];  this->a23 = vec[6];  this->a24 = vec[7];
   this->a31 = vec[8];  this->a32 = vec[9];  this->a33 = vec[10]; this->a34 = vec[11];
   this->a41 = vec[12]; this->a42 = vec[13]; this->a43 = vec[14]; this->a44 = vec[15];
}

Matrix4x4 &Matrix4x4::get()
{
    return *this;
}

void Matrix4x4::data(float *vec) const
{
    vec[0]  = a11; vec[1]  = a12; vec[2]  = a13; vec[3]  = a14;
    vec[4]  = a21; vec[5]  = a22; vec[6]  = a23; vec[7]  = a24;
    vec[8]  = a31; vec[9]  = a32; vec[10] = a33; vec[11] = a34;
    vec[12] = a41; vec[13] = a42; vec[14] = a43; vec[15] = a44;
}

Matrix4x4 Matrix4x4::inverse()
{
    float D = determinant();
    
    D = (D==0) ? 1 : D;


    Matrix3x3 m11(a22, a23, a24, a32, a33, a34, a42, a43, a44);
    Matrix3x3 m12(a21, a23, a24, a31, a33, a34, a41, a43, a44);
    Matrix3x3 m13(a21, a22, a24, a31, a32, a34, a41, a42, a44);
    Matrix3x3 m14(a21, a22, a23, a31, a32, a33, a41, a42, a43);
    
    Matrix3x3 m21(a12, a13, a14, a32, a33, a34, a42, a43, a44);
    Matrix3x3 m22(a11, a13, a14, a31, a33, a34, a41, a43, a44);
    Matrix3x3 m23(a11, a12, a14, a31, a32, a34, a41, a42, a44);
    Matrix3x3 m24(a11, a12, a13, a31, a32, a33, a41, a42, a43);

    Matrix3x3 m31(a12, a13, a14, a22, a23, a24, a42, a43, a44);
    Matrix3x3 m32(a11, a13, a14, a21, a23, a24, a41, a43, a44);
    Matrix3x3 m33(a11, a12, a14, a21, a22, a24, a41, a42, a44);
    Matrix3x3 m34(a11, a12, a13, a21, a22, a23, a41, a42, a43);

    Matrix3x3 m41(a12, a13, a14, a22, a23, a24, a32, a33, a34);
    Matrix3x3 m42(a11, a13, a14, a21, a23, a24, a31, a33, a34);
    Matrix3x3 m43(a11, a12, a14, a21, a22, a24, a31, a32, a34);
    Matrix3x3 m44(a11, a12, a13, a21, a22, a23, a31, a32, a33);
    
    return Matrix4x4( 
             m11.determinant() / D,
            -m21.determinant() / D,
             m31.determinant() / D,
            -m41.determinant() / D,

            -m12.determinant() / D,
             m22.determinant() / D,
            -m32.determinant() / D,
             m42.determinant() / D,
            
             m13.determinant() / D,
            -m23.determinant() / D,
             m33.determinant() / D,
            -m43.determinant() / D,

            -m14.determinant() / D,
             m24.determinant() / D,
            -m34.determinant() / D,
             m44.determinant() / D
            );   
}

Matrix4x4 Matrix4x4::transpose()
{
    return Matrix4x4(a11, a21, a31, a41, 
                     a12, a22, a32, a42, 
                     a13, a23, a33, a43, 
                     a14, a24, a34, a44);
}

void Matrix4x4::setToIdentity()
{
   this->a11 = 1.0f; this->a12 = 0.0f; this->a13 = 0.0f; this->a14 = 0.0f;
   this->a21 = 0.0f; this->a22 = 1.0f; this->a23 = 0.0f; this->a24 = 0.0f;
   this->a31 = 0.0f; this->a31 = 0.0f; this->a33 = 1.0f; this->a34 = 0.0f;
   this->a41 = 0.0f; this->a41 = 0.0f; this->a43 = 0.0f; this->a44 = 1.0f;
}

void Matrix4x4::setToZero()
{
   this->a11 = 0.0f; this->a12 = 0.0f; this->a13 = 0.0f; this->a14 = 0.0f;
   this->a21 = 0.0f; this->a22 = 0.0f; this->a23 = 0.0f; this->a24 = 0.0f;
   this->a31 = 0.0f; this->a32 = 0.0f; this->a33 = 0.0f; this->a34 = 0.0f;
   this->a41 = 0.0f; this->a42 = 0.0f; this->a43 = 0.0f; this->a44 = 0.0f;
}

//!!!!!!!!!!!!!!!!!!! TODO !!!!!!!!!!!!!!!!!!!!!!!!!!
float Matrix4x4::determinant()
{
    float D1 = (a22 * a33 * a44) + (a23 * a34 * a42) + (a24 * a32 * a43)
            -((a24 * a33 * a42) + (a22 * a34 * a43) + (a23 * a32 * a44));

    float D2 = (a21 * a33 * a44) + (a23 * a34 * a41) + (a24 * a31 * a43)
             -((a24 * a33 * a41) + (a21 * a34 * a43) + (a23 * a31 * a44));

    float D3 = (a21 * a32 * a44) + (a22 * a34 * a41) + (a24 * a31 * a42)
             -((a24 * a32 * a41) + (a21 * a34 * a42) + (a22 * a31 * a44));

    float D4 = (a21 * a32 * a43) + (a22 * a33 * a41) + (a23 * a31 * a42)
             -((a23 * a32 * a41) + (a21 * a33 * a42) + (a22 * a31 * a43));


    float D = a11 * D1 - a12 * D2 + a13 * D3 - a14 * D4;

    return D;
}

void Matrix4x4::print()
{
    printf("a11: %.5f  a12: %.5f  a13: %.5f  a14: %.5f\n"
           "a21: %.5f  a22: %.5f  a23: %.5f  a24: %.5f\n"
           "a31: %.5f  a32: %.5f  a33: %.5f  a34: %.5f\n"
           "a41: %.5f  a42: %.5f  a43: %.5f  a44: %.5f\n",
           a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44);
}

Matrix4x4 &Matrix4x4::operator =(const Matrix4x4 &a)
{
   this->a11 = a.a11; this->a12 = a.a12; this->a13 = a.a13; this->a14 = a.a14;
   this->a21 = a.a21; this->a22 = a.a22; this->a23 = a.a23; this->a24 = a.a24;
   this->a31 = a.a31; this->a32 = a.a32; this->a33 = a.a33; this->a34 = a.a34;
   this->a41 = a.a41; this->a42 = a.a42; this->a43 = a.a43; this->a44 = a.a44;

   return *this;
}

Matrix4x4 &Matrix4x4::operator +=(const Matrix4x4 &a)
{
   this->a11 += a.a11; this->a12 += a.a12; this->a13 += a.a13; this->a14 += a.a14;
   this->a21 += a.a21; this->a22 += a.a22; this->a23 += a.a23; this->a24 += a.a24;
   this->a31 += a.a31; this->a32 += a.a32; this->a33 += a.a33; this->a34 += a.a34;
   this->a41 += a.a41; this->a42 += a.a42; this->a43 += a.a43; this->a44 += a.a44;

   return *this;
}

Matrix4x4 &Matrix4x4::operator +=(float s)
{
   this->a11 += s; this->a12 += s; this->a13 += s; this->a14 += s;
   this->a21 += s; this->a22 += s; this->a23 += s; this->a24 += s;
   this->a31 += s; this->a32 += s; this->a33 += s; this->a34 += s;
   this->a41 += s; this->a42 += s; this->a43 += s; this->a44 += s;

   return *this;
}

Matrix4x4 &Matrix4x4::operator -=(const Matrix4x4 &a)
{
   this->a11 -= a.a11; this->a12 -= a.a12; this->a13 -= a.a13; this->a14 -= a.a14;
   this->a21 -= a.a21; this->a22 -= a.a22; this->a23 -= a.a23; this->a24 -= a.a24;
   this->a31 -= a.a31; this->a32 -= a.a32; this->a33 -= a.a33; this->a34 -= a.a34;
   this->a41 -= a.a41; this->a42 -= a.a42; this->a43 -= a.a43; this->a44 -= a.a44;

   return *this;
}

Matrix4x4 &Matrix4x4::operator -=(float s)
{
   this->a11 -= s; this->a12 -= s; this->a13 -= s; this->a14 -= s;
   this->a21 -= s; this->a22 -= s; this->a23 -= s; this->a24 -= s;
   this->a31 -= s; this->a32 -= s; this->a33 -= s; this->a34 -= s;
   this->a41 -= s; this->a42 -= s; this->a43 -= s; this->a44 -= s;

   return *this;
}

Matrix4x4 &Matrix4x4::operator *=(const Matrix4x4 &m)
{
   Matrix4x4 c;

   c.a11 = this->a11*m.a11 + this->a12*m.a21 + this->a13*m.a31 + this->a14*m.a41;
   c.a12 = this->a11*m.a12 + this->a12*m.a22 + this->a13*m.a32 + this->a14*m.a42;   
   c.a13 = this->a11*m.a13 + this->a12*m.a23 + this->a13*m.a33 + this->a14*m.a43;
   c.a14 = this->a11*m.a14 + this->a12*m.a24 + this->a13*m.a34 + this->a14*m.a44;

   c.a21 = this->a21*m.a11 + this->a22*m.a21 + this->a23*m.a31 + this->a24*m.a41;
   c.a22 = this->a21*m.a12 + this->a22*m.a22 + this->a23*m.a32 + this->a24*m.a42;
   c.a23 = this->a21*m.a13 + this->a22*m.a23 + this->a23*m.a33 + this->a24*m.a43;
   c.a24 = this->a21*m.a14 + this->a22*m.a24 + this->a23*m.a34 + this->a24*m.a44;

   c.a31 = this->a31*m.a11 + this->a32*m.a21 + this->a33*m.a31 + this->a34*m.a41;
   c.a32 = this->a31*m.a12 + this->a32*m.a22 + this->a33*m.a32 + this->a34*m.a42;
   c.a33 = this->a31*m.a13 + this->a32*m.a23 + this->a33*m.a33 + this->a34*m.a43;
   c.a34 = this->a31*m.a14 + this->a32*m.a24 + this->a33*m.a34 + this->a34*m.a44;

   c.a41 = this->a41*m.a11 + this->a42*m.a21 + this->a43*m.a31 + this->a44*m.a41;
   c.a42 = this->a41*m.a12 + this->a42*m.a22 + this->a43*m.a32 + this->a44*m.a42;
   c.a43 = this->a41*m.a13 + this->a42*m.a23 + this->a43*m.a33 + this->a44*m.a43;
   c.a44 = this->a41*m.a14 + this->a42*m.a24 + this->a43*m.a34 + this->a44*m.a44;


   this->a11 = c.a11;
   this->a21 = c.a21;
   this->a31 = c.a31;
   this->a41 = c.a41;

   this->a12 = c.a12;
   this->a22 = c.a22;
   this->a32 = c.a32;
   this->a42 = c.a42;

   this->a13 = c.a13;
   this->a23 = c.a23;
   this->a33 = c.a33;
   this->a43 = c.a43;

   this->a14 = c.a14;
   this->a24 = c.a24;
   this->a34 = c.a34;
   this->a44 = c.a44;

   return *this;
}

Matrix4x4 &Matrix4x4::operator *=(float s)
{
   this->a11 *= s; this->a12 *= s; this->a13 *= s; this->a14 *= s;
   this->a21 *= s; this->a22 *= s; this->a23 *= s; this->a24 *= s;
   this->a31 *= s; this->a32 *= s; this->a33 *= s; this->a34 *= s;
   this->a41 *= s; this->a42 *= s; this->a43 *= s; this->a44 *= s;

   return *this;
}

Matrix4x4 &Matrix4x4::operator /=(float s)
{
   this->a11 /= s; this->a12 /= s; this->a13 /= s; this->a14 /= s;
   this->a21 /= s; this->a22 /= s; this->a23 /= s; this->a24 /= s;
   this->a31 /= s; this->a32 /= s; this->a33 /= s; this->a34 /= s;
   this->a41 /= s; this->a42 /= s; this->a43 /= s; this->a44 /= s;

   return *this;
}

//-----------------------------------------------------------------------------------------------

Matrix4x4 operator +(const Matrix4x4 &a, const Matrix4x4 &b)
{
   Matrix4x4 r;

   r.a11 = a.a11 + b.a11;  r.a12 = a.a12 + b.a12;  r.a13 = a.a13 + b.a13;  r.a14 = a.a14 + b.a14;
   r.a21 = a.a21 + b.a21;  r.a22 = a.a22 + b.a22;  r.a23 = a.a23 + b.a23;  r.a24 = a.a24 + b.a24;
   r.a31 = a.a31 + b.a31;  r.a32 = a.a32 + b.a32;  r.a33 = a.a33 + b.a33;  r.a34 = a.a34 + b.a34;
   r.a41 = a.a41 + b.a41;  r.a42 = a.a42 + b.a42;  r.a43 = a.a43 + b.a43;  r.a44 = a.a44 + b.a44;

   return r;
}

Matrix4x4 operator +(const Matrix4x4 &a, float s)
{
   Matrix4x4 r;

   r.a11 = a.a11 + s;  r.a12 = a.a12 + s;  r.a13 = a.a13 + s;  r.a14 = a.a14 + s;
   r.a21 = a.a21 + s;  r.a22 = a.a22 + s;  r.a23 = a.a23 + s;  r.a24 = a.a24 + s;
   r.a31 = a.a31 + s;  r.a32 = a.a32 + s;  r.a33 = a.a33 + s;  r.a34 = a.a34 + s;
   r.a31 = a.a41 + s;  r.a42 = a.a42 + s;  r.a43 = a.a43 + s;  r.a44 = a.a44 + s;

   return r;
}

Matrix4x4 operator +(float s, const Matrix4x4 &a)
{
   Matrix4x4 r;

   r.a11 = a.a11 + s;  r.a12 = a.a12 + s;  r.a13 = a.a13 + s;  r.a14 = a.a14 + s;
   r.a21 = a.a21 + s;  r.a22 = a.a22 + s;  r.a23 = a.a23 + s;  r.a24 = a.a24 + s;
   r.a31 = a.a31 + s;  r.a32 = a.a32 + s;  r.a33 = a.a33 + s;  r.a34 = a.a34 + s;
   r.a41 = a.a41 + s;  r.a42 = a.a42 + s;  r.a43 = a.a43 + s;  r.a44 = a.a44 + s;

   return r;
}

Matrix4x4 operator -(const Matrix4x4 &a, const Matrix4x4 &b)
{
   Matrix4x4 r;

   r.a11 = a.a11 - b.a11;  r.a12 = a.a12 - b.a12;  r.a13 = a.a13 - b.a13;  r.a14 = a.a14 - b.a14;
   r.a21 = a.a21 - b.a21;  r.a22 = a.a22 - b.a22;  r.a23 = a.a23 - b.a23;  r.a24 = a.a24 - b.a24;
   r.a31 = a.a31 - b.a31;  r.a32 = a.a32 - b.a32;  r.a33 = a.a33 - b.a33;  r.a34 = a.a34 - b.a34;
   r.a41 = a.a41 - b.a41;  r.a42 = a.a42 - b.a42;  r.a43 = a.a43 - b.a43;  r.a44 = a.a44 - b.a44;

   return r;
}

Matrix4x4 operator -(const Matrix4x4 &a, float s)
{
   Matrix4x4 r;

   r.a11 = a.a11 - s;  r.a12 = a.a12 - s;  r.a13 = a.a13 - s;  r.a14 = a.a14 - s;
   r.a21 = a.a21 - s;  r.a22 = a.a22 - s;  r.a23 = a.a23 - s;  r.a24 = a.a24 - s;
   r.a31 = a.a31 - s;  r.a32 = a.a32 - s;  r.a33 = a.a33 - s;  r.a34 = a.a34 - s;
   r.a41 = a.a41 - s;  r.a42 = a.a42 - s;  r.a43 = a.a43 - s;  r.a44 = a.a44 - s;

   return r;
}

Matrix4x4 operator -(const Matrix4x4 &a)
{
    Matrix4x4 r;
	
   r.a11 = -a.a11;  r.a12 = -a.a12;  r.a13 = -a.a13;  r.a14 = -a.a14;
   r.a21 = -a.a21;  r.a22 = -a.a22;  r.a23 = -a.a23;  r.a24 = -a.a24; 
   r.a31 = -a.a31;  r.a32 = -a.a32;  r.a33 = -a.a33;  r.a34 = -a.a34;
   r.a41 = -a.a41;  r.a42 = -a.a42;  r.a43 = -a.a43;  r.a44 = -a.a44;
	
	return r;
}

Matrix4x4 operator *(const Matrix4x4 &a, float s)
{
   Matrix4x4 r;

   r.a11 = a.a11 * s;  r.a12 = a.a12 * s;  r.a13 = a.a13 * s;  r.a14 = a.a14 * s;
   r.a21 = a.a21 * s;  r.a22 = a.a22 * s;  r.a23 = a.a23 * s;  r.a24 = a.a24 * s;
   r.a31 = a.a31 * s;  r.a32 = a.a32 * s;  r.a33 = a.a33 * s;  r.a34 = a.a34 * s;
   r.a41 = a.a41 * s;  r.a42 = a.a42 * s;  r.a43 = a.a43 * s;  r.a44 = a.a44 * s;

   return r;
}

Matrix4x4 operator *(const Matrix4x4 &a, const Matrix4x4 &b)
{
   Matrix4x4 c;

   c.a11 = a.a11*b.a11 + a.a12*b.a21 + a.a13*b.a31 + a.a14*b.a41;
   c.a12 = a.a11*b.a12 + a.a12*b.a22 + a.a13*b.a32 + a.a14*b.a42;   
   c.a13 = a.a11*b.a13 + a.a12*b.a23 + a.a13*b.a33 + a.a14*b.a43;
   c.a14 = a.a11*b.a14 + a.a12*b.a24 + a.a13*b.a34 + a.a14*b.a44;

   c.a21 = a.a21*b.a11 + a.a22*b.a21 + a.a23*b.a31 + a.a24*b.a41;
   c.a22 = a.a21*b.a12 + a.a22*b.a22 + a.a23*b.a32 + a.a24*b.a42;
   c.a23 = a.a21*b.a13 + a.a22*b.a23 + a.a23*b.a33 + a.a24*b.a43;
   c.a24 = a.a21*b.a14 + a.a22*b.a24 + a.a23*b.a34 + a.a24*b.a44;

   c.a31 = a.a31*b.a11 + a.a32*b.a21 + a.a33*b.a31 + a.a34*b.a41;
   c.a32 = a.a31*b.a12 + a.a32*b.a22 + a.a33*b.a32 + a.a34*b.a42;
   c.a33 = a.a31*b.a13 + a.a32*b.a23 + a.a33*b.a33 + a.a34*b.a43;
   c.a34 = a.a31*b.a14 + a.a32*b.a24 + a.a33*b.a34 + a.a34*b.a44;

   c.a41 = a.a41*b.a11 + a.a42*b.a21 + a.a43*b.a31 + a.a44*b.a41;
   c.a42 = a.a41*b.a12 + a.a42*b.a22 + a.a43*b.a32 + a.a44*b.a42;
   c.a43 = a.a41*b.a13 + a.a42*b.a23 + a.a43*b.a33 + a.a44*b.a43;
   c.a44 = a.a41*b.a14 + a.a42*b.a24 + a.a43*b.a34 + a.a44*b.a44;

   return c;

		//return mat4(a.a11 * b.a11 + a.a12 * b.a21 + a.a13 * b.a31 + a.a14 * b.a41,
		//            a.a11 * b.a12 + a.a12 * b.a22 + a.a13 * b.a32 + a.a14 * b.a42,
		//            a.a11 * b.a13 + a.a12 * b.a23 + a.a13 * b.a33 + a.a14 * b.a43,
		//            a.a11 * b.a14 + a.a12 * b.a24 + a.a13 * b.a34 + a.a14 * b.a44,
		//            a.a21 * b.a11 + a.a22 * b.a21 + a.a23 * b.a31 + a.a24 * b.a41,
		//            a.a21 * b.a12 + a.a22 * b.a22 + a.a23 * b.a32 + a.a24 * b.a42,
		//            a.a21 * b.a13 + a.a22 * b.a23 + a.a23 * b.a33 + a.a24 * b.a43,
		//            a.a21 * b.a14 + a.a22 * b.a24 + a.a23 * b.a34 + a.a24 * b.a44,
		//            a.a31 * b.a11 + a.a32 * b.a21 + a.a33 * b.a31 + a.a34 * b.a41,
		//            a.a31 * b.a12 + a.a32 * b.a22 + a.a33 * b.a32 + a.a34 * b.a42,
		//            a.a31 * b.a13 + a.a32 * b.a23 + a.a33 * b.a33 + a.a34 * b.a43,
		//            a.a31 * b.a14 + a.a32 * b.a24 + a.a33 * b.a34 + a.a34 * b.a44,
		//            a.a41 * b.a11 + a.a42 * b.a21 + a.a43 * b.a31 + a.a44 * b.a41,
		//            a.a41 * b.a12 + a.a42 * b.a22 + a.a43 * b.a32 + a.a44 * b.a42,
		//            a.a41 * b.a13 + a.a42 * b.a23 + a.a43 * b.a33 + a.a44 * b.a43,
		//            a.a41 * b.a14 + a.a42 * b.a24 + a.a43 * b.a34 + a.a44 * b.a44);

}

Matrix4x4 operator /(const Matrix4x4 &a, float s)
{
   Matrix4x4 r;

   r.a11 = a.a11 / s;  r.a12 = a.a12 / s;  r.a13 = a.a13 / s;  r.a14 = a.a14 / s;
   r.a21 = a.a21 / s;  r.a22 = a.a22 / s;  r.a23 = a.a23 / s;  r.a24 = a.a24 / s;
   r.a31 = a.a31 / s;  r.a32 = a.a32 / s;  r.a33 = a.a33 / s;  r.a34 = a.a34 / s;
   r.a41 = a.a41 / s;  r.a42 = a.a42 / s;  r.a43 = a.a43 / s;  r.a44 = a.a44 / s;

   return r;
}

//-----------------------------------------------------------------------------------------------

bool operator == (const Matrix4x4 &a, const Matrix4x4 &b)
{
    return(a.a11 == b.a11 && a.a12 == b.a12 && a.a13 == b.a13 && a.a14 == b.a14 &&
           a.a21 == b.a21 && a.a22 == b.a22 && a.a23 == b.a23 && a.a24 == b.a24 &&
           a.a31 == b.a31 && a.a32 == b.a32 && a.a33 == b.a33 && a.a34 == b.a34 &&
           a.a41 == b.a41 && a.a42 == b.a42 && a.a43 == b.a43 && a.a44 == b.a44);
}

bool operator != (const Matrix4x4 &a, const Matrix4x4 &b)
{
    return(a.a11 != b.a11 || a.a12 != b.a12 || a.a13 != b.a13 || a.a14 != b.a14 || 
           a.a21 != b.a21 || a.a22 != b.a22 || a.a23 != b.a23 || a.a24 != b.a24 || 
           a.a31 != b.a31 || a.a32 != b.a32 || a.a33 != b.a33 || a.a34 != b.a34 || 
           a.a31 != b.a41 || a.a42 != b.a42 || a.a43 != b.a43 || a.a44 != b.a44);
}

//-----------------------------------------------------------------------------------------------

Matrix4x4 operator *(float s, const Matrix4x4 &m)
{
    return (m * s);
}

Vector4 operator *(const Matrix4x4 &m, const Vector4 &v)
{
    Vector4 r;

    r.x = m.a11 * v.x + m.a12 * v.y + m.a13 * v.z + m.a14 * v.w;
    r.y = m.a21 * v.x + m.a22 * v.y + m.a23 * v.z + m.a24 * v.w;
    r.z = m.a31 * v.x + m.a32 * v.y + m.a33 * v.z + m.a34 * v.w;
    r.w = m.a41 * v.x + m.a42 * v.y + m.a43 * v.z + m.a44 * v.w;

    return r;
}

//!!!!!!!!!!!!!!!!!!!!!!! richtig?
Vector4 operator *(const Vector4 &v, const Matrix4x4 &m)
{
    Vector4 r;

    r.x = m.a11 * v.x + m.a21 * v.y + m.a31 * v.z + m.a41 * v.w;
    r.y = m.a12 * v.x + m.a22 * v.y + m.a32 * v.z + m.a42 * v.w;
    r.z = m.a13 * v.x + m.a23 * v.y + m.a33 * v.z + m.a43 * v.w;
    r.w = m.a14 * v.x + m.a24 * v.y + m.a34 * v.z + m.a44 * v.w;

    return r;
}

Matrix4x4 transpose(const Matrix4x4 &m)
{
    Matrix4x4 mat(m);
    return mat.transpose();
}

Matrix4x4 inverse(const Matrix4x4 &m)
{
    Matrix4x4 mat(m);
    return mat.inverse();
}

//-----------------------------------------------------------------------------------------------
const Matrix4x4 Matrix4x4::zero()
{
	return Matrix4x4(0, 0, 0, 0,
	                 0, 0, 0, 0,
	                 0, 0, 0, 0,
	                 0, 0, 0, 0);
}


const Matrix4x4 Matrix4x4::identitiy()
{
	return Matrix4x4(1, 0, 0, 0,
	                 0, 1, 0, 0,
	                 0, 0, 1, 0,
	                 0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::rotateX(float angle)
{
	const float c = cosf(angle * math_radians);
    const float s = sinf(angle * math_radians);

	return Matrix4x4(1, 0, 0, 0,
	                 0, c,-s, 0,
	                 0, s, c, 0,
	                 0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::rotateY(float angle)
{
	const float c = cosf(angle * math_radians);
    const float s = sinf(angle * math_radians);

	return Matrix4x4(c, 0, s, 0,
	                 0, 1, 0, 0,
	                -s, 0, c, 0,
	                 0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::rotateZ(float angle)
{
    const float c = cosf(angle * math_radians);
    const float s = sinf(angle * math_radians);

    return Matrix4x4(c,-s, 0, 0,
                     s, c, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::rotate(float angle, float x, float y, float z)
{
    Vector3 n(x, y, z);
    return rotate(angle, n);
}

const Matrix4x4 Matrix4x4::rotate(float angle, Vector3 &n)
{    
    n.normalize();

    const float c = cosf(angle * math_radians);
    const float ac = 1.0f - c;
    const float s = sinf(angle * math_radians);

    float m11 = n.x * n.x * ac + c;
    float m12 = n.x * n.y * ac + n.z * s;
    float m13 = n.x * n.z * ac - n.y * s;

    float m21 = n.y * n.x * ac - n.z * s;
    float m22 = n.y * n.y * ac + c;
    float m23 = n.y * n.z * ac + n.x * s;

    float m31 = n.z * n.x * ac + n.y * s;
    float m32 = n.z * n.y * ac - n.x * s;
    float m33 = n.z * n.z * ac + c;

    return Matrix4x4( m11,  m12,  m13, 0.0f, 
                      m21,  m22,  m23, 0.0f,
                      m31,  m32,  m33, 0.0f, 
                      0.0f, 0.0f, 0.0f, 1.0f);
}

const Matrix4x4 Matrix4x4::scale(float x, float y, float z)
{
	return Matrix4x4(x, 0, 0, 0,
	                 0, y, 0, 0,
	                 0, 0, z, 0,
	                 0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::scale(const Vector3 &s)
{
    return scale(s.x, s.y, s.z);
}

const Matrix4x4 Matrix4x4::translate(float x, float y, float z)
{
	return Matrix4x4(1, 0, 0, x,
	                 0, 1, 0, y,
	                 0, 0, 1, z,
                     0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::translate(const Vector3 &t)
{
    return translate(t.x, t.y, t.z);
}

const Matrix4x4 Matrix4x4::orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
    const float tx = - (right + left) / (right - left);
	const float ty = - (top + bottom) / (top - bottom);
    const float tz = - (zFar + zNear) / (zFar - zNear);

	return Matrix4x4(2 / (right - left), 0, 0, tx,
	                 0, 2 / (top - bottom), 0, ty,
	                 0, 0, -2 / (zFar - zNear), tz,
	                 0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::perspective(float fov, float ratio, float zNear, float zFar)
{
    const float aspect = ratio;

	const float f = 1.0f / tanf(fov * math_radians / 2.0f);
	const float A = (zFar + zNear) / (zNear - zFar);
    const float B = (2.0f * zFar * zNear) / (zNear - zFar);

    return Matrix4x4(f / aspect, 0.0f, 0.0f, 0.0f,
	                 0.0f, f, 0.0f, 0.0f,
	                 0.0f, 0.0f, A, B,
	                 0.0f, 0.0f, -1.0f, 0.0f);
}

const Matrix4x4 Matrix4x4::lookAt(const Vector3 &position, const Vector3 &center, const Vector3 &up)
{
	const vec3 f = normalize(position - center);
	const vec3 s = normalize(cross(up, f));
	const vec3 u = normalize(cross(f, s));

	return Matrix4x4(s.x, s.y, s.z, -dot(s, position),
	                 u.x, u.y, u.z, -dot(u, position),
	                 f.x, f.y, f.z, -dot(f, position),
                     0, 0, 0, 1);
}

const Matrix4x4 Matrix4x4::lookAt(float px, float py, float pz, float cx, float cy, float cz, float ux, float uy, float uz)
{
    return lookAt(Vector3(px, py, pz), Vector3(cx, cy, cz), Vector3(ux, uy, uz));
}

const Matrix3x3 Matrix4x4::normalMatrix(const Matrix4x4 &model)
{
    Matrix4x4 m(model);
    Matrix3x3 inv = m.inverse();
    
    return inv.transpose();
}