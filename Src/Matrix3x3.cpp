//Author: Sören Pirk
//Date: 22.01.2013

#include "Matrix3x3.h"
#include "Matrix4x4.h"

Matrix3x3::Matrix3x3() 
: a11(0.0),
  a21(0.0),
  a31(0.0),
  a12(0.0),
  a22(0.0),
  a32(0.0),
  a13(0.0),
  a23(0.0),
  a33(0.0)
{
}

Matrix3x3::Matrix3x3(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33)
{
   this->a11 = a11; this->a12 = a12; this->a13 = a13;
   this->a21 = a21; this->a22 = a22; this->a23 = a23;
   this->a31 = a31; this->a32 = a32; this->a33 = a33;
}

Matrix3x3::Matrix3x3(const Matrix3x3 &m)
{
   this->a11 = m.a11; this->a12 = m.a12; this->a13 = m.a13;
   this->a21 = m.a21; this->a22 = m.a22; this->a23 = m.a23;
   this->a31 = m.a31; this->a32 = m.a32; this->a33 = m.a33;
}

Matrix3x3::Matrix3x3(const Matrix4x4 &m)
{
   this->a11 = m.a11; this->a12 = m.a12; this->a13 = m.a13;
   this->a21 = m.a21; this->a22 = m.a22; this->a23 = m.a23;
   this->a31 = m.a31; this->a32 = m.a32; this->a33 = m.a33;
}

Matrix3x3::Matrix3x3(const float *vec)
{
   this->a11 = vec[0]; this->a12 = vec[1]; this->a13 = vec[2];
   this->a21 = vec[3]; this->a22 = vec[4]; this->a23 = vec[5];
   this->a31 = vec[6]; this->a32 = vec[7]; this->a33 = vec[8];
}

Matrix3x3::~Matrix3x3() 
{
}

void Matrix3x3::set(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33)
{
   this->a11 = a11; this->a12 = a12; this->a13 = a13;
   this->a21 = a21; this->a22 = a22; this->a23 = a23;
   this->a31 = a31; this->a32 = a32; this->a33 = a33;
}

void Matrix3x3::set(float *vec)
{
   this->a11 = vec[0]; this->a12 = vec[1]; this->a13 = vec[2];
   this->a21 = vec[3]; this->a22 = vec[4]; this->a23 = vec[5];
   this->a31 = vec[6]; this->a32 = vec[7]; this->a33 = vec[8];
}

Matrix3x3 &Matrix3x3::get()
{
    return *this;
}

void Matrix3x3::data(float *vec) const
{
    vec[0]  = a11; vec[1]  = a12; vec[2]  = a13; 
    vec[3]  = a21; vec[4]  = a22; vec[5]  = a23; 
    vec[6]  = a31; vec[7]  = a32; vec[8]  = a33; 
}

Matrix3x3 Matrix3x3::inverse()
{
    float D = determinant();
    
    D = (D==0) ? 1 : D;
    
    return Matrix3x3( 
             (a22 * a33 - a23 * a32) / D,
            -(a12 * a33 - a13 * a32) / D,
             (a12 * a23 - a13 * a22) / D,
            -(a21 * a33 - a23 * a31) / D,
             (a11 * a33 - a13 * a31) / D,
            -(a11 * a23 - a13 * a21) / D,
             (a21 * a32 - a22 * a31) / D,
            -(a11 * a32 - a12 * a31) / D,
             (a11 * a22 - a12 * a21) / D );   
}

Matrix3x3 Matrix3x3::transpose()
{
    return Matrix3x3(a11, a21, a31, a12, a22, a32, a13, a23, a33);
}

void Matrix3x3::setToIdentity()
{
   this->a11 = 1.0f; this->a12 = 0.0f; this->a13 = 0.0f;
   this->a21 = 0.0f; this->a22 = 1.0f; this->a23 = 0.0f;
   this->a31 = 0.0f; this->a31 = 0.0f; this->a33 = 1.0f;
}

void Matrix3x3::setToZero()
{
   this->a11 = 0.0f; this->a12 = 0.0f; this->a13 = 0.0f;
   this->a21 = 0.0f; this->a22 = 0.0f; this->a23 = 0.0f;
   this->a31 = 0.0f; this->a32 = 0.0f; this->a33 = 0.0f;
}

float Matrix3x3::determinant()
{
    float D = (a11 * a22 * a33) + (a12 * a23 * a31) + (a13 * a21 * a32)
            -((a13 * a22 * a31) + (a11 * a23 * a32) + (a12 * a21 * a33));

    return D;
}

void Matrix3x3::print()
{
    printf("a11: %.5f  a12: %.5f  a13: %.5f\na21: %.5f  a22: %.5f  a23: %.5f\na31: %.5f  a32: %.5f  a33: %.5f\n",
           a11, a12, a13, a21, a22, a23, a31, a32, a33);
}

Matrix3x3 &Matrix3x3::operator =(const Matrix3x3 &a)
{
   this->a11 = a.a11; this->a12 = a.a12; this->a13 = a.a13;
   this->a21 = a.a21; this->a22 = a.a22; this->a23 = a.a23;
   this->a31 = a.a31; this->a32 = a.a32; this->a33 = a.a33;

   return *this;
}

Matrix3x3 &Matrix3x3::operator +=(const Matrix3x3 &a)
{
   this->a11 += a.a11; this->a12 += a.a12; this->a13 += a.a13;
   this->a21 += a.a21; this->a22 += a.a22; this->a23 += a.a23;
   this->a31 += a.a31; this->a32 += a.a32; this->a33 += a.a33;

   return *this;
}

Matrix3x3 &Matrix3x3::operator +=(float s)
{
   this->a11 += s; this->a12 += s; this->a13 += s;
   this->a21 += s; this->a22 += s; this->a23 += s;
   this->a31 += s; this->a32 += s; this->a33 += s;

   return *this;
}

Matrix3x3 &Matrix3x3::operator -=(const Matrix3x3 &a)
{
   this->a11 -= a.a11; this->a12 -= a.a12; this->a13 -= a.a13;
   this->a21 -= a.a21; this->a22 -= a.a22; this->a23 -= a.a23;
   this->a31 -= a.a31; this->a32 -= a.a32; this->a33 -= a.a33;

   return *this;
}

Matrix3x3 &Matrix3x3::operator -=(float s)
{
   this->a11 -= s; this->a12 -= s; this->a13 -= s;
   this->a21 -= s; this->a22 -= s; this->a23 -= s;
   this->a31 -= s; this->a32 -= s; this->a33 -= s;

   return *this;
}

Matrix3x3 &Matrix3x3::operator *=(const Matrix3x3 &m)
{
   Matrix3x3 c;

   c.a11 = this->a11*m.a11 + this->a12*m.a21 + this->a13*m.a31;
   c.a21 = this->a21*m.a11 + this->a22*m.a21 + this->a23*m.a31;
   c.a31 = this->a31*m.a11 + this->a32*m.a21 + this->a33*m.a31;

   c.a12 = this->a11*m.a12 + this->a12*m.a22 + this->a13*m.a32;
   c.a22 = this->a21*m.a12 + this->a22*m.a22 + this->a23*m.a32;
   c.a32 = this->a31*m.a12 + this->a32*m.a22 + this->a33*m.a32;

   c.a13 = this->a11*m.a13 + this->a12*m.a23 + this->a13*m.a33;
   c.a23 = this->a21*m.a13 + this->a22*m.a23 + this->a23*m.a33;
   c.a33 = this->a31*m.a13 + this->a32*m.a23 + this->a33*m.a33;

   this->a11 = c.a11;
   this->a21 = c.a21;
   this->a31 = c.a31;

   this->a12 = c.a12;
   this->a22 = c.a22;
   this->a32 = c.a32;

   this->a13 = c.a13;
   this->a23 = c.a23;
   this->a33 = c.a33;

   return *this;
}

Matrix3x3 &Matrix3x3::operator *=(float s)
{
   this->a11 *= s; this->a12 *= s; this->a13 *= s;
   this->a21 *= s; this->a22 *= s; this->a23 *= s;
   this->a31 *= s; this->a32 *= s; this->a33 *= s;

   return *this;
}

Matrix3x3 &Matrix3x3::operator /=(float s)
{
   this->a11 /= s; this->a12 /= s; this->a13 /= s;
   this->a21 /= s; this->a22 /= s; this->a23 /= s;
   this->a31 /= s; this->a32 /= s; this->a33 /= s;

   return *this;
}

//-----------------------------------------------------------------------------------------------

Matrix3x3 operator +(const Matrix3x3 &a, const Matrix3x3 &b)
{
   Matrix3x3 r;

   r.a11 = a.a11 + b.a11;  r.a12 = a.a12 + b.a12;  r.a13 = a.a13 + b.a13;
   r.a21 = a.a21 + b.a21;  r.a22 = a.a22 + b.a22;  r.a23 = a.a23 + b.a23;
   r.a31 = a.a31 + b.a31;  r.a32 = a.a32 + b.a32;  r.a33 = a.a33 + b.a33;

   return r;
}

Matrix3x3 operator +(const Matrix3x3 &a, float s)
{
   Matrix3x3 r;

   r.a11 = a.a11 + s;  r.a12 = a.a12 + s;  r.a13 = a.a13 + s;
   r.a21 = a.a21 + s;  r.a22 = a.a22 + s;  r.a23 = a.a23 + s;
   r.a31 = a.a31 + s;  r.a32 = a.a32 + s;  r.a33 = a.a33 + s;

   return r;
}

Matrix3x3 operator +(float s, const Matrix3x3 &a)
{
   Matrix3x3 r;

   r.a11 = a.a11 + s;  r.a12 = a.a12 + s;  r.a13 = a.a13 + s;
   r.a21 = a.a21 + s;  r.a22 = a.a22 + s;  r.a23 = a.a23 + s;
   r.a31 = a.a31 + s;  r.a32 = a.a32 + s;  r.a33 = a.a33 + s;

   return r;
}

Matrix3x3 operator -(const Matrix3x3 &a, const Matrix3x3 &b)
{
   Matrix3x3 r;

   r.a11 = a.a11 - b.a11;  r.a12 = a.a12 - b.a12;  r.a13 = a.a13 - b.a13;
   r.a21 = a.a21 - b.a21;  r.a22 = a.a22 - b.a22;  r.a23 = a.a23 - b.a23;
   r.a31 = a.a31 - b.a31;  r.a32 = a.a32 - b.a32;  r.a33 = a.a33 - b.a33;

   return r;
}

Matrix3x3 operator -(const Matrix3x3 &a, float s)
{
   Matrix3x3 r;

   r.a11 = a.a11 - s;  r.a12 = a.a12 - s;  r.a13 = a.a13 - s;
   r.a21 = a.a21 - s;  r.a22 = a.a22 - s;  r.a23 = a.a23 - s;
   r.a31 = a.a31 - s;  r.a32 = a.a32 - s;  r.a33 = a.a33 - s;

   return r;
}


Matrix3x3 operator -(const Matrix3x3 &a)
{
    Matrix3x3 r;
	
   r.a11 = -a.a11;  r.a12 = -a.a12;  r.a13 = -a.a13;
   r.a21 = -a.a21;  r.a22 = -a.a22;  r.a23 = -a.a23;
   r.a31 = -a.a31;  r.a32 = -a.a32;  r.a33 = -a.a33;
	
	return r;
}

Matrix3x3 operator *(const Matrix3x3 &a, float s)
{
   Matrix3x3 r;

   r.a11 = a.a11 * s;  r.a12 = a.a12 * s;  r.a13 = a.a13 * s;
   r.a21 = a.a21 * s;  r.a22 = a.a22 * s;  r.a23 = a.a23 * s;
   r.a31 = a.a31 * s;  r.a32 = a.a32 * s;  r.a33 = a.a33 * s;

   return r;
}

Matrix3x3 operator *(const Matrix3x3 &a, const Matrix3x3 &b)
{
   Matrix3x3 c;

   c.a11 = a.a11*b.a11 + a.a12*b.a21 + a.a13*b.a31;
   c.a21 = a.a21*b.a11 + a.a22*b.a21 + a.a23*b.a31;
   c.a31 = a.a31*b.a11 + a.a32*b.a21 + a.a33*b.a31;

   c.a12 = a.a11*b.a12 + a.a12*b.a22 + a.a13*b.a32;
   c.a22 = a.a21*b.a12 + a.a22*b.a22 + a.a23*b.a32;
   c.a32 = a.a31*b.a12 + a.a32*b.a22 + a.a33*b.a32;

   c.a13 = a.a11*b.a13 + a.a12*b.a23 + a.a13*b.a33;
   c.a23 = a.a21*b.a13 + a.a22*b.a23 + a.a23*b.a33;
   c.a33 = a.a31*b.a13 + a.a32*b.a23 + a.a33*b.a33;

   return c;
}

Matrix3x3 operator /(const Matrix3x3 &a, float s)
{
   Matrix3x3 r;

   r.a11 = a.a11 / s;  r.a12 = a.a12 / s;  r.a13 = a.a13 / s;
   r.a21 = a.a21 / s;  r.a22 = a.a22 / s;  r.a23 = a.a23 / s;
   r.a31 = a.a31 / s;  r.a32 = a.a32 / s;  r.a33 = a.a33 / s;

   return r;
}

//-----------------------------------------------------------------------------------------------

bool operator == (const Matrix3x3 &a, const Matrix3x3 &b)
{
    return(a.a11 == b.a11 && a.a12 == b.a12 && a.a13 == b.a13 &&
           a.a21 == b.a21 && a.a22 == b.a22 && a.a23 == b.a23 &&
           a.a31 == b.a31 && a.a32 == b.a32 && a.a33 == b.a33);
}

bool operator != (const Matrix3x3 &a, const Matrix3x3 &b)
{
    return(a.a11 != b.a11 || a.a12 != b.a12 || a.a13 != b.a13 ||
           a.a21 != b.a21 || a.a22 != b.a22 || a.a23 != b.a23 ||
           a.a31 != b.a31 || a.a32 != b.a32 || a.a33 != b.a33);
}

//-----------------------------------------------------------------------------------------------

Matrix3x3 operator *(float s, const Matrix3x3 &m)
{
    return (m * s);
}

Vector3 operator *(const Matrix3x3 &m, const Vector3 &v)
{
    Vector3 r;

    r.x = m.a11 * v.x + m.a12 * v.y + m.a13 * v.z;
    r.y = m.a21 * v.x + m.a22 * v.y + m.a23 * v.z;
    r.z = m.a31 * v.x + m.a32 * v.y + m.a33 * v.z;

    return r;
}

//!!!!!!!!!!!!!!!!!!!!!!! richtig?
Vector3 operator *(const Vector3 &v, const Matrix3x3 &m)
{
    Vector3 r;

    r.x = m.a11 * v.x + m.a21 * v.y + m.a31 * v.z;
    r.y = m.a12 * v.x + m.a22 * v.y + m.a32 * v.z;
    r.z = m.a13 * v.x + m.a23 * v.y + m.a33 * v.z;

    return r;
}

Matrix3x3 transpose(const Matrix3x3 &m)
{
    Matrix3x3 mat(m);
    return mat.transpose();
}

Matrix3x3 inverse(const Matrix3x3 &m)
{
    Matrix3x3 mat(m);
    return mat.inverse();
}