#include "Camera.h"

#include <iostream>

//std::shared_ptr<Camera> Camera::instance = nullptr;
Camera * Camera::instance = nullptr;

// camera to be in inheritance game object

Camera::Camera() : Movable(), center(Point3f(0,0,0)) {
	assigned = nullptr;
    /*m_pos          = Vector3f(0.0f, 0.0f, 0.0f);
    m_target       = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.normalize();
    m_up           = Vector3f(0.0f, 1.0f, 0.0f);

	Vector3f HTarget(m_target.x, 0.0, m_target.z);
	HTarget.normalize();

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = 360.0f - toDegree(asin(HTarget.z));
		}
		else
		{
			m_AngleH = 180.0f + toDegree(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = toDegree(asin(-HTarget.z));
		}
		else
		{
			m_AngleH = 90.0f + toDegree(asin(-HTarget.z));
		}
	}

	m_AngleV = -toDegree(asin(m_target.y));*/
}

Camera* Camera::getInstance() {
	if (instance == nullptr)
		instance = new Camera();
	return instance;
}

void Camera::follow(std::shared_ptr<Movable> objectToFollow) {
	assigned = objectToFollow;
}

void Camera::updateView() {
	if (assigned != nullptr) {
		center = assigned->getCenter();
		direction = assigned->getDirection();
		up = assigned->getUpVector();
	}
}

void Camera::move(const Vector3f& v) {
	center += v;
}

Point3f Camera::getCenter() {
	return center;
}

/*void Camera::init() {
    
}

void Camera::update() {
    const Vector3f vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f view(1.0f, 0.0f, 0.0f);
    view.rotate(m_AngleH, vaxis);
    view.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f haxis = vaxis.cross(view);
    haxis.normalize();
    view.rotate(m_AngleV, haxis);
       
    m_target = view;
    m_target.normalize();

    m_up = m_target.cross(haxis);
    m_up.normalize();
}

void Camera::moveForward() {
	//m_pos += (m_target * speed);	// for moving in Y as well
	Vector3f tmp = m_target * speed;
	tmp.y = 0.0f;
	m_pos += tmp;
}

void Camera::moveBackward() {
	//m_pos -= (m_target * speed);
	Vector3f tmp = m_target * speed;
	tmp.y = 0.0f;
	m_pos -= tmp;
}

void Camera::moveRight() {
	Vector3f right = m_up.cross(m_target);
	right.normalize();
	right *= speed;
	m_pos += right;
}

void Camera::moveLeft() {
	Vector3f left = m_target.cross(m_up);
	left.normalize();
	left *= speed;
	m_pos += left;
}

void Camera::lookUp(float units) {
	m_AngleV += units;
	update();
}

void Camera::lookRight(float units) {
	m_AngleH += units;
	update();
}

Point3f Camera::getPosition() {
	return Point3f(m_pos.x, m_pos.y, m_pos.z);
}

Vector3f Camera::getDirection() {
	return Vector3f(m_target.x, m_target.y, m_target.z);
}

Vector3f Camera::getCameraUp() {
	return Vector3f(m_up.x, m_up.y, m_up.z);
}

void Camera::printDebug() {
	std::cout << "POS:" << m_pos.x << " " << m_pos.y << " " << m_pos.z << std::endl;
	std::cout << "DIR:" << m_target.x << " " << m_target.y << " " << m_target.z << std::endl;
	std::cout << "UP:" << m_up.x << " " << m_up.y << " " << m_up.z << std::endl;
}*/

/* CAMERA TRANSFORMER CLASS DEFINITION BEGIN */

CameraTransformer::CameraTransformer(Point3f _center)
: scale(Vector3f(1.0f, 1.0f, 1.0f)), center(Vector3f(_center.x, _center.y, _center.z)), rotate(Vector3f(0, 0, 0)) {
	perspective.fov = 60.0f;
	perspective.width = 800;
	perspective.height = 600;
	perspective.zNear = 1.0f;
	perspective.zFar = 100.0f;
}

const Matrix4f * CameraTransformer::transform() {
	Matrix4f scaleTrans, rotateTrans, translationTrans, cameraTranslationTrans, cameraRotateTrans, persProjTrans;

	// scale, rotate, translate
	scaleTrans.initScaleTransform(scale.x, scale.y, scale.z);
	rotateTrans.initRotateTransform(rotate.x, rotate.y, rotate.z);
	translationTrans.initTranslationTransform(center.x, center.y, center.z);

	Camera * camera = Camera::getInstance();

	// camera transformations
	cameraTranslationTrans.initTranslationTransform(-camera->getCenter().x, -camera->getCenter().y, -camera->getCenter().z);
	cameraRotateTrans.initCameraTransform(camera->getDirection(), camera->getUpVector());	// careful passing references
	persProjTrans.initPersProjTransform(perspective.fov, perspective.width, perspective.height, perspective.zNear, perspective.zFar);

	transformation = persProjTrans * cameraRotateTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans;

	/*Matrix4f * trans = &(persProjTrans * cameraRotateTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans);*/

	return &transformation;
	//return trans;
}

void CameraTransformer::printDebug() {
	/*std::cout << perspective.fov << std::endl;
	std::cout << perspective.width << std::endl;
	std::cout << perspective.height << std::endl;
	std::cout << perspective.zNear << std::endl;
	std::cout << perspective.zFar << std::endl;*/

	std::cout << center.x << " " << center.y << " " << center.z << std::endl;
}
