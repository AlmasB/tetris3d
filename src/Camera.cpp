#include "Camera.h"

#include <iostream>

//std::shared_ptr<Camera> Camera::instance = nullptr;
Camera * Camera::instance = nullptr;

Camera::Camera() : speed(0.15f) {
    m_pos          = Vector3f(0.0f, 0.0f, 0.0f);
    m_target       = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.normalize();
    m_up           = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
}

Camera* Camera::getInstance() {
	if (instance == nullptr)
		instance = new Camera();
	return instance;
}

/*Camera::Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up) : speed(0.15f) {
    m_pos = Pos;

    m_target = Target;
    m_target.normalize();

    m_up = Up;
    m_up.normalize();

    Init();
}*/

void Camera::Init() {
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
    
    m_AngleV = -toDegree(asin(m_target.y));
}

void Camera::Update() {
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(1.0f, 0.0f, 0.0f);
    View.rotate(m_AngleH, Vaxis);
    View.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.cross(View);
    Haxis.normalize();
    View.rotate(m_AngleV, Haxis);
       
    m_target = View;
    m_target.normalize();

    m_up = m_target.cross(Haxis);
    m_up.normalize();
}

void Camera::moveForward() {
	Vector3f tmp = m_target * speed;
	tmp.y = 0.0f;
	m_pos += tmp;
}

void Camera::moveBackward() {
	Vector3f tmp = m_target * speed;
	tmp.y = 0.0f;
	m_pos -= tmp;
}

void Camera::moveRight() {
	Vector3f Right = m_up.cross(m_target);
	Right.normalize();
	Right *= speed;
	m_pos += Right;
}

void Camera::moveLeft() {
	Vector3f Left = m_target.cross(m_up);
	Left.normalize();
	Left *= speed;
	m_pos += Left;
}

void Camera::lookUp(float units) {
	m_AngleV += units;
	Update();
}

void Camera::lookRight(float units) {
	m_AngleH += units;
	Update();
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
}

/* CAMERA TRANSFORMER CLASS DEFINITION BEGIN */

CameraTransformer::CameraTransformer(Point3f _center) 
	: scale(Vector3f(1.0f, 1.0f, 1.0f)), center(Vector3f(_center.x,_center.y,_center.z)), rotate(Vector3f(0,0,0)) {
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
	cameraTranslationTrans.initTranslationTransform(-camera->getPosition().x, -camera->getPosition().y, -camera->getPosition().z);
	cameraRotateTrans.initCameraTransform(camera->getDirection(), camera->getCameraUp());	// careful passing references
	persProjTrans.initPersProjTransform(perspective.fov, perspective.width, perspective.height, perspective.zNear, perspective.zFar);

	transformation = persProjTrans * cameraRotateTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans;

	/*Matrix4f * trans = &(persProjTrans * cameraRotateTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans);*/

	// perhaps send a new pointer so it doesn't change the member
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
