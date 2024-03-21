//Whored some of this from http://www.robthebloke.org/cppdir/RandomStuff.htm

#ifndef __CVEC_H
#define __CVEC_H

extern "C"
{
#include "q_shared.h" //needed for vec3_t definition

	void QDECL G_Printf( const char *fmt, ... );
}

class CVec3 
{
public:

	// default constructor
	CVec3() : x(0),y(0),z(0) {}

	// constructor for floats
	CVec3( const float x_, 
		   const float y_,
		   const float z_ ) 
				: x(x_), y(y_), z(z_){} 
	CVec3( const vec3_t a );

    // ---- Operators ----

	float& operator[](int i); //subscript
	float operator[](int i) const;  //const subscript
	CVec3& operator = (const CVec3& b); //assignment
	friend CVec3 operator + (const CVec3& a, const CVec3& b);  //addition
	friend CVec3 operator - (const CVec3& a, const CVec3& b);  //subtraction by vector object
	CVec3& operator *= (CVec3 b); //multiplication by vector object
	CVec3& operator *= (float b); //multiplication by scalar
	CVec3& operator += (CVec3 b); //+= by vector
	CVec3& operator -= (CVec3 b); //-= by vector
	bool operator != (vec3_t b);
	CVec3& operator /= (vec3_t b);
	CVec3& operator /= (float b);
	bool operator < (CVec3 b);
	bool operator > (CVec3 b);
	friend CVec3 operator* ( CVec3 a, float b );

	// ---- Data ----

	union 
	{
		float data[3];

		struct 
		{ 
			float x;
			float y;
			float z;
		}; 

		vec3_t v;
	};

	// ---- Function ----

	//gives the distance between this point and the inputted point.
	float Dist( CVec3 b ) const;

	//gives the squared distance between this point and the inputted point.
	float Dist2( CVec3 b ) const;

	//gives the distance between this point and the line generated by the two inputted points.
	float DistToLine( CVec3 a, CVec3 b ) const;

	//gives the distance between this point and the line generated by the two inputted points.
	float DistToLine2( CVec3 a, CVec3 b ) const;

	//converts the vector data into a character string
	void ToStr(char * dest);

	//"safe" normalization of the vector. 
	//normalizes vector and returns original vector length.  
	//RAFIXME - I assume the safe normalization is to prevent zero vectors from breaking stuff.  Need it?
	float SafeNorm();

	//normalize the vector
	float Norm();

	//I assume this means returning the closest point to this vector from the line formed by points a and b.
	//returns the dot product between the line vector and the point in relation to the line.  
	//IE any result between 0-1 means the point projected into the line inbetween the line points.
	float ProjectToLine( const CVec3 a, const CVec3 b );

	//project this vector onto the line segment formed by these points.
	//returns the dot product between the line vector and the point in relation to the line.  
	//IE any result between 0-1 means the point projected into the line inbetween the line points.
	float ProjectToLineSeg( const CVec3 a, const CVec3 b );

	//do the dot product for these two points.
	float Dot(const CVec3 a);

	//returns the length of this vector
	float Len();

	//returns the squared length of this vector
	float Len2();

	//clears the vector
	void Clear();

	//checks to see if this vector is zero?
	bool IsFinite();

	//truncate vector values to this value?
	float Truncate( float trunValue );

	//converts the vector data to angle data.
	void VecToAng();

	//converts the angle data back to vector data
	void AngToVec();

	//I think this is a VectorMA thingy
	void Reposition(CVec3 dir, float scalar);

	//I think this is a VectorMA thingy
	void ScaleAdd(CVec3 dir, float scalar);

	//returns a reference to the pitch value of the vector
	float& pitch();

	//cross product.
	void Cross(float a, float b, float c);


};

#endif
