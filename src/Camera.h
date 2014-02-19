#ifndef __CAMERA_H__
#define	__CAMERA_H__

#include <memory>

#include "GameMath.h"

#define __FOV 60.0f
#define __WIDTH 800
#define __HEIGHT 600
#define __Z_NEAR 1.0f
#define __Z_FAR 100.0f;


class Camera {
	private:
		void Init();
		void Update();

		Vector3f m_pos;
		Vector3f m_target;
		Vector3f m_up;

		float m_AngleH;
		float m_AngleV;

		float speed;

		//static std::shared_ptr<Camera> instance;
		static Camera * instance;

		Camera();
	public:

		//static std::shared_ptr<Camera> getInstance();
		static Camera * getInstance();
		//Camera();	// make singleton
		//Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

		const Vector3f& GetPos() const
		{
			return m_pos;
		}

		const Vector3f& GetTarget() const
		{
			return m_target;
		}

		const Vector3f& GetUp() const
		{
			return m_up;
		}

		void moveForward();
		void moveBackward();
		void moveRight();
		void moveLeft();

		void lookUp(float);
		void lookRight(float);

		Point3f getPosition();
		Vector3f getDirection();
		Vector3f getCameraUp();
};

class CameraTransformer {
	private:

		

		Matrix4f transformation;
	public:
		CameraTransformer(Point3f _center);
		Vector3f scale;
		Vector3f center;
		Vector3f rotate;
		Matrix4f* transform();

		struct {
			float fov;	// field of view
			float width;
			float height;
			float zNear;
			float zFar;
		} perspective;
};

#endif

