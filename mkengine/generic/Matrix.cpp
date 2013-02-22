// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Matrix.h"
#include "MathTools.h"

namespace mk
{
	Matrix::Matrix()
	{
		// Identity
		a = d = 1.0f;
		b = c = tx = ty = 0.0f;
	}

	Matrix::Matrix(float a, float b, float c, float d, float tx, float ty)
	{
		Set(a, b, c, d, tx, ty);
	}

	void Matrix::Set(float a, float b, float c, float d, float tx, float ty)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->tx = tx;
		this->ty = ty;
	}

	void Matrix::Transform(float &_x, float &_y)
	{	
		float t0 = ( a  * _x ) + ( c * _y ) + tx;
		float t1 = ( b * _x ) + ( d  * _y ) + ty;
		
		_x = t0;
		_y = t1;
	}

	void Matrix::Concat(const Matrix &m)
	{
		float _a;
		float _b;
		float _c;
		float _d;
		float _tx;
		float _ty;
		
		_a	= ( a * m.a  ) + ( c * m.b  );
		_b	= ( b * m.a  ) + ( d * m.b  );
		_c	= ( a * m.c  ) + ( c * m.d  );
		_d	= ( b * m.c  ) + ( d * m.d  );
		//_tx	= ( a * m.tx ) + ( c * m.ty ) + tx;
		//_ty	= ( b * m.tx ) + ( d * m.ty ) + ty;
		_tx = m.tx + tx;
		_ty = m.ty + ty;
		
		Set( _a, _b, _c, _d, _tx, _ty );
	}

	Matrix Matrix::Invert()
	{
		float det = Determinant();
		
		if( det == 0 )
		{
			Set( 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 );	// set identity.
			
			return *this;
		}
		
		const float _d	= 1.0 / det;
		const float t0	= d * _d;
		
		d = (  a * _d );
		c = ( -c * _d );
		b = ( -b * _d );
		
		float t4 = -( ( tx * t0 ) + ( ty * c ) );
		ty = ( -( ( tx * b ) + ( ty * d ) ) );
		
		a = ( t0 );
		tx = ( t4 );
		
		return *this;
	}

	float Matrix::Determinant()
	{
		return a * d - b * c;
	}

	float Matrix::GetAngle()
	{
		float _a = a;
		float _b = b;
		float r = atan2f( _b, _a ); 
		
		return r;
	}

	float Matrix:: GetScaleX()
	{
		return sqrtf( ( a * a + b * b ) ) / 1.0;
	}

	float Matrix::GetScaleY()
	{
		return sqrtf( ( d * d + c * c ) ) / 1.0;
	}

	void Matrix::SetScaleRotation ( float x_scale, float y_scale, float angle )
	{
		const float cos_angle = cosf( angle );
		const float sin_angle = sinf( angle );
		
		a = ( x_scale *  cos_angle );
		b = ( x_scale *  sin_angle );
		c = ( y_scale * -sin_angle );
		d = ( y_scale *  cos_angle );
	}
	
	void Matrix::SetXScale ( float xscale )
	{
		const double rot_x = atan2f( b, a );
		
		a = ( xscale * cos( rot_x ) );
		b = ( xscale * sin( rot_x ) );
	}
	
	void Matrix::SetYScale ( float yscale )
	{
		const float rot_y = atan2f( -c, d );
		
		c = ( yscale * sin( rot_y ) );
		d = ( yscale * cos( rot_y ) );
	}
	
	void Matrix::SetScale ( float xscale, float yscale )
	{
		const float rotation = GetAngle();
		SetScaleRotation( xscale, yscale, rotation ); 
	}
	
	void Matrix::SetRotation ( float rotation )
	{   
		const float rot_x	= atan2f(  b, a );
		const float rot_y	= atan2f( -c, d );
		const float scale_x	= GetScaleX();
		const float scale_y	= GetScaleY();
		
		a = ( scale_x * cos( rotation ) );
		b = ( scale_x * sin( rotation ) );
		c = ( scale_y * sin( rot_y - rot_x + rotation ) * -1 );		// reverse sign.
		d = ( scale_y * cos( rot_y - rot_x + rotation ) );
	}

	Matrix Matrix::InterpolateMtx(Matrix &m1, Matrix &m2, float t, float max)
	{
		Matrix out;

		// Lerping translation/scale
		float scaleX = Lerp(t, 0.0f, max, m1.GetScaleX(), m2.GetScaleX() );
		float scaleY = Lerp(t, 0.0f, max, m1.GetScaleY(), m2.GetScaleY() );
		float transX = Lerp(t, 0.0f, max, m1.tx, m2.tx );
		float transY = Lerp(t, 0.0f, max, m1.ty, m2.ty );

		// Slerping angle
		float angle = Slerp2D(t, 0.0f, max, m1.GetAngle(), m2.GetAngle() );

		// Making Mtx
		out.tx = transX;
		out.ty = transY;
		
		Matrix scale, rot;
		scale.SetScale(scaleX, scaleY);
		rot.SetRotation(angle);

		out.Concat(scale);
		out.Concat(rot);

		return out;
	}
};