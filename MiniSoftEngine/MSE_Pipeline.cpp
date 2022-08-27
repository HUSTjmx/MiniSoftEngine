#include "MSE_Pipeline.h"
#include "MSE_BRDF.hpp"
#include "MSE_Shader.h"

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
		if (v.z > v.w) return true;
		if (v.x < -v.w) return true;
		if (v.x > v.w) return true;
		if (v.y < -v.w) return true;
		if (v.y > v.w) return true;
		return false;

	}

	void Pipeline::Rasterize(Facet& facet, FrameBuffer& fbo, std::vector<Fragment>& re, Vec4 param1 /* = Vec4() */, Vec4 param2 /* = Vec4() */)
	{
		const Vertex& a = facet.p1_R;
		const Vertex& b = facet.p2_R;
		const Vertex& c = facet.p3_R;

		float xl = a.pos.x; if (b.pos.x < xl) xl = b.pos.x; if (c.pos.x < xl) xl = c.pos.x;
		float xr = a.pos.x; if (b.pos.x > xr) xr = b.pos.x; if (c.pos.x > xr) xr = c.pos.x;
		float yt = a.pos.y; if (b.pos.y < yt) yt = b.pos.y; if (c.pos.y < yt) yt = c.pos.y;
		float yb = a.pos.y; if (b.pos.y > yb) yb = b.pos.y; if (c.pos.y > yb) yb = c.pos.y;

		int xMin = (int)(xl + 0.5f), xMax = (int)(xr + 0.5f), yMin = (int)(yt + 0.5f), yMax = (int)(yb + 0.5f);
		for (int x = xMin; x <= xMax; ++x)
		{
			for (int y = yMin; y <= yMax; ++y)
			{
				// ���ǲ���
				Vec4 ret = BaryCentric(a.pos, b.pos, c.pos, { (float)x, (float)y, 0.0f, 0.0f });
				if (ret.x < 0 || ret.y < 0 || ret.z < 0) continue;

				// ��ǰ��Ȳ���
				float depth = (a.pos.z * ret.x + b.pos.z * ret.y + c.pos.z * ret.z);
				if (!EarlyZTest(fbo, x, y, depth)) continue;

				// ��ǰģ�����
				if (UseStencil && EarlyStencilTest(fbo, x, y)) continue;

				Fragment temp;
				temp.depth = depth;
				temp.color = BaryLerp(a.color, b.color, c.color, ret);
				temp.posWS = BaryLerp(a.posWS, b.posWS, c.posWS, ret);
				temp.normal = BaryLerp(a.normal, b.normal, c.normal, ret);
				temp.isValid = true;
				temp.stencil = stencilValue;
				temp.fboX = x;
				temp.fboY = y;

				re.push_back(temp);
			}
		}

	}

	bool Pipeline::EarlyZTest(FrameBuffer& fbo, int x, int y, float depth)
	{
		if (fbo.GetDepthAttachElem(x, y) < depth)return false;
		else
		{
			fbo.DrawDepthAttach(x, y, depth);
			return true;
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

				// Todo�����ֲ�ͬ��ɫ��ӳ�䷽����
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