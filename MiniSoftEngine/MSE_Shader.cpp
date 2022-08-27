#include "MSE_Shader.h"
#include "MSE_Math.h"
#include "MSE_Material.h"
#include "MSE_Camera.h"
#include "MSE_BRDF.hpp"
#include "MSE_Object.h"
#include "MSE_FrameBuffer.h"

namespace MSE
{
	void vertShader::Process(const Vertex& v, Vertex& v_R, const Matrix4x4& model, std::shared_ptr<Camera> camera)
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

			for (int j = 0; j < lights.size(); j++)
			{
				Vec4 L = lights[j].dir;
				L = L.Normalize_V3() * -1;

				Vec4 H = (L + V) / 2.0; H.w = 0.0;
				Vec4 R = Reflect(L, N);
				float VoH = Dot_V3(V, H);

				float fd = SHADER::BRDF_Diffuse_Lambert(N, L);
				float fs = SHADER::BRDF_Specular_BlinPhong(N, H, 4);

				Vec4 F0 = material->GetF0();
				float F90 = material->GetF90().x;
				Vec4 ks = SHADER::BRDF_F_Schlick(VoH, F0, F90);
				Vec4 kd = OneMinus(ks);

				Vec4 diffuseColor = material->GetDiffuseColor();

				//
				Vec4 indirectLight = GI::GI_GetIndirectLight_Unifom();
				Vec4 directLight = (diffuseColor * kd * fd + ks * fs) * lights[j].color;

				col = col + (indirectLight + directLight);

				//col = Vec4(fs, fs, fs);
			}
			fbo.DrawColorAttach(frag.fboX, frag.fboY, col);
		}
	}
	void fragShader_DepthDraw::Process(std::vector<Fragment>& frags, FrameBuffer& fbo, const std::vector<Light>& lights, std::shared_ptr<Camera> camera, std::shared_ptr<Material> material)
	{
	}
}