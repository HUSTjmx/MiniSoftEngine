#include "MSE_Object.h"
#include "MSE_FrameBuffer.h"
#include "MSE_Camera.h"
#include <iostream>

namespace MSE {
	Vertex::Vertex()
	{
		pos = Vec4();
		normal = Vec4();
		color = Vec4();
	}

	Vertex::Vertex(Vec4 p)
	{
		pos = p;
		normal = Vec4();
		color = Vec4();
	}

	Vertex::Vertex(Vec4 p, Vec4 n)
	{
		pos = p;
		normal = n;
		color = Vec4();
	}

	Vertex::Vertex(Vec4 p, Vec4 n, Vec4 c)
	{
		pos = p;
		normal = n;
		color = c;
	}

	Vertex::Vertex(const Vertex& other)
	{
		pos = other.pos;
		normal = other.normal;
		color = other.color;
		posWS = other.posWS;
		posesLightPos = other.posesLightPos;
		UV = other.UV;
	}

	Facet::Facet()
	{
		p1 = Vertex();
		p2 = Vertex();
		p3 = Vertex();

		p1_R = p1;
		p2_R = p2;
		p3_R = p3;
	}

	Facet::Facet(Vertex a, Vertex b, Vertex c)
	{
		p1 = a;
		p2 = b;
		p3 = c;

		p1_R = p1;
		p2_R = p2;
		p3_R = p3;
	}

	Facet::Facet(const Facet& other)
	{
		p1 = other.p1;
		p2 = other.p2;
		p3 = other.p3;

		p1_R = other.p1_R;
		p2_R = other.p2_R;
		p3_R = other.p3_R;
	}

	Fragment::Fragment(const Fragment& other)
	{
		color = other.color;
		normal = other.normal;
		posWS = other.posWS;
		posesLightPos = other.posesLightPos;
		UV = other.UV;
		depth = other.depth;
		stencil = other.stencil;
		isValid = other.isValid;

		fboX = other.fboX;
		fboY = other.fboY;
	}

	Fragment& Fragment::operator=(const Fragment& other)
	{
		color = other.color;
		normal = other.normal;
		posWS = other.posWS;
		posesLightPos = other.posesLightPos;
		UV = other.UV;
		depth = other.depth;
		stencil = other.stencil;
		isValid = other.isValid;

		fboX = other.fboX;
		fboY = other.fboY;
		return *this;
	}

	Object::Object(std::shared_ptr<std::vector<Facet>> p)
	{
		facets = p;
		material = nullptr;
		type = ObjType::opaque;
		position = Vec4(0.0, 0.0, 0.0, 1.0);
		scale = Vec4(1.0, 1.0, 1.0, 0.0);
		rotate = Vec4(0.0, 0.0, 0.0, 0.0);
		modelMat = Matrix4x4();
	}

	Object::Object(const Object& other)
	{
		facets = other.facets;
		material = other.material;
		type = other.type;
		position = other.position;
		scale = other.scale;
		rotate = other.rotate;
		CalModelMat();
	}

	Object::Object(const std::vector<float>& vertices, const std::vector<int>& layout)
	{
		material = nullptr;
		type = ObjType::opaque;
		position = Vec4(0.0, 0.0, 0.0, 1.0);
		scale = Vec4(1.0, 1.0, 1.0, 0.0);
		rotate = Vec4(0.0, 0.0, 0.0, 0.0);
		modelMat = Matrix4x4();
		LoadVertices(vertices, layout);
	}

	void Object::LoadVertices(const std::vector<float>& vertices, const std::vector<int>& layout)
	{
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
			if(layout.size() >= 2)temp.p1.normal = Vec4(vertices[startPos[0] + 3], vertices[startPos[0] + 4], vertices[startPos[0] + 5], 0.0);
			if(layout.size() >= 3)temp.p1.color = Vec4(vertices[startPos[0] + 6], vertices[startPos[0] + 7], vertices[startPos[0] + 8], 0.0);
			temp.p2.pos = Vec4(vertices[startPos[1]], vertices[startPos[1] + 1], vertices[startPos[1] + 2], 1.0);
			if (layout.size() >= 2)temp.p2.normal = Vec4(vertices[startPos[1] + 3], vertices[startPos[1] + 4], vertices[startPos[1] + 5], 0.0);
			if (layout.size() >= 3)temp.p2.color = Vec4(vertices[startPos[1] + 6], vertices[startPos[1] + 7], vertices[startPos[1] + 8], 0.0);
			temp.p3.pos = Vec4(vertices[startPos[2]], vertices[startPos[2] + 1], vertices[startPos[2] + 2], 1.0);
			if (layout.size() >= 2)temp.p3.normal = Vec4(vertices[startPos[2] + 3], vertices[startPos[2] + 4], vertices[startPos[2] + 5], 0.0);
			if (layout.size() >= 3)temp.p3.color = Vec4(vertices[startPos[2] + 6], vertices[startPos[2] + 7], vertices[startPos[2] + 8], 0.0);

			facets->push_back(temp);
		}
	}

	Matrix4x4& Object::CalModelMat()
	{
		Matrix4x4 baseMat = Matrix4x4();
		float T[4][4] = {
			{1.0, 0.0, 0.0, position.x},
			{0.0, 1.0, 0.0, position.y},
			{0.0, 0.0, 1.0, position.z},
			{0.0, 0.0, 0.0, 1.0}
		};

		float S[4][4] = {
			{scale.x, 0.0, 0.0, 0.0},
			{0.0, scale.y, 0.0, 0.0},
			{0.0, 0.0, scale.z, 0.0},
			{0.0, 0.0, 0.0,     1.0}
		};

		float RX[4][4] = {
			{1.0, 0.0, 0.0, 0.0},
			{0.0, cos(rotate.x), -sin(rotate.x), 0.0},
			{0.0, sin(rotate.x), cos(rotate.x), 0.0},
			{0.0, 0.0, 0.0, 1.0}
		};

		float RY[4][4] = {
			{cos(rotate.y), 0.0, sin(rotate.y), 0.0},
			{0.0, 1.0, 0.0, 0.0},
			{-sin(rotate.y), 0.0, cos(rotate.y), 0.0},
			{0.0, 0.0, 0.0, 1.0}
		};

		Matrix4x4 T_M = Matrix4x4(T);
		Matrix4x4 S_M = Matrix4x4(S);
		Matrix4x4 R_M = Mul(Matrix4x4(RX), Matrix4x4(RY));

		baseMat = Mul(R_M, baseMat);
		baseMat = Mul(S_M, baseMat);
		baseMat = Mul(T_M, baseMat);
		modelMat = baseMat.Transpose();
		modelMat.ExChange();
		modelMat.InvTranspose();
		return modelMat;
	}

	Light::Light()
	{
		dir = Vec4(0.0, -1.0, 0.0, 0.0);
		color = Vec4(0.8, 0.8, 0.8, 0.0);
		power = 10.0;
		CanCastShadow = false;
		fbo = std::make_shared<FrameBuffer>();
		camera = std::make_shared<Camera>(Vec4(0.0, 8.0, 0.0), dir);
		camera->projectionType = CameraType::Ortho;
		camera->farPlane = 20.0f;
		camera->nearPlane = 0.01f;
	}

	Light::Light(Vec4 c, Vec4 d, float p)
	{
		dir = d;
		color = c;
		power = p;
		CanCastShadow = false;
		fbo = std::make_shared<FrameBuffer>();
		//Vec4 XX = dir;
		//XX = XX * -1;
		camera = std::make_shared<Camera>(Vec4(0.0, 4.0, 0.0), dir);
		camera->projectionType = CameraType::Ortho;
		camera->farPlane = 20.0f;
		camera->nearPlane = 0.01f;
	}

}