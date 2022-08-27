#pragma once
#include <vector>
#include <memory>
#include "MSE_Math.h"


namespace MSE
{
	class Material;
	class Facet;

	extern const unsigned int SCR_WIDTH;
	extern const unsigned int SCR_HEIGHT;

	extern const float YAW_C ;
	extern const float PITCH_C;
	extern const float CAMERA_SPEED;
	extern const float SENSITIVITY;
	extern const float ZOOM;
	extern const float  NEAR_PLANE;
	extern const float FAR_PLANE;

	namespace GEOMETRY
	{
		extern const std::vector<float> cube_01_Vertices;
		extern const std::vector<int> cube_01_Layouts;

		extern const std::vector<float> plane_01_Vertices;
		extern const std::vector<int> plane_01_Layouts;

		extern std::shared_ptr<std::vector<Facet>> GetCubeObj();
		extern std::shared_ptr<std::vector<Facet>> GetPlaneObj();
	}

	namespace LIGHT {
		extern const std::vector<Vec4> colors;
		extern const std::vector<Vec4> dirs;
		extern const std::vector<float> powers;
	}

	namespace COLOR {
		extern const Vec4 BgColor;
		extern const Vec4 Green;
		extern const Vec4 Red;
	}

	namespace DATA
	{
		extern std::vector<std::shared_ptr<Material>> materialList;

		extern void SetupMaterialList();

		extern std::shared_ptr<Material> GetStaticMaterial(unsigned int index);
	}
}

