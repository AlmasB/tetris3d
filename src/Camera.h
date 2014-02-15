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

		void moveUp();
		void moveDown();

		void lookUp(float);

		/**
		* Rotates camera right by given units
		* Note: you can rotate camera left by passing negative value
		*
		* @param units
		*
		*/
		void lookRight(float);

		Point3 getPosition();
		Vector3 getDirection();
		Vector3 getUP();
};

#endif