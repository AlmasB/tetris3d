#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <math.h>

#include "Common.h"

class Camera {
	private:
		Point3 position;
		Vector3 direction;
		Vector3 worldUP;
		float angle;
		float speed;

		void adjustDirection();
	public:
		Camera();
		void moveForward();
		void moveBack();
		void moveRight();
		void moveLeft();

		void rotateUp(float);
		void rotateDown(float);
		void rotateRight(float);
		void rotateLeft(float);

		Point3 getPosition();
		Vector3 getDirection();
		Vector3 getUP();

};

#endif