#pragma once
#include <vector>
#include <memory>
#include "MSE_Math.h"
#include "MSE_Material.h"

namespace MSE {

	class FrameBuffer;
	class Camera;

	enum class ObjType {
		opaque,
		transparent,
	};

	class Vertex
	{
	public:
		Vertex();

		Vertex(Vec4 p);

		Vertex(Vec4 p, Vec4 n);

		Vertex(Vec4 p, Vec4 n, Vec4 c);

		Vertex(const Vertex& other);

	public:
		Vec4 pos;
		Vec4 normal;
		Vec4 color;
		Vec4 posWS;
		std::vector<Vec4> posesLightPos;
		Vec4 UV;
	};

	class Facet {
	public:
		Facet();

		Facet(Vertex a, Vertex b, Vertex c);

		Facet(const Facet& other);

	public:
		Vertex p1;
		Vertex p2;
		Vertex p3;

		Vertex p1_R;
		Vertex p2_R;
		Vertex p3_R;
	};

	class Fragment {
	public:
		Fragment() {}

		Fragment(const Fragment& other);

		Fragment& operator=(const Fragment& other);

	public:
		Vec4 color;
		Vec4 posWS;
		std::vector<Vec4> posesLightPos;
		Vec4 normal;
		Vec4 UV;
		float depth;
		float stencil;
		bool isValid;

		int fboX;
		int fboY;
	};

	class Object {
	public:

		Object(std::shared_ptr<std::vector<Facet>> p);

		Object(const std::vector<float>& vertices, const std::vector<int>& layout);

		Object(const Object& other);

		void LoadVertices(const std::vector<float>& vertices, const std::vector<int>& layout);

		Matrix4x4& CalModelMat();

	public:
		std::shared_ptr<std::vector<Facet>> facets;
		std::shared_ptr<Material> material;
		ObjType type;

		Vec4 position;
		Vec4 scale;
		Vec4 rotate;

		Matrix4x4 modelMat;
	};

	class Light
	{
	public:
		Light();

		Light(Vec4 c, Vec4 d, float p);

	public:
		Vec4 dir;
		Vec4 color;
		float power;
		bool CanCastShadow;
		std::shared_ptr<FrameBuffer> fbo;
		std::shared_ptr<Camera> camera;
	};

	class Scene
	{
	public:
		std::vector<Object> OpaqueObjects;
		std::vector<Object> TransObjects;
		std::vector<Light> lights;
	};
}