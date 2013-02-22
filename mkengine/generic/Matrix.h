// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_MATRIX
#define MK_MATRIX

namespace mk 
{
	//	[  a   c   tx  ]		Out mtx
	//	[  b   d   ty  ]
	//	[  0   0   1   ]
	class Matrix
	{
		public:
			Matrix();
			Matrix(float a, float b, float c, float d, float tx, float ty);

			void Set(float a, float b, float c, float d, float tx, float ty);
			void Transform(float &x, float &y);
			void Concat(const Matrix &m);
			Matrix Invert();
			float Determinant();

			void SetScaleRotation ( float x_scale, float y_scale, float angle );
			void SetXScale ( float xscale );
			void SetYScale ( float yscale );
			void SetScale ( float xscale, float yscale );
			void SetRotation ( float rotation );

			float GetAngle();
			float GetScaleX();
			float GetScaleY();

			static Matrix InterpolateMtx(Matrix &m1, Matrix &m2, float t, float max);

		public:
			float a, b, c, d, tx, ty;
	};
}

#endif