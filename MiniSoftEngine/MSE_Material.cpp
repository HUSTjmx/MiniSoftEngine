#include "MSE_Material.h"

namespace MSE
{
	Material::Material()
	{
		baseColor = Vec4(1.0, 0.0, 1.0, 1.0);
		roughness = 0.5;
		metallic = 0.0;
		reflectance = 0.06;
		emissive = Vec4(0.0, 0.0, 0.0, 0.0);
		
		stencilOp = 0;
		stencilValue = 0.0;
	}

	Material::Material(Vec4 col, float r, float m, float ref /* = 0.06 */)
	{
		baseColor = col;
		roughness = r;
		metallic = m;
		reflectance = ref;
		emissive = Vec4(0.0, 0.0, 0.0, 0.0);

		stencilOp = 0;
		stencilValue = 0.0;
	}

	Material::Material(const Material& other)
	{
		baseColor = other.baseColor;
		roughness = other.roughness;
		metallic = other.metallic;
		reflectance = other.reflectance;
		emissive = other.emissive;

		stencilOp = 0;
		stencilValue = 0.0;
	}

	Material& Material::operator=(const Material& other)
	{
		baseColor = other.baseColor;
		roughness = other.roughness;
		metallic = other.metallic;
		reflectance = other.reflectance;
		emissive = other.emissive;

		stencilOp = 0;
		stencilValue = 0.0;

		return *this;
	}

	Vec4 Material::GetDiffuseColor()
	{
		metallic = Clamp(metallic, 0.0, 1.0);
		return baseColor * (1.0 - metallic);
	}

	Vec4 Material::GetF0()
	{
		metallic = Clamp(metallic, 0.0, 1.0);
		reflectance = Clamp(reflectance, 0.2, 0.9);

		float a = 0.16 * reflectance * reflectance;
		return Vec4(a, a, a) * (1.0 - metallic) + baseColor * metallic;
	}

	Vec4 Material::GetF90()
	{
		return Vec4(1.0, 1.0, 1.0, 0.0);
	}

	float Material::GetRoughness(int mapOp)
	{
		roughness = Clamp(roughness, 0.04, 1.0);
		if (mapOp == 0)
		{
			return roughness * roughness;
		}
		return roughness;
	}

}