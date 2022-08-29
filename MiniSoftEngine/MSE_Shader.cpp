#include "MSE_Shader.h"
#include "MSE_Math.h"
#include "MSE_Material.h"
#include "MSE_Camera.h"
#include "MSE_BRDF.hpp"
#include "MSE_Object.h"
#include "MSE_FrameBuffer.h"

using namespace SHADER;

namespace MSE
{

	void vertShader::Process(const Vertex& v, Vertex& v_R, const Matrix4x4& model, std::shared_ptr<Camera> camera, const std::vector<Light>& lights)
	{
		Vec4 p = Transform(v.pos, model);
		v_R.posWS = p;

		p = Transform(p, camera->viewMat);
		if (camera->projectionType == CameraType::Perspective)
			p = Transform(p, camera->projMat);
		else
			p = Transform(p, camera->orthoMat);
		Matrix4x4 norM = Matrix4x4(model.elems_Inv_T);
		v_R.pos = p;
		v_R.normal = Transform(v.normal, norM);
		v_R.color = v.color;
	}

	void fragShader::Process(std::vector<Fragment>& frags, FrameBuffer& fbo, const std::vector<Light>& lights, std::shared_ptr<Camera> camera, std::shared_ptr<Material> material)
	{
		for (int i = 0; i < frags.size(); i++)
		{
			Fragment& frag = frags[i];
			Vec4 col = Vec4();

			Vec4 N = frag.normal;
			N.Normalize_V3();
			Vec4 V = camera->Position - frag.posWS;
			V.Normalize_V3();

			int m = 0;
			for (int j = 0; j < lights.size(); j++)
			{
				Vec4 L = lights[j].dir;
				L = L.Normalize_V3() * -1.0f;

				Vec4 H = (L + V) / 2.0f; H.w = 0.0f;
				Vec4 R = Reflect(L, N);
				float VoH = Dot_V3(V, H);

				float fd = BRDF::BRDF_Diffuse_Lambert(N, L);
				float fs = BRDF::BRDF_Specular_BlinPhong(N, H, 4);

				Vec4 F0 = material->GetF0();
				float F90 = material->GetF90().x;
				Vec4 ks = BRDF::BRDF_F_Schlick(VoH, F0, F90);
				Vec4 kd = OneMinus(ks);

				Vec4 diffuseColor = material->GetDiffuseColor();

				float shadow = 0.0;
				if (lights[j].CanCastShadow)
				{
					//shadow = SHADOW::SHADOW_HARD_ShadowMap(lights[j], frag.posWS, N);
					shadow = SHADOW::SHADOW_SOFT_PCF(lights[j], frag.posWS, N);
				}

				//
				Vec4 indirectLight = GI::GI_GetIndirectLight_Unifom() * diffuseColor;
				Vec4 directLight = (diffuseColor * kd * fd + ks * fs) * lights[j].color * (1.0f - shadow);

				col = col + (indirectLight + directLight);

				//col = Vec4(fs, fs, fs);
			}
			fbo.DrawColorAttach(frag.fboX, frag.fboY, col);
		}
	}
	
	void fragShader_DepthDraw::Process(std::vector<Fragment>& frags, FrameBuffer& fbo, const std::vector<Light>& lights, std::shared_ptr<Camera> camera, std::shared_ptr<Material> material)
	{
		
	}

	void vertShaderWithPosInLightSpace::Process(const Vertex& v, Vertex& v_R, const Matrix4x4& model, std::shared_ptr<Camera> camera, const std::vector<Light>& lights)
	{
		Vec4 p = Transform(v.pos, model);
		v_R.posWS = p;
		
		for (int i = 0; i < lights.size(); i++)
		{
			if (lights[i].CanCastShadow)
			{
				Vec4 temp = Transform(v_R.posWS, lights[i].camera->viewMat);
				temp = Transform(temp, lights[i].camera->orthoMat);
				v_R.posesLightPos.push_back(temp);
			}
		}

		p = Transform(p, camera->viewMat);
		if (camera->projectionType == CameraType::Perspective)
			p = Transform(p, camera->projMat);
		else
			p = Transform(p, camera->orthoMat);
		Matrix4x4 norM = Matrix4x4(model.elems_Inv_T);
		v_R.pos = p;
		v_R.normal = Transform(v.normal, norM);
		v_R.color = v.color;
	}
}