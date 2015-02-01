//Author: Sören Pirk
//Date: 22.01.2013

#include "Vector4.h"
#include "Vector3.h"

Vector4::Vector4() 
: x(0.0),
  y(0.0),
  z(0.0),
  w(0.0)
{
}

Vector4::Vector4(float x, float y, float z, float w)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->w = w;
}

Vector4::Vector4(const Vector4 &vec)
{
   this->x = vec.x;
   this->y = vec.y;
   this->z = vec.z;
   this->w = vec.w;
}

Vector4::Vector4(const Vector3 &vec)
{
   this->x = vec.x;
   this->y = vec.y;
   this->z = vec.z;
   this->w = 1;
}

Vector4::Vector4(const float *vec)
{
   this->x = vec[0];
   this->y = vec[1];
   this->z = vec[2];
   this->w = vec[3];
}

Vector4::Vector4(float x)
{
   this->x = x;
   this->y = x;
   this->z = x;
   this->w = x;
}

Vector4::~Vector4() 
{
}

float Vector4::length()
{
   float length = 0.0f;

   length = sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));

   return length;
}

float Vector4::dot(const Vector4 &vec)
{
   float dot = 0.0f;

   dot = (this->x * vec.x) + (this->y * vec.y) + (this->z * vec.z) + (this->w * vec.w);

   return dot;
}

float Vector4::angle(Vector4 &vec)
{
    float d = this->dot(vec);

    float a = this->length();    
    float b = vec.length();

    float s = d / (a*b);
    float angle = (float)acos((double)s);

    return angle;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Vector4 Vector4::cross(const Vector4 &vec)
{
   Vector4 cross;

   cross.x = (this->y * vec.z) - (this->z * vec.y);
   cross.y = (this->z * vec.x) - (this->x * vec.z);
   cross.z = (this->x * vec.y) - (this->y * vec.x);
   cross.w = 1.0f;

   return cross;
}

void Vector4::normalize()
{
   float len = this->length();

   if(len > 0.0f)
   {
       this->x = this->x / len;
       this->y = this->y / len;
       this->z = this->z / len;   
       this->w = this->w / len;
   }
}

Vector4 Vector4::normalized()
{
   float len = this->length();
   Vector4 normalized; 

   if(len > 0.0)
   {
       normalized.x = this->x / len;
       normalized.y = this->y / len;
       normalized.z = this->z / len;
       normalized.w = this->w / len;
   }

   return normalized;
}

void Vector4::set(float x, float y, float z, float w)
{
   this->x = x;
   this->y = y;
   this->z = z;
   this->w = w;
}

void Vector4::set(float *vec)
{
   this->x = vec[0];
   this->y = vec[1];
   this->z = vec[2];
   this->w = vec[3];
}

Vector4 &Vector4::get()
{
    return *this;
}

void Vector4::print()
{
    printf("x: %f, y: %f, z: %f, w: %f\n", x, y, z, w);
}

Vector4 &Vector4::operator =(const Vector4 &a)
{
   this->x = a.x;
   this->y = a.y;
   this->z = a.z;
   this->w = a.w;

   return *this;
}

Vector4 &Vector4::operator +=(const Vector4 &a)
{
   this->x += a.x;
   this->y += a.y;
   this->z += a.z;
   this->w += a.w;

   return *this;
}

Vector4 &Vector4::operator +=(float s)
{
   this->x += s;
   this->y += s;
   this->z += s;
   this->w += s;

   return *this;
}

Vector4 &Vector4::operator -=(const Vector4 &a)
{
   this->x -= a.x;
   this->y -= a.y;
   this->z -= a.z;
   this->w -= a.w;

   return *this;
}

Vector4 &Vector4::operator -=(float s)
{
   this->x -= s;
   this->y -= s;
   this->z -= s;
   this->w -= s;

   return *this;
}

Vector4 &Vector4::operator *=(const Vector4 &a)
{
   this->x *= a.x;
   this->y *= a.y;
   this->z *= a.z;
   this->w *= a.w;

   return *this;
}

Vector4 &Vector4::operator *=(float s)
{
   this->x *= s;
   this->y *= s;
   this->z *= s;
   this->w *= s;

   return *this;
}

Vector4 &Vector4::operator /=(const Vector4 &a)
{
   this->x /= a.x;
   this->y /= a.y;
   this->z /= a.z;
   this->w /= a.w;
   
   return *this;
}

Vector4 &Vector4::operator /=(float s)
{
   this->x /= s;
   this->y /= s;
   this->z /= s;
   this->w /= s;

   return *this;
}

//-----------------------------------------------------------------------------------------------

Vector4 operator +(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x + b.x;
   r.y = a.y + b.y;
   r.z = a.z + b.z;
   r.w = a.w + b.w;

   return r;
}

Vector4 operator +(const Vector4 &a, float s)
{
   Vector4 r;

   r.x = a.x + s;
   r.y = a.y + s;
   r.z = a.z + s;
   r.w = a.w + s;

   return r;
}

Vector4 operator +(float s, const Vector4 &a)
{
   Vector4 r;

   r.x = a.x + s;
   r.y = a.y + s;
   r.z = a.z + s;
   r.w = a.w + s;

   return r;
}

Vector4 operator -(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x - b.x;
   r.y = a.y - b.y;
   r.z = a.z - b.z;
   r.w = a.w - b.w;

   return r;
}

Vector4 operator -(const Vector4 &a, float s)
{
   Vector4 r;

   r.x = a.x - s;
   r.y = a.y - s;
   r.z = a.z - s;
   r.w = a.w - s;

   return r;
}


Vector4 operator -(const Vector4 &a)
{
    Vector4 r;
	
	r.x = -a.x;
	r.y = -a.y;
	r.z = -a.z;
    r.w = -a.w;
	
	return r;
}

Vector4 operator *(const Vector4 &a, float s)
{
   Vector4 r;

   r.x = a.x * s;
   r.y = a.y * s;
   r.z = a.z * s;
   r.w = a.w * s;

   return r;
}

Vector4 operator *(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x * b.x;
   r.y = a.y * b.y;
   r.z = a.z * b.z;
   r.w = a.w * b.w;

   return r;
}

Vector4 operator /(const Vector4 &a, float s)
{
   Vector4 r;

   r.x = a.x / s;
   r.y = a.y / s;
   r.z = a.z / s;
   r.w = a.w / s;

   return r;
}

Vector4 operator /(const Vector4 &a, const Vector4 &b)
{
   Vector4 r;

   r.x = a.x / b.x;
   r.y = a.y / b.y;
   r.z = a.z / b.z;
   r.w = a.w / b.w;

   return r;
}

//-----------------------------------------------------------------------------------------------

bool operator == (const Vector4 &a, const Vector4 &b)
{
    return(a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}

bool operator != (const Vector4 &a, const Vector4 &b)
{
    return(a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
}

bool operator <= (const Vector4 &a, const Vector4 &b)
{
    return(a.x <= b.x && a.y <= b.y && a.z <= b.z && a.w <= b.w);
}

bool operator < (const Vector4 &a, const Vector4 &b)
{
    return(a.x < b.x && a.y < b.y && a.z < b.z && a.w < b.w);
}

bool operator >= (const Vector4 &a, const Vector4 &b)
{
    return(a.x >= b.x && a.y >= b.y && a.z >= b.z && a.w >= b.w);
}

bool operator > (const Vector4 &a, const Vector4 &b)
{
    return(a.x > b.x && a.y > b.y && a.z > b.z && a.w > b.w);
}

//-----------------------------------------------------------------------------------------------

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Vector4 cross(const Vector4 &a, const Vector4 &b)
{
   Vector4 c;

   c.x = (a.y * b.z) - (a.z * b.y);
   c.y = (a.z * b.x) - (a.x * b.z);
   c.z = (a.x * b.y) - (a.y * b.x);
   c.w = 1.0f;

   return c;
}

Vector4 normalize(Vector4 &v)
{
   float len = v.length();
   Vector4 normalized; 

   if(len > 0.0)
   {
       normalized.x = v.x / len;
       normalized.y = v.y / len;
       normalized.z = v.z / len;  
       normalized.w = v.w / len;
   }

   return normalized;
}

float dot(const Vector4 &a, const Vector4 &b)
{
   float dot = 0.0f;

   dot = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);

   return dot;
}

float length(const Vector4 &v)
{
   float length = 0.0f;

   length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));

   return length;
}

Vector4 operator *(float s, const Vector4 &v)
{
    return (v * s);
}