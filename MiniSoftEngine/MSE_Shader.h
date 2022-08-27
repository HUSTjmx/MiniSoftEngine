#pragma once
#include <memory>
#include <vector>
namespace MSE
{
	class Vertex;
	class Matrix4x4;
	class Camera;
	class Material;
	class FrameBuffer;
	class Vec4;
	class Light;
	class Fragment;

	class vertShader
	{
	public:
		virtual void Process(const Vertex& v, Vertex& v_R, const Matrix4x4& model, std::shared_ptr<Camera> camera);
	};

	class fragShader
	{
	public:
		virtual void Process(std::vector<Fragment>& frags, FrameBuffer& fbo, const std::vector<Light>& lights, std::shared_ptr<Camera> camera, std::shared_ptr<Material> material);
	};

	class fragShader_DepthDraw
	{
		virtual void Process(std::vector<Fragment>& frags, FrameBuffer& fbo, const std::vector<Light>& lights, std::shared_ptr<Camera> camera, std::shared_ptr<Material> material);
	};
}