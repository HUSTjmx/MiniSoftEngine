#include "MSE_Camera.h"

namespace MSE
{
	 Camera::Camera(Vec4 position, Vec4 up, float yaw, float pitch)
	{
		Front = Vec4(0.0f, 0.0f, 1.0f);
		Zoom = ZOOM;
		MovementSpeed = CAMERA_SPEED;
		MouseSensitivity = SENSITIVITY;
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		projectionType = CameraType::Perspective;

		nearPlane = NEAR_PLANE;
		farPlane = FAR_PLANE;

		DirectUseFront = false;
	}

	 Camera::Camera(Vec4 position, Vec4 front)
	 {
		 Front = front;
		 Zoom = ZOOM;
		 MovementSpeed = CAMERA_SPEED;
		 MouseSensitivity = SENSITIVITY;
		 Position = position;
		 projectionType = CameraType::Perspective;
		 WorldUp = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
		 nearPlane = NEAR_PLANE;
		 farPlane = FAR_PLANE;

		 DirectUseFront = true;
	 }

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	{
		Front = Vec4(0.0f, 0.0f, 1.0f);
		Zoom = ZOOM;
		MovementSpeed = CAMERA_SPEED;
		MouseSensitivity = SENSITIVITY;
		Position = Vec4(posX, posY, posZ);
		WorldUp = Vec4(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		projectionType = CameraType::Perspective;
		nearPlane = NEAR_PLANE;
		farPlane = FAR_PLANE;

		DirectUseFront = false;
	}

	Camera& Camera::operator=(const Camera& c)
	{
		this->farPlane = c.farPlane;
		this->Front = c.Front;
		this->nearPlane = c.nearPlane;
		this->Pitch = c.Pitch;
		this->Position = c.Position;
		this->projectionType = c.projectionType;
		this->Right = c.Right;
		this->Up = c.Up;
		this->WorldUp = c.WorldUp;
		this->Yaw = c.Yaw;
		this->Zoom = c.Zoom;
		this->MovementSpeed = c.MovementSpeed;
		this->MouseSensitivity = c.MouseSensitivity;
		this->DirectUseFront = c.DirectUseFront;

		return *this;
	}
	
	Matrix4x4& Camera::GetViewMat()
	{
		if (!DirectUseFront)
		{
			Front.x = cos(Radians(Yaw)) * cos(Radians(Pitch));
			Front.y = sin(Radians(Pitch));
			Front.z = sin(Radians(Yaw)) * cos(Radians(Pitch));
			Front.w = 0.0;
		}

		Vec4 w = Front;
		w.Normalize_V3();
		Vec4 u = WorldUp.Cross_V3(w).Normalize_V3();
		Vec4 v = w.Cross_V3(u).Normalize_V3();

		float view[4][4] = {
			{u.x, v.x, w.x, 0.0f},
			{u.y, v.y, w.y, 0.0f},
			{u.z, v.z, w.z, 0.0f},
			{-Position.Dot_V3(u), -Position.Dot_V3(v), -Position.Dot_V3(w), 1.0}
		};

		viewMat = Matrix4x4(view);
		//viewMat.Transpose();
		//viewMat.ExChange();
		return viewMat;
	}

	Matrix4x4& Camera::GetProjMat(float aspect)
	{
		float f = 1.0f / (float)tan(Radians(Zoom));
		float proj[4][4] = {
			{f / aspect, 0.0f, 0.0f, 0.0f},
			{0.0f, f, 0.0f, 0.0f},
			{0.0f, 0.0f, farPlane / (farPlane - nearPlane), 1.0f},
			{0.0f, 0.0f, -nearPlane * farPlane / (farPlane - nearPlane), 0.0f}
		};

		projMat = Matrix4x4(proj);
		//projMat.Transpose();
		//projMat.ExChange();
		return projMat;

	}

	Matrix4x4& Camera::GetOrthoMat(float width, float height)
	{
		float n = nearPlane;
		float f = farPlane;
		float r = width * 0.5f;
		float l = -width * 0.5f;
		float t = height * 0.5f;
		float b = -height * 0.5f;

		float ortho[4][4] = {
			{2.0f / (r - l), 0.0f, 0.0f, -(r + l) / (r - l)},
			{0.0f, 2.0f / (t - b), 0.0f, -(t + b) / (t - b)},
			{0.0f, 0.0f, 1.0f / (f - n), -n / (f - n)},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

		orthoMat = Matrix4x4(ortho);
		//orthoMat.Transpose();
		//orthoMat.ExChange();
		return orthoMat;
	}

	Matrix4x4& Camera::GetPMat()
	{
		if (projectionType == CameraType::Ortho)
			return orthoMat;
		else
			return projMat;
	}

	void Camera::ProcessMouseScroll(float offset)
	{
		Zoom -= (float)offset * 0.05;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
	void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw -= xoffset;
		Pitch -= yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;

			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
	}
	void Camera::Reset()
	{
		Yaw = YAW_C;
		Pitch = PITCH_C;
		Zoom = ZOOM;
	}
};