#pragma once
#include "MSE_Math.h"

namespace MSE
{
	class Material
	{
	public:
		Material();

		Material(Vec4 col, float r, float m, float ref = 0.06);

		Material(const Material& other);

		Material& operator=(const Material& other);

		virtual Vec4 GetDiffuseColor();

		virtual Vec4 GetF0();

		virtual Vec4 GetF90();

		virtual float GetRoughness(int mapOp = 0);

	public:
		Vec4 baseColor;
		float roughness;
		float metallic;
		float reflectance;
		Vec4 emissive;

		int stencilOp;
		int stencilValue;
	};
}