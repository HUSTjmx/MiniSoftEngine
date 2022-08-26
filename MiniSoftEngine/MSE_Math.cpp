#include "MSE_Math.h"
#include <cmath>
#include <iostream>

namespace MSE
{
	Vec4& Vec4::operator=(const Vec4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	Vec4 Vec4::operator+(const Vec4& other)
	{
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vec4 Vec4::operator+(float a)
	{
		return Vec4(x + a, y + a, z + a, w);
	}

	Vec4 Vec4::operator-(const Vec4& other)
	{
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vec4 Vec4::operator-(float a)
	{
		return Vec4(x - a, y - a, z - a, w);
	}

	Vec4 Vec4::operator*(const Vec4& other)
	{
		return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	Vec4 Vec4::operator*(float a)
	{
		return Vec4(x * a, y * a, z * a, w);
	}

	Vec4 Vec4::operator/(const Vec4& other)
	{
		//return *this;
		if (other.x <= 0.0001 || other.y <= 0.0001 || other.z <= 0.0001)
		{
			std::cout << "Vec4 Divide Error" << std::endl;
			return *this;
		}
		return Vec4(x / other.x, y / other.y, z / other.z, w * other.w);
	}

	Vec4 Vec4::operator/(float a)
	{
		if (a < 0.0001)return *this;
		float b = 1 / a;
		return Vec4(x * b, y * b, z * b, w);
	}

	Vec4& Vec4::HomogenousDivide()
	{
		// 向量没有齐次除法
		if (w > 0.0001)
		{
			float w_inv = 1.0 / w;
			x *= w_inv;
			y *= w_inv;
			z *= w_inv;
		}
	
		return *this;
	}

	Vec4& Vec4::Normalize_V3()
	{
		float L2 = x * x + y * y + z * z;
		float L = 1.0 / sqrt(L2);
		x *= L;
		y *= L;
		z *= L;
		return *this;
	}

	float Vec4::Dot_V3(const Vec4& other) const
	{
		
		return x * other.x + y * other.y + z * other.z;
	}

	Vec4 Vec4::Cross_V3(const Vec4& other) const
	{
		// u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x
		// 向量才可以叉乘
		return Vec4(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, 0.0);
	}

	float Dot_V3(const Vec4& lhs, const Vec4& rhs)
	{
		return lhs.Dot_V3(rhs);
	}

	Vec4 Cross_V3(const Vec4& lhs, const Vec4& rhs)
	{
		return lhs.Cross_V3(rhs);
	}

	Matrix4x4& Matrix4x4::Transpose()
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elems_T[j][i] = elems[i][j];
		return *this;
	}

	void Matrix4x4::ExChange()
	{
		float temp[4][4];
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				temp[i][j] = elems[i][j];

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elems[i][j] = elems_T[i][j];

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elems_T[i][j] = temp[i][j];

	}

	float Matrix4x4::GetA(float arcs[4][4], int n)
	{
		if (n == 1)
		{
			return arcs[0][0];
		}
		float ans = 0;
		float temp[4][4] = { 0.0 };
		int i, j, k;
		for (i = 0;i < n;i++)
		{
			for (j = 0;j < n - 1;j++)
			{
				for (k = 0;k < n - 1;k++)
				{
					temp[j][k] = arcs[j + 1][(k >= i) ? k + 1 : k];

				}
			}
			float t = GetA(temp, n - 1);
			if (i % 2 == 0)
			{
				ans += arcs[0][i] * t;
			}
			else
			{
				ans -= arcs[0][i] * t;
			}
		}
		return ans;
	}

	void Matrix4x4::GetAStart(float arcs[4][4], int n, float ans[4][4])
	{
		if (n == 1)
		{
			ans[0][0] = 1;
			return;
		}
		int i, j, k, t;
		float temp[4][4];
		for (i = 0;i < n;i++)
		{
			for (j = 0;j < n;j++)
			{
				for (k = 0;k < n - 1;k++)
				{
					for (t = 0;t < n - 1;t++)
					{
						temp[k][t] = arcs[k >= i ? k + 1 : k][t >= j ? t + 1 : t];
					}
				}


				ans[j][i] = GetA(temp, n - 1);
				if ((i + j) % 2 == 1)
				{
					ans[j][i] = -ans[j][i];
				}
			}
		}
	}

	Matrix4x4& Matrix4x4::Inverse()
	{
		float flag = GetA(elems, 4);
		float t[4][4];
		if (flag == 0)
		{
			return *this;
		}
		else
		{
			GetAStart(elems, 4, t);
			for (int i = 0;i < 4; i++)
			{
				for (int j = 0;j < 4; j++)
				{
					elems_Inv[i][j] = t[i][j] / flag;
				}

			}
		}
		return *this;
	}

	Matrix4x4& Matrix4x4::InvTranspose()
	{
		Matrix4x4 a = GetInvTranspose(*this);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elems_Inv_T[i][j] = a.elems[i][j];
		return *this;
	}

	Matrix4x4 Mul(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 r;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				r.elems[i][j] = a.elems[i][0] * b.elems[0][j]
					+ a.elems[i][1] * b.elems[1][j]
					+ a.elems[i][2] * b.elems[2][j]
					+ a.elems[i][3] * b.elems[3][j];
			}
		}
		return r;
	}

	Vec4 Transform(const Vec4& v, const Matrix4x4& m)
	{
		Vec4 r;
		r.x = v.x * m.elems[0][0] + v.y * m.elems[1][0] + v.z * m.elems[2][0] + v.w * m.elems[3][0];
		r.y = v.x * m.elems[0][1] + v.y * m.elems[1][1] + v.z * m.elems[2][1] + v.w * m.elems[3][1];
		r.z = v.x * m.elems[0][2] + v.y * m.elems[1][2] + v.z * m.elems[2][2] + v.w * m.elems[3][2];
		r.w = v.x * m.elems[0][3] + v.y * m.elems[1][3] + v.z * m.elems[2][3] + v.w * m.elems[3][3];
		return r;
	}

	Matrix4x4 GetTansposeInv(Matrix4x4 m)
	{
		m.Transpose();
		m.ExChange();
		m.Inverse();
		return Matrix4x4(m.elems_Inv);
	}

	Matrix4x4 GetInvTranspose(Matrix4x4 m)
	{
		m.Inverse();
		Matrix4x4 temp(m.elems_Inv);
		temp.Transpose();
		temp.ExChange();
		return temp;
	}

	float Radians(float degree)
	{
		return degree * 0.01745329251994329576;
	}

	int VecCol2IntCol(const Vec4& col)
	{
		int re = ((int)(col.x) << 16) | ((int)(col.y) << 8) | (int)(col.z);
		return re;

	}

	int FloatCol2IntCol(float col)
	{
		int re = ((int)col << 16) | ((int)col << 8) | ((int)col);
		return re;
	}

	Vec4 BaryCentric(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& p)
	{
		Vec4 v1(c.x - a.x, b.x - a.x, a.x - p.x);
		Vec4 v2(c.y - a.y, b.y - a.y, a.y - p.y);

		Vec4 u = Cross_V3(v1, v2);
		if (std::abs(u.z) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
			return { 1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z };
		return { -1.0, 1.0, 1.0, 0.0 };
	}

	Vec4 BaryLerp(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& ret)
	{
		Vec4 re;
		re.x = a.x * ret.x + b.x * ret.y + c.x * ret.z;
		re.y = a.y * ret.x + b.y * ret.y + c.y * ret.z;
		re.z = a.z * ret.x + b.z * ret.y + c.z * ret.z;
		re.w = a.w;
		return re;
	}

	Vec4 Reflect(Vec4& L, Vec4& N)
	{
		float a = Dot_V3(L, N);

		return N * 2.0 * a - L;
	}

	float Clamp(float value, float L, float R)
	{
		if (value < L)return L;
		if (value > R)return R;
		return value;
	}

	Vec4 OneMinus(const Vec4& other)
	{
		return Vec4(1.0 - other.x, 1.0 - other.y, 1.0 - other.z, 1.0 - other.w);
	}

	Vec4 Exp_V4(const Vec4& other)
	{
		return Vec4(std::exp(other.x), std::exp(other.y), std::exp(other.z), 1.0);
	}

	Vec4 Pow_V4(const Vec4& other, float base)
	{
		return Vec4(pow(other.x, base), pow(other.y, base), pow(other.z, base), 1.0);
	}
}