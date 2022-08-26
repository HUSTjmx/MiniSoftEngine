#include "MSE_Camera.h"

namespace MSE
{
	 Camera::Camera(Vec4 position, Vec4 up, float yaw, float pitch)
	{
		Front = Vec4(0.0f, 0.0f, -1.0f);
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
	}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	{
		Front = Vec4(0.0f, 0.0f, -1.0f);
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

		return *this;
	}
	
	Matrix4x4& Camera::GetViewMat()
	{
		Front.x = cos(Radians(Yaw)) * cos(Radians(Pitch));
		Front.y = sin(Radians(Pitch));
		Front.z = sin(Radians(Yaw)) * cos(Radians(Pitch));
		Front.w = 0.0;
		Front.Normalize_V3();

		Vec4 w = Front;
		Vec4 u = WorldUp.Cross_V3(w).Normalize_V3();
		Vec4 v = w.Cross_V3(u).Normalize_V3();

		float view[4][4] = {
			{u.x, v.x, w.x, 0.0f},
			{u.y, v.y, w.y, 0.0f},
			{u.z, v.z, w.z, 0.0f},
			{-Position.x, -Position.y, -Position.z, 1.0}
		};

		viewMat = Matrix4x4(view);

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
		return projMat;

	}

	Matrix4x4& Camera::GetOrthoMat(float width, float height)
	{
		float n = nearPlane;
		float f = farPlane;
		float r = Position.x + width * 0.5;
		float l = Position.x - width * 0.5;
		float t = Position.y + height * 0.5;
		float b = Position.y - height * 0.5;
		float ortho[4][4] = {
			{2.0 / (r - l), 0.0, 0.0, 0.0},
			{0.0, 2.0 / (t - b), 0.0, 0.0},
			{0.0, 0.0, -2.0 / (f - n), 0.0},
			{-(r + l) / (r - l), - (t + b) / (t - b), -(f + b) / (f - n), 1.0}
		};

		orthoMat = Matrix4x4(ortho);
		return orthoMat;
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

		Yaw += xoffset;
		Pitch += yoffset;

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