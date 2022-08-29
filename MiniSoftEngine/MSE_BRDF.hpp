#pragma once
#include <cmath>
#include "MSE_Math.h";
#include "MSE_Object.h"
#include "MSE_FrameBuffer.h"
#include "MSE_Texture.h"

using namespace MSE;

namespace SHADER
{
	namespace BRDF
	{
		static Vec4 BRDF_F_Schlick(float VoH, Vec4 f0, float f90)
		{
			return f0 + (Vec4(f90, f90, f90) - f0) * std::pow(1.0 - VoH, 5.0);
		}

		static float BRDF_F_Schlick(float VoH, float f0, float f90)
		{
			return f0 + (f90 - f0) * std::pow(1.0 - VoH, 5.0);
		}


		static float BRDF_Diffuse_Lambert(const Vec4& N, const Vec4& L)
		{
			return std::max(Dot_V3(N, L), 0.0f);
		}

		static float BRDF_Specular_Phong(const Vec4& V, const Vec4& R, int shininess)
		{
			return std::pow(std::max(Dot_V3(V, R), 0.0f), shininess);
		}

		static float BRDF_Specular_BlinPhong(const Vec4& N, const Vec4& H, int shininess)
		{
			return std::pow(std::max(Dot_V3(N, H), 0.0f), shininess);
		}
	}

	namespace SHADOW
	{
		static float SHADOW_HARD_ShadowMap(const Light& light, const Vec4& pos, const Vec4& N)
		{
			//Vec4 fragPosLightSpace = pos;
			Vec4 fragPosLightSpace = Transform(pos, light.camera->viewMat);
			fragPosLightSpace = Transform(fragPosLightSpace, light.camera->orthoMat);
			// Í¸ÊÓ³ý·¨
			fragPosLightSpace.HomogenousDivide();
			// ±ä»»·¶Î§
			fragPosLightSpace = fragPosLightSpace * 0.5f + 0.5f;
		

		///	float closestDepth = (*light.fbo->depthAttachments[0]->contents)[0][0].x;
			float closestDepth = Sample(light.fbo->GetDepthAttach(), fragPosLightSpace.x, fragPosLightSpace.y).x;
			float currentDepth = fragPosLightSpace.z;
			Vec4 L = light.dir;
			float bias = std::max(0.01 * (1.0 - Dot_V3(N, L * -1)), 0.001);
			float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
			//return 0.0;
			return shadow;
		}

		static float SHADOW_SOFT_PCF(const Light& light, const Vec4& pos, const Vec4& N)
		{
			//Vec4 fragPosLightSpace = pos;
			Vec4 fragPosLightSpace = Transform(pos, light.camera->viewMat);
			fragPosLightSpace = Transform(fragPosLightSpace, light.camera->orthoMat);
			// Í¸ÊÓ³ý·¨
			fragPosLightSpace.HomogenousDivide();
			// ±ä»»·¶Î§
			fragPosLightSpace = fragPosLightSpace * 0.5f + 0.5f;


			///	float closestDepth = (*light.fbo->depthAttachments[0]->contents)[0][0].x;
			float currentDepth = fragPosLightSpace.z;
			Vec4 L = light.dir;
			float bias = std::max(0.05 * (1.0 - Dot_V3(N, L * -1)), 0.005);
			float shadow = 0.0;
			float xSize = 1.0f / (float)light.fbo->width, ySize = 1.0f / (float)light.fbo->height ;
			for(float x = -1.0f; x <= 1.0f; x += 1.0f)
				for (float y = -1.0; y <= 1.0f; y += 1.0f)
				{
					float closestDepth = Sample(light.fbo->GetDepthAttach(), fragPosLightSpace.x + x * xSize, fragPosLightSpace.y + y * ySize).x;
					shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
				}

			//return 0.0;
			return shadow / 9.0f;
		}
	}

	namespace GI
	{
		static Vec4 GI_GetIndirectLight_Unifom()
		{
			return Vec4(0.04, 0.04, 0.04, 1.0);
		}
	}

	namespace POSTPROCESS
	{
		static Vec4 PP_ToneMap_Reinhard(Vec4& a)
		{
			return a / (a + Vec4(1.0, 1.0, 1.0, 1.0));
		}

		static Vec4 PP_ToneMap_Exposure(Vec4& a, float exposure)
		{
			return Vec4(1.0, 1.0, 1.0, 1.0) - Exp_V4(a * -1 * exposure);
		}

		static Vec4 PP_GammaCorrection(Vec4& a, float base)
		{
			base = 1.0 / base;
			return Pow_V4(a, base);
		}
	}
}