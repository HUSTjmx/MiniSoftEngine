#pragma once
#include <cmath>
#include "MSE_Math.h";

namespace MSE
{
	namespace SHADER
	{
		Vec4 BRDF_F_Schlick(float VoH, Vec4 f0, float f90)
		{
			return f0 + (Vec4(f90, f90, f90) - f0) * std::pow(1.0 - VoH, 5.0);
		}

		float BRDF_F_Schlick(float VoH, float f0, float f90)
		{
			return f0 + (f90 - f0) * std::pow(1.0 - VoH, 5.0);
		}


		float BRDF_Diffuse_Lambert(const Vec4& N, const Vec4& L)
		{
			return std::max(Dot_V3(N, L), 0.0f);
		}

		float BRDF_Specular_Phong(const Vec4& V, const Vec4& R, int shininess)
		{
			return std::pow(std::max(Dot_V3(V, R), 0.0f), shininess);
		}

		float BRDF_Specular_BlinPhong(const Vec4& N, const Vec4& H, int shininess)
		{
			return std::pow(std::max(Dot_V3(N, H), 0.0f), shininess);
		}
	}

	namespace GI
	{
		Vec4 GI_GetIndirectLight_Unifom()
		{
			return Vec4(0.04, 0.04, 0.04, 1.0);
		}
	}

	namespace POSTPROCESS
	{
		Vec4 PP_ToneMap_Reinhard(Vec4& a)
		{
			return a / (a + Vec4(1.0, 1.0, 1.0, 1.0));
		}

		Vec4 PP_ToneMap_Exposure(Vec4& a, float exposure)
		{
			return Vec4(1.0, 1.0, 1.0, 1.0) - Exp_V4(a * -1 * exposure);
		}

		Vec4 PP_GammaCorrection(Vec4& a, float base)
		{
			base = 1.0 / base;
			return Pow_V4(a, base);
		}
	}
}