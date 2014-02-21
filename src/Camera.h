/*#ifndef __CAMERA_H__
#define	__CAMERA_H__

#include <memory>
#include <iostream>
#include "GameMath.h"
#include "Movable.h"
#include "Debug.h"

// TODO: implement these
#define __FOV 60.0f
#define __WIDTH 800
#define __HEIGHT 600
#define __Z_NEAR 1.0f
#define __Z_FAR 100.0f;

struct Perspective {
	float fov;	// field of view
	float width;
	float height;
	float zNear;
	float zFar;
};

class Camera : public Movable {
	friend class CameraTransformer;
	private:
		//void init();
		//void update();

		//static std::shared_ptr<Camera> instance;

		Point3f center;
		static Camera * instance;
		Perspective cameraPerspective;

		std::shared_ptr<Movable> assigned;

		Camera();
	public:
		void updateView();
		//static std::shared_ptr<Camera> getInstance();
		static Camera * getInstance();
		void move(const Vector3f & v);
		Point3f getCenter();
		void setPerspective(float, float, float, float, float);

		//Camera();	// make singleton
		//Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

		

		void follow(std::shared_ptr<Movable> objectToFollow);

		//void printDebug();
};

class CameraTransformer {
	private:
		Matrix4f transformation;
	public:
		CameraTransformer(Point3f _center);
		Vector3f scale;
		Vector3f center;
		Vector3f rotate;
		const Matrix4f* transform();

		void printDebug();

		struct {
			float fov;	// field of view
			float width;
			float height;
			float zNear;
			float zFar;
		} perspective;
};

#endif*/

