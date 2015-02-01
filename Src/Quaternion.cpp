//Author: Sören Pirk
//Date: 22.01.2013

#include "Quaternion.h"

Quaternion::Quaternion() 
: x(0.0f),
  y(0.0f),
  z(0.0f),
  w(1.0f)
{
}

Quaternion::Quaternion(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion::Quaternion(const Vector3 &v, const float s)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = s;
}

Quaternion::Quaternion(const float *f)
{
    this->x = f[0];
    this->y = f[1];
    this->z = f[2];
    this->w = f[3];
}

Quaternion::Quaternion(const Quaternion &q)
{
    this->x = q.x;
    this->y = q.y;
    this->z = q.z;
    this->w = q.w;
}

Quaternion::~Quaternion() 
{
}

float Quaternion::length()
{
    float l = sqrtf(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);
    return l;
}

float Quaternion::length2()
{
    //To-Do!
    return 0;
}

Quaternion Quaternion::normalized()
{
    Quaternion r(*this);
    float l = this->length();
	
    r.x /= l;
	r.y /= l;
	r.z /= l;
	r.w /= l;

	return r;
}

void Quaternion::normalize()
{
    float l = this->length();
	
    this->x /= l;
	this->y /= l;
	this->z /= l;
	this->w /= l;
}

void Quaternion::set(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::set(const Vector3 &v, const float s)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = s;
}

void Quaternion::set(const float *f)
{
    this->x = f[0];
    this->y = f[1];
    this->z = f[2];
    this->w = f[3];
}

void Quaternion::set(const Quaternion &q)
{
    this->x = q.x;
    this->y = q.y;
    this->z = q.z;
    this->w = q.w;
}

Quaternion &Quaternion::setEuler(const float Yaw, const float Pitch, const float Roll)
{
	float cosY = cosf(Yaw / 2.0f);
	float sinY = sinf(Yaw / 2.0f);
	float cosP = cosf(Pitch / 2.0f);
	float sinP = sinf(Pitch / 2.0f);
	float cosR = cosf(Roll / 2.0f);
	float sinR = sinf(Roll / 2.0f);

	set(
		cosR * sinP * cosY + sinR * cosP * sinY,
		cosR * cosP * sinY - sinR * sinP * cosY,
		sinR * cosP * cosY - cosR * sinP * sinY,
		cosR * cosP * cosY + sinR * sinP * sinY
		);

	return *this;	
}

Quaternion Quaternion::slerp(const Quaternion &from, const Quaternion &to, const float t)
{
    Quaternion to1;
    float omega, cosom, sinom, scale0, scale1;

    //calculate cosine
    cosom = (from.x * to.x + from.y * to.y + from.z * to.z) + from.w + to.w;

    //Adjust signs (if necessary) 
    if ( cosom < 0.0 ) 
    {
        cosom = -cosom;
        to1 = -to;
    }
    else
    {
        to1 = to;
    }

    //Calculate coefficients
    if ((1.0 - cosom) > QUATERNION_ERROR ) 
    {
        //standard case (slerp)
        omega =  (float) acos( cosom );
        sinom =  (float) sin( omega );
        scale0 = (float) sin((1.0 - t) * omega) / sinom;
        scale1 = (float) sin(t * omega) / sinom;
    }
    else 
    {
        //'from' and 'to' are very close - just do linear interpolation
        scale0 = 1.0f - t;
        scale1 = t;      
    }

    return scale0 * from + scale1 * to1;
    return Quaternion();
}

void Quaternion::toMatrix(float *m)
{
	if(!m) return;

	// First row
	m[ 0] = 1.0f - 2.0f * (y * y + z * z); 
	m[ 1] = 2.0f * (x * y + z * w);
	m[ 2] = 2.0f * (x * z - y * w);
	m[ 3] = 0.0f;  

	// Second row
	m[ 4] = 2.0f * (x * y - z * w);  
	m[ 5] = 1.0f - 2.0f * (x * x + z * z); 
	m[ 6] = 2.0f * (z * y + x * w);  
	m[ 7] = 0.0f;  

	// Third row
	m[ 8] = 2.0f * (x * z + y * w);
	m[ 9] = 2.0f * (y * z - x * w);
	m[10] = 1.0f - 2.0f * (x * x + y * y);  
	m[11] = 0.0f;  

	// Fourth row
	m[12] = 0;  
	m[13] = 0;  
	m[14] = 0;  
	m[15] = 1.0f;
}

void Quaternion::fromAxisAngle(float x, float y, float z, float degrees)
{
	// First we want to convert the degrees to radians 
	// since the angle is assumed to be in radians
	float angle = float((degrees / 180.0f) * QUATERNION_PI);

	// Here we calculate the sin( theta / 2) once for optimization
	float result = (float)sin( angle / 2.0f );
		
	// Calcualte the w value by cos( theta / 2 )
	this->w = (float)cos( angle / 2.0f );

	// Calculate the x, y and z of the quaternion
	this->x = float(x * result);
	this->y = float(y * result);
	this->z = float(z * result);
}

Quaternion &Quaternion::get()
{
    return *this;
}

void Quaternion::print()
{
    printf("v: %3.2f, %3.2f, %3.2f  s: %3.2f", this->x, this->y, this->z, this->w);
}

Quaternion &Quaternion::operator =(const Quaternion &q)
{
   this->x = q.x;
   this->y = q.y;
   this->z = q.z;
   this->w = q.w;

   return *this;
}

Quaternion &Quaternion::operator +=(const Quaternion &q)
{
   this->x += q.x;
   this->y += q.y;
   this->z += q.z;
   this->w += q.w;

   return *this;
}

Quaternion &Quaternion::operator -=(const Quaternion &q)
{
   this->x -= q.x;
   this->y -= q.y;
   this->z -= q.z;
   this->w -= q.w;

   return *this;
}

Quaternion &Quaternion::operator *=(const Quaternion &q)
{
   w = w*q.w - x*q.x - y*q.y - z*q.z;
   
   x = w*q.x + x*q.w + y*q.z - z*q.y;
   y = w*q.y + y*q.w + z*q.x - x*q.z;
   z = w*q.z + z*q.w + x*q.y - y*q.x;

   return *this;
}

Quaternion &Quaternion::operator *=(float s)
{
   this->x *= s;
   this->x *= s;
   this->z *= s;
   this->w *= s;

   return *this;
}

Quaternion &Quaternion::operator /=(float s)
{
   this->x /= s;
   this->y /= s;
   this->z /= s;
   this->w /= s;

   return *this;
}

Quaternion Quaternion::operator *(const Quaternion &q)
{
	Quaternion r;

	r.w = w * q.w - x * q.x - y * q.y - z * q.z;
	r.x = w * q.x + x * q.w + y * q.z - z * q.y;
	r.y = w * q.y + y * q.w + z * q.x - x * q.z;
	r.z = w * q.z + z * q.w + x * q.y - y * q.x;
	
	return(r);
}


//-----------------------------------------------------------------------------------------

Quaternion operator -(const Quaternion &a)
{
	return Quaternion(-a.x, -a.y, -a.z, -a.w);
}

Quaternion operator +(const Quaternion &a, const Quaternion &b)
{
   return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Quaternion operator -(const Quaternion &a, const Quaternion &b)
{
    return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Quaternion operator *(const Quaternion &a, const float f)
{
    return Quaternion(a.x * f, a.y * f, a.z * f, a.w * f);
}

Quaternion operator *(const float f, const Quaternion &a)
{
    return Quaternion(a.x * f, a.y * f, a.z * f, a.w * f);
}

Quaternion operator *(const Quaternion &a, const Quaternion &b)
{
    Quaternion r;

    r.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    r.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    r.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
    r.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;

    return r;
}

bool operator == (const Quaternion &a, const Quaternion &b)
{
	return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
}

bool operator != (const Quaternion &a, const Quaternion &b)
{
    return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w));
}
