#include "MSE_Pipeline.h"
#include "MSE_BRDF.hpp"
#include "MSE_Shader.h"

using namespace SHADER;

namespace MSE {
	

	void Pipeline::TransformScreen(Vec4& p, int width, int height)
	{
		p.x = p.x * 0.5 + 0.5;
		p.y = p.y * 0.5 + 0.5;
		p.z = p.z * 0.5 + 0.5;

		p.x *= width;
		p.y *= height;
	}

	bool Pipeline::ViewClip(const Vec4& v)
	{
		if (v.z < 0.0f) return true;
		if (v.z > 1.0) return true;
		if (v.x < -1.0) return true;
		if (v.x > 1.0) return true;
		if (v.y < -1.0) return true;
		if (v.y > 1.0) return true;
		return false;

	}

	void Pipeline::Rasterize(Facet& facet, FrameBuffer& fbo, std::vector<Fragment>& re, std::shared_ptr<Camera> camera, Vec4 param1 /* = Vec4() */, Vec4 param2 /* = Vec4() */)
	{
		const Vertex& a = facet.p1_R;
		const Vertex& b = facet.p2_R;
		const Vertex& c = facet.p3_R;

		float xl = a.pos.x; if (b.pos.x < xl) xl = b.pos.x; if (c.pos.x < xl) xl = c.pos.x;
		float xr = a.pos.x; if (b.pos.x > xr) xr = b.pos.x; if (c.pos.x > xr) xr = c.pos.x;
		float yt = a.pos.y; if (b.pos.y < yt) yt = b.pos.y; if (c.pos.y < yt) yt = c.pos.y;
		float yb = a.pos.y; if (b.pos.y > yb) yb = b.pos.y; if (c.pos.y > yb) yb = c.pos.y;

		int xMin = (int)(xl), xMax = (int)(xr + 0.5f), yMin = (int)(yt + 0.5f), yMax = (int)(yb + 0.5f);
		for (int x = xMin; x <= xMax; ++x)
		{
			for (int y = yMin; y <= yMax; ++y)
			{
				// 覆盖测试
				Vec4 ret = BaryCentric(a.pos, b.pos, c.pos, { (float)x, (float)y, 0.0f, 0.0f });
				if (ret.x < 0 || ret.y < 0 || ret.z < 0) continue;

				// 提前深度测试
				float depth = 1.0;
				if(camera->projectionType == CameraType::Perspective)depth = 1.0f / a.pos.w * ret.x + 1.0f / b.pos.w * ret.y + 1.0f / c.pos.w * ret.z;
				else
				{
					depth = a.pos.z * ret.x + b.pos.z * ret.y + c.pos.z * ret.z;
				}
				if (!EarlyZTest(fbo, x, y, depth, camera->projectionType)) continue;

				// 提前模板测试
				if (UseStencil && EarlyStencilTest(fbo, x, y)) continue;

				Fragment temp;
				temp.depth = depth;
				if (camera->projectionType == CameraType::Perspective)
				{
					depth = 1.0 / depth;
					temp.color = BaryLerp(a.color, b.color, c.color, ret, a.pos.w, b.pos.w, c.pos.w, depth);
					temp.posWS = BaryLerp(a.posWS, b.posWS, c.posWS, ret, a.pos.w, b.pos.w, c.pos.w, depth);
					for (int i = 0; i < a.posesLightPos.size(); i++)
					{
						Vec4 xx = BaryLerp(a.posesLightPos[i], b.posesLightPos[i], c.posesLightPos[i], ret, a.pos.w, b.pos.w, c.pos.w, depth);
						temp.posesLightPos.push_back(xx);
					}
					temp.normal = BaryLerp(a.normal, b.normal, c.normal, ret, a.pos.w, b.pos.w, c.pos.w, depth);
				}
				else
				{
					temp.color = BaryLerp(a.color, b.color, c.color, ret);
					temp.posWS = BaryLerp(a.posWS, b.posWS, c.posWS, ret);
					for (int i = 0; i < a.posesLightPos.size(); i++)
					{
						Vec4 xx = BaryLerp(a.posesLightPos[i], b.posesLightPos[i], c.posesLightPos[i], ret);
						temp.posesLightPos.push_back(xx);
					}
					temp.normal = BaryLerp(a.normal, b.normal, c.normal, ret);
				}
				temp.isValid = true;
				temp.stencil = stencilValue;
				temp.fboX = x;
				temp.fboY = y;
				re.push_back(temp);
			}
		}

	}

	bool Pipeline::EarlyZTest(FrameBuffer& fbo, int x, int y, float depth, CameraType type_)
	{
		if (type_ == CameraType::Ortho)
		{
			if (fbo.GetDepthAttachElem(x, y) < depth)return false;
			else
			{
				fbo.DrawDepthAttach(x, y, depth);
				return true;
			}
		}
		else
		{
			if (fbo.GetDepthAttachElem(x, y) > depth)return false;
			else
			{
				fbo.DrawDepthAttach(x, y, depth);
				return true;
			}
		}
	}

	bool Pipeline::EarlyStencilTest(FrameBuffer& fbo, int x, int y)
	{
		float fboValue = fbo.GetStencilValue(x, y);
		switch (stOp)
		{
		case MSE::StencilOp::None:
			return true;
			break;
		case MSE::StencilOp::Equal:
			if (abs(fboValue - stencilValue) < 0.001)return true;
			else return false;
			break;
		case MSE::StencilOp::Leq:
			if (fboValue < stencilValue - 0.001)return true;
			else return false;
			break;
		case MSE::StencilOp::Leq_Write:
			if (fboValue < stencilValue - 0.001)
			{
				fbo.DrawStencilAttach(x, y, stencilValue);
				return true;
			}
			else return false;
			break;
		case MSE::StencilOp::Req:
			if (fboValue > stencilValue + 0.001)return true;
			else return false;
			break;
		case MSE::StencilOp::Req_Write:
			if (fboValue > stencilValue + 0.001)
			{
				fbo.DrawStencilAttach(x, y, stencilValue);
				return true;
			}
			else return false;
			break;
		default:
			return true;
			break;
		}
	}

	void Pipeline::ToneMapping(FrameBuffer& fbo, unsigned int index)
	{
		if (ToneMap == 0)return;
		for(int i = 0; i < fbo.width; i++)
			for (int j = 0; j < fbo.height; j++)
			{
				Vec4 col = fbo.GetColorAttachElem(i, j);
				col = POSTPROCESS::PP_ToneMap_Reinhard(col);
				fbo.DrawColorAttach(i, j, col);

				// Todo：区分不同的色调映射方法。
			}
	}

	void Pipeline::GammaCorrection(FrameBuffer& fbo, unsigned int index /* = 0 */)
	{
		if (!UseGamma)return;
		for (int i = 0; i < fbo.width; i++)
			for (int j = 0; j < fbo.height; j++)
			{
				Vec4 col = fbo.GetColorAttachElem(i, j);
				col = POSTPROCESS::PP_GammaCorrection(col, gammaBase);
				fbo.DrawColorAttach(i, j, col);
			}
	}
	void Pipeline::PostProcessBeforeToneMap(FrameBuffer& fbo, std::shared_ptr<FrameBuffer> texes, unsigned int index)
	{
	}

	void Pipeline::PostProcessAfterToneMap(FrameBuffer& fbo, std::shared_ptr<FrameBuffer> texes, unsigned int index)
	{
	}
}