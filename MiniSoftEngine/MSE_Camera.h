#pragma once
#include <vector>
#include "MSE_Math.h"
#include "MSE_Config.h"

namespace MSE 
{

	enum class CameraType
	{
		Ortho,
		Perspective,
	};

	class Camera
	{
	public:
		Camera(Vec4 position, Vec4 up, float yaw, float pitch = PITCH_C);

		Camera(Vec4 position, Vec4 front);
		
		// constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		Camera& operator=(const Camera& c);

		Matrix4x4& GetViewMat();
		
		Matrix4x4& GetProjMat(float aspect);

		Matrix4x4& GetOrthoMat(float width, float height);

		void ProcessMouseScroll(float offset);

		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

		void Reset();

	public:

		// camera Attributes
		Vec4 Position;
		Vec4 Front;
		Vec4 Up;
		Vec4 Right;
		Vec4 WorldUp;
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
		// euler Angles
		float Yaw;
		float Pitch;
		// Near plane.
		float nearPlane;
		// Far plane.
		float farPlane;

		CameraType projectionType;

		Matrix4x4 viewMat;
		Matrix4x4 projMat;
		Matrix4x4 orthoMat;

		bool DirectUseFront;
	};
}

