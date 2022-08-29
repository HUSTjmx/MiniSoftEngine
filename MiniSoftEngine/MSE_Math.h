#pragma once
#include <array>

namespace MSE
{
	class Vec4 {
	public:

		Vec4() : x(0.0),y(0.0), z(0.0),w(0.0) {}

		Vec4(float a) : x(a), y(0.0), z(0.0), w(0.0) {}

		Vec4(float a, float b) : x(a), y(b), z(0.0), w(0.0) {}

		Vec4(float a, float b, float c) : x(a), y(b), z(c), w(0.0) {}

		Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}

		Vec4& operator=(const Vec4& other);

		Vec4 operator+(const Vec4& other);
		Vec4 operator+(float a);

		Vec4 operator-(const Vec4& other);
		Vec4 operator-(float a);

		Vec4 operator*(const Vec4& other);
		Vec4 operator*(float a);

		Vec4 operator/(const Vec4& other);
		Vec4 operator/(float a);

		Vec4& HomogenousDivide();

		Vec4& Normalize_V3();

		//Vec4& Normalize_V4();

		float Dot_V3(const Vec4& other) const;

		Vec4 Cross_V3(const Vec4& other)const;

	public:
	
		float x;
		float y;
		float z;
		float w;

	};

	float Dot_V3(const Vec4& lhs, const Vec4& rhs);

	Vec4 Cross_V3(const Vec4& lhs, const Vec4& rhs);

	class Matrix4x4 {
	public:

		Matrix4x4()
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					elems[i][j] = 0;
			elems[0][0] = 1;
			elems[1][1] = 1;
			elems[2][2] = 1;
			elems[3][3] = 1;
			Transpose();
		}

		Matrix4x4(float a[4][4])
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					elems[i][j] = a[i][j];
			Transpose();
		}

		Matrix4x4(const float a[4][4])
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					elems[i][j] = a[i][j];
			Transpose();
		}

		Matrix4x4(const Matrix4x4& other)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					elems[i][j] = other.elems[i][j];
					elems_T[i][j] = other.elems_T[i][j];
				}
		}

		Matrix4x4& operator=(const Matrix4x4& other)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					elems[i][j] = other.elems[i][j];
					elems_T[i][j] = other.elems_T[i][j];
				}
			return *this;
		}

		Matrix4x4& Transpose();

		Matrix4x4& Inverse();

		Matrix4x4& InvTranspose();

		void ExChange();


	private:

		float GetA(float arcs[4][4], int n);

		void  GetAStart(float arcs[4][4], int n, float ans[4][4]);

	public:
		float elems[4][4];
		float elems_T[4][4];
		float elems_Inv[4][4];
		float elems_Inv_T[4][4];
	};

	Matrix4x4 Mul(const Matrix4x4& a, const Matrix4x4& b);

	Vec4 Transform(const Vec4& v, const Matrix4x4& m);

	Matrix4x4 GetTansposeInv(Matrix4x4 m);

	Matrix4x4 GetInvTranspose(Matrix4x4 m);

	float Clamp(float value, float L, float R);

	float Radians(float degree);

	int VecCol2IntCol(const Vec4& col);

	int FloatCol2IntCol(float col);

	Vec4 BaryCentric(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& p);

	Vec4 BaryLerp(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& ret, float ZA, float ZB, float ZC, float Z);

	Vec4 BaryLerp(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& ret);

	Vec4 Reflect(Vec4& L, Vec4& N);

	Vec4 OneMinus(const Vec4& other);

	Vec4 Exp_V4(const Vec4& other);

	Vec4 Pow_V4(const Vec4& other, float base);
}

