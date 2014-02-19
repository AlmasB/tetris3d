/*

	Copyright 2010 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Camera.h"

#include <iostream>

Camera::Camera() : speed(0.15f) {
    m_pos          = Vector3f(0.0f, 0.0f, 0.0f);
    m_target       = Vector3f(0.0f, 0.0f, 1.0f);
    m_target.normalize();
    m_up           = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
}


Camera::Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up) : speed(0.15f) {
    m_pos = Pos;

    m_target = Target;
    m_target.normalize();

    m_up = Up;
    m_up.normalize();

    Init();
}

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
