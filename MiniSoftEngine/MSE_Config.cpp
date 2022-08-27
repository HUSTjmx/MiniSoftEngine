#include "MSE_Config.h"
#include "MSE_Material.h"
#include "MSE_Object.h"
#include <iostream>

namespace MSE
{
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	const float YAW_C = 90.0f;
	const float PITCH_C = -45.0f;
	const float ZOOM = 45.0f;
    const float CAMERA_SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float  NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 100.0f;


	namespace GEOMETRY {
		const std::vector<float> cube_01_Vertices = {
			// Back face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // bottom-right         
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f,  // top-right
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// top-left
			// Front face
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,  // top-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top-right
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // top-left
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
			// Left face
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f,// top-right
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-right
			// Right face
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top-right         
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // top-left
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,// bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			// Top face
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,// top-left
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,// bottom-right
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,// top-right     
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,// bottom-right
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,// top-left
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f// bottom-left   
		};
		const std::vector<int> cube_01_Layouts = { 3, 3, 3 };

		const std::vector<float> plane_01_Vertices = {
			// Positions          // Normals         // Texture Coords
			5.0f, -0.5f, 5.0f, 0.0f, -1.0f, 0.0f, 5.0f, 0.0f,
			-5.0f, -0.5f, -5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 5.0f,
			-5.0f, -0.5f, 5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

			5.0f, -0.5f, 5.0f, 0.0f, -1.0f, 0.0f, 5.0f, 0.0f,
			5.0f, -0.5f, -5.0f, 0.0f, -1.0f, 0.0f, 5.0f, 5.0f,
			-5.0f, -0.5f, -5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 5.0f
		};
		const std::vector<int> plane_01_Layouts = { 3, 3, 2 };

		std::shared_ptr<std::vector<Facet>> GetCubeObj()
		{
			static std::shared_ptr<std::vector<Facet>> cube = std::make_shared<std::vector<Facet>>();
			if (cube->size() >= 2)return cube;
			const std::vector<float>& vertices = cube_01_Vertices;
			const std::vector<int>& layout = cube_01_Layouts;

			int offset = 0;
			for (int i = 0; i < layout.size(); i++)
				offset += layout[i];

			int pointsNum = vertices.size() / offset;
			if (vertices.size() % offset != 0)
				std::cout << "Vertices Data Is Error!" << std::endl;

			for (int i = 0; i < pointsNum; i += 3)
			{
				std::vector<int> startPos(3, 0);
				startPos[0] = i * offset;
				startPos[1] = startPos[0] + offset;
				startPos[2] = startPos[1] + offset;

				Facet temp;
				temp.p1.pos = Vec4(vertices[startPos[0]], vertices[startPos[0] + 1], vertices[startPos[0] + 2], 1.0);
				if (layout.size() >= 2)temp.p1.normal = Vec4(vertices[startPos[0] + 3], vertices[startPos[0] + 4], vertices[startPos[0] + 5], 0.0);
				if (layout.size() >= 3 && layout[2] == 3)temp.p1.color = Vec4(vertices[startPos[0] + 6], vertices[startPos[0] + 7], vertices[startPos[0] + 8], 0.0);
				else temp.p1.UV = Vec4(vertices[startPos[0] + 6], vertices[startPos[0] + 7], 0.0, 0.0);

				temp.p2.pos = Vec4(vertices[startPos[1]], vertices[startPos[1] + 1], vertices[startPos[1] + 2], 1.0);
				if (layout.size() >= 2)temp.p2.normal = Vec4(vertices[startPos[1] + 3], vertices[startPos[1] + 4], vertices[startPos[1] + 5], 0.0);
				if (layout.size() >= 3 && layout[2] == 3)temp.p2.color = Vec4(vertices[startPos[1] + 6], vertices[startPos[1] + 7], vertices[startPos[1] + 8], 0.0);
				else temp.p2.UV = Vec4(vertices[startPos[1] + 6], vertices[startPos[1] + 7], 0.0, 0.0);

				temp.p3.pos = Vec4(vertices[startPos[2]], vertices[startPos[2] + 1], vertices[startPos[2] + 2], 1.0);
				if (layout.size() >= 2)temp.p3.normal = Vec4(vertices[startPos[2] + 3], vertices[startPos[2] + 4], vertices[startPos[2] + 5], 0.0);
				if (layout.size() >= 3 && layout[2] == 3)temp.p3.color = Vec4(vertices[startPos[2] + 6], vertices[startPos[2] + 7], vertices[startPos[2] + 8], 0.0);
				else temp.p3.UV = Vec4(vertices[startPos[2] + 6], vertices[startPos[2] + 7], 0.0, 0.0);

				cube->push_back(temp);
			}
			return cube;
		}

		std::shared_ptr<std::vector<Facet>> GetPlaneObj()
		{
			static std::shared_ptr<std::vector<Facet>> plane = std::make_shared<std::vector<Facet>>();
			if (plane->size() >= 2)return plane;
			const std::vector<float>& vertices = plane_01_Vertices;
			const std::vector<int>& layout = plane_01_Layouts;

			int offset = 0;
			for (int i = 0; i < layout.size(); i++)
				offset += layout[i];

			int pointsNum = vertices.size() / offset;
			if (vertices.size() % offset != 0)
				std::cout << "Vertices Data Is Error!" << std::endl;

			for (int i = 0; i < pointsNum; i += 3)
			{
				std::vector<int> startPos(3, 0);
				startPos[0] = i * offset;
				startPos[1] = startPos[0] + offset;
				startPos[2] = startPos[1] + offset;

				Facet temp;
				temp.p1.pos = Vec4(vertices[startPos[0]], vertices[startPos[0] + 1], vertices[startPos[0] + 2], 1.0);
				if (layout.size() >= 2)temp.p1.normal = Vec4(vertices[startPos[0] + 3], vertices[startPos[0] + 4], vertices[startPos[0] + 5], 0.0);
				if (layout.size() >= 3 && layout[2] == 3)temp.p1.color = Vec4(vertices[startPos[0] + 6], vertices[startPos[0] + 7], vertices[startPos[0] + 8], 0.0);
				else temp.p1.UV = Vec4(vertices[startPos[0] + 6], vertices[startPos[0] + 7], 0.0, 0.0);

				temp.p2.pos = Vec4(vertices[startPos[1]], vertices[startPos[1] + 1], vertices[startPos[1] + 2], 1.0);
				if (layout.size() >= 2)temp.p2.normal = Vec4(vertices[startPos[1] + 3], vertices[startPos[1] + 4], vertices[startPos[1] + 5], 0.0);
				if (layout.size() >= 3 && layout[2] == 3)temp.p2.color = Vec4(vertices[startPos[1] + 6], vertices[startPos[1] + 7], vertices[startPos[1] + 8], 0.0);
				else temp.p2.UV = Vec4(vertices[startPos[1] + 6], vertices[startPos[1] + 7], 0.0, 0.0);

				temp.p3.pos = Vec4(vertices[startPos[2]], vertices[startPos[2] + 1], vertices[startPos[2] + 2], 1.0);
				if (layout.size() >= 2)temp.p3.normal = Vec4(vertices[startPos[2] + 3], vertices[startPos[2] + 4], vertices[startPos[2] + 5], 0.0);
				if (layout.size() >= 3 && layout[2] == 3)temp.p3.color = Vec4(vertices[startPos[2] + 6], vertices[startPos[2] + 7], vertices[startPos[2] + 8], 0.0);
				else temp.p3.UV = Vec4(vertices[startPos[2] + 6], vertices[startPos[2] + 7], 0.0, 0.0);

				plane->push_back(temp);
			}
			return plane;
		}
	}

	namespace LIGHT {
		const std::vector<Vec4> colors = {
			Vec4(0.9, 0.9, 0.9, 1.0)
		};
		const std::vector<Vec4> dirs = {
			Vec4(-0.3, -0.7, 0.0, 0.0)
		};
		const std::vector<float> powers = {
			10
		};
	}

	namespace COLOR {
		// static unsigned int bgColor = ((123 << 16) | (195 << 8) | 221);
		const Vec4 BgColor = Vec4(123.0 / 255.0, 195.0 / 255.0, 221.0 / 225.0, 1.0);
		const Vec4 Green = Vec4(0.0, 1.0, 0.0, 1.0);
		const Vec4 Red = Vec4(1.0, 0.0, 0.0, 1.0);
	}

	namespace DATA
	{
		std::vector<std::shared_ptr<Material>> materialList;

		void SetupMaterialList()
		{
			// 基本电介质材质_01
			std::shared_ptr<Material> m1 = std::make_shared<Material>();
			m1->baseColor = Vec4(0.5, 0.0, 0.5);
			m1->metallic = 0.1;
			m1->emissive = 0.0;
			m1->reflectance = 0.3;
			m1->roughness = 0.1;

			std::shared_ptr<Material> m2 = std::make_shared<Material>();
			m2->baseColor = Vec4(0.5, 0.3, 0.5);
			m2->metallic = 0.3;
			m2->emissive = 0.0;
			m2->reflectance = 0.3;
			m2->roughness = 0.1;

			materialList.push_back(m1);
			materialList.push_back(m2);

		}

		std::shared_ptr<Material> GetStaticMaterial(unsigned int index)
		{
			return materialList[index];
		}

		
	}
}