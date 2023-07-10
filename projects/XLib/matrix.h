#pragma once
#include "PCH.h"
#include "vector3f.h"

namespace X
{
	// Here's the matrix class used for applying transformations to vectors in 3D space
	// and projecting points to 2D.
	// This is a column-major matrix, compatible with OpenGL.
	// So the elements of the matrix are stored like this..
	// [ 0  4  8 12 ]
	// [ 1  5  9 13 ]
	// [ 2  6 10 14 ]
	// [ 3  7 11 15 ]
	// 
	// A matrix is a rectangular array of numbers layed out in rows and columns, like this...
	// [1 0 0 0]
	// [0 1 0 0]
	// [0 0 1 0]
	// [0 0 0 1]
	// 
	// The above is a special case of a matrix, known as an identity matrix where if multiplying
	// a point/vector by it, will produce no translation, rotation or scale.
	//
	// A translation in a matrix for x, y and z are stored in these values...
	// [1 0 0 x]
	// [0 1 0 y]
	// [0 0 1 z]
	// [0 0 0 1]
	//
	// The scaling values are along the diagonal(scaling of x, y and z)...
	// [x 0 0 0]
	// [0 y 0 0]
	// [0 0 z 0]
	// [0 0 0 1]
	// 
	// The rotation part is more complicated since it uses cosine and sine functions and it
	// changes depending on the axis...
	//		X - Axis Rotation			Y - Axis Rotation			Z - Axis Rotation
	// [1	0			0		0]	[cos(θ)		0	-sin(θ)	0]	[cos(θ)		sin(θ)  0   0]
	// [0	cos(θ)		sin(θ)	0]	[0			1   0		0]	[-sin(θ)	cos(θ)  0   0]
	// [0	-sin(θ)		cos(θ)  0]	[sin(θ)		0   cos(θ)	0]	[0			0       1   0]
	// [0	0			0		1]	[0			0   0		1]	[0			0       0   1]
	//
	// The rotation matrices can be combined with the scale and translation matrix to create the final matrix.
	//
	// Here's what a poor vertex has to go through to get to the screen...
	// Vertex object coordinates x model matrix = world coordinates
	// World coordinates x view matrix = eye coordinates
	// Eye coordinates x projection matrix = clipping space coordinates
	// Clipping space coordinates x perspective division = normalized device coordinates
	// Normalized device coordinates x viewport transformation = screen coordinates
	//
	// Vertex object coordinates are the vertex positions usually created in 3D modelling apps such as Blender.
	// Model matrix is what translates, rotates and scales a vertex into the correct world position.
	// View matrix is the camera position and created by the inverse of the camera position. We use the lookat()
	// method to create a view matrix.
	// Projection matrix is created using setProjectionOrtho() and setProjectionPerspective() depending
	// upon how we wish the vertices to be transformed.
	// The other stuff is performed in the vertex/fragment shaders.
	// 
	// Remember that the order of matrix multiplication is important and we go from right to left, like this in a shader...
	// gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
	// 
	class CMatrix
	{
	public:
		// Default constructor, leaves matrix in undefined state
		CMatrix();

		// Constructor which sets this matrix from the given floats
		CMatrix(
			float m00, float m01, float m02, float m03,
			float m04, float m05, float m06, float m07,
			float m08, float m09, float m10, float m11,
			float m12, float m13, float m14, float m15);

		// Compares whether this matrix and the one to the right of the == operator are equal.
		// This is an exact compare, no teeny epsilon value.
		bool operator==(const CMatrix& other) const;
		
		// Compares whether this matrix and the one to the right of the != operator are equal.
		// This is an exact compare, no teeny epsilon value.
		bool operator!=(const CMatrix& other) const;

		// Sets matrix to identity matrix
		void setIdentity(void);

		// Fills the matrix with zeros
		void setZero(void);

		// Sets this matrix from the given array of floats
		void set(const float src[16]);

		// Sets the matrix from the one given
		void set(const CMatrix& src);

		// Returns a matrix containing the contents of this matrix
		CMatrix get(void);

		// Sets the given vector to hold this matrix's right vector/axis
		// This is used to obtain the orientation of the current state of the matrix
		void getRightVector(CVector3f& vVec);

		// Sets the given vector to hold this matrix's up vector/axis
		// This is used to obtain the orientation of the current state of the matrix
		void getUpVector(CVector3f& vVec);

		// Sets the given vector to hold this matrix's forward vector/axis
		// This is used to obtain the orientation of the current state of the matrix
		void getForwardVector(CVector3f& vVec);

		// Returns a matrix which is the transpose of this matrix
		// The transpose of a matrix is obtained by flipping the matrix over its diagonal.
		// Which means the rows become columns and the columns become rows.
		// The transpose of a matrix has some useful properties such as reversing the order
		// of multiplication, preserving the trace and determinant, and changing the orientation of vectors.
		CMatrix transpose(void);

		// Multiplies this matrix by the one given and returns the resulting matrix
		CMatrix multiply(const CMatrix& other);
	private:
		float m[16];	// Values for the matrix
//		float tm[16];	// Used as a temporary matrix in calculations
	};
}