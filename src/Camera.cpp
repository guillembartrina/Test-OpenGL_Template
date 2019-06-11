
#include "Camera.hpp"

#include <math.h>

Camera::Camera()
{
	optic = OpticType::ORTHOGONAL;
	
	//setdefaultFocus() VM = glm::mat4(1.f);
	//setdefaultOptic() PM = glm::mat4(1.f);
}

Camera::~Camera() {}

void Camera::setFocus(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up)
{
	this->OBS = OBS;
	this->VRP = VRP;
	this->up = up;
	
	VM = glm::lookAt(OBS, VRP, up);
}

void Camera::setAsDefault3PCOf(glm::vec3 min, glm::vec3 max, OpticType type)
{
	VRP = (min+max)/2.f;
	radius = glm::distance(min, VRP);
	OBS = VRP + glm::vec3(0, 0, 1) * (2.f * radius);
	up = glm::vec3(0, 1, 0);
	
	setFocus(OBS, VRP, up);
	
	//----
	
	zN = radius;
	zF = 3.f * radius;
	
	switch(type)
	{
		case OpticType::ORTHOGONAL:
		{
			l = -radius;
			r = radius;
			b = -radius;
			t = radius;
			
			PM = glm::ortho(l, r, b, t, zN, zF);
			
			optic = OpticType::ORTHOGONAL;
		}
			break;
		case OpticType::PERSPECTIVE:
		{
			alpha = asin(0.5f);
			FOV = 2.f * alpha;
			ra = 1.f;
			
			PM = glm::perspective(FOV, ra, zN, zF);
			
			optic = OpticType::PERSPECTIVE;
		}
			break;
	}
}

void Camera::setAs3PCOf(glm::vec3 min, glm::vec3 max, OpticType type, glm::vec3 OBSdir, float dist, glm::vec3 up)
{
	VRP = (min+max)/2.f;
	radius = glm::distance(min, VRP);
	OBS = VRP + OBSdir * dist;
	//A generic up generator (don't work with cam parallel to (1, 1, 1)): up = glm::normalize(glm::cross(VRP - OBS, glm::vec3(1, 1, 1)));
	
	setFocus(OBS, VRP, up);
	
	//----
	
	zN = dist - radius;
	zF = dist + radius;
	
	switch(type)
	{
		case OpticType::ORTHOGONAL:
		{
			l = -radius;
			r = radius;
			b = -radius;
			t = radius;
			
			PM = glm::ortho(l, r, b, t, zN, zF);
			
			optic = OpticType::ORTHOGONAL;
		}
			break;
		case OpticType::PERSPECTIVE:
		{
			alpha = asin(radius/dist);
			FOV = 2.f * alpha;
			ra = 1.f;
			
			PM = glm::perspective(FOV, ra, zN, zF);
			
			optic = OpticType::PERSPECTIVE;
		}
			break;
	}
}


void Camera::setOptic_Perspective(float FOV, float ra, float zN, float zF)
{
	optic = OpticType::PERSPECTIVE;
	
	this->FOV = FOV;
	alpha = FOV / 2.f;
	this->ra = ra;
	this->zN = zN;
	this->zF = zF;
	
	PM = glm::perspective(FOV, ra, zN, zF);
}

void Camera::setOptic_Orthogonal(float l, float r, float b, float t, float zN, float zF)
{
	optic = OpticType::ORTHOGONAL;
	
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;
	this->zN = zN;
	this->zF = zF;
	
	PM = glm::ortho(l, r, b, t, zN, zF);
}

void Camera::moveOBS(glm::vec3 offset, bool vrptoo)
{
	OBS += offset;
	if(vrptoo) VRP += offset;
	
	VM = glm::lookAt(OBS, VRP, up);
}

void Camera::moveVRP(glm::vec3 offset, bool relative)
{
	if(relative) VRP += offset;
	else VRP = offset;
	
	VM = glm::lookAt(OBS, VRP, up);
}

void Camera::rotateX(float offset, bool relative)
{
	glm::mat4 tmp(1.f);
	tmp = glm::translate(tmp, VRP);
	
	if(relative)
	{
		tmp = glm::rotate(tmp, -offset, up);
	}
	else
	{
		tmp = glm::rotate(tmp, -offset, glm::vec3(0, 1, 0));
	}
	
	tmp = glm::translate(tmp, -1.f * VRP);
	
	OBS = glm::mat3(tmp) * OBS;
	up = glm::inverse(glm::transpose(glm::mat3(tmp))) * up;
		
	VM = glm::lookAt(OBS, VRP, up);
}

void Camera::rotateY(float offset, bool relative)
{
	glm::mat4 tmp(1.f);
	tmp = glm::translate(tmp, VRP);
	
	if(relative)
	{
		tmp = glm::rotate(tmp, -offset, glm::cross(VRP - OBS, up));
	}
	else
	{	
		tmp = glm::rotate(tmp, -offset, glm::vec3(1, 0, 0));	
	}
	
	tmp = glm::translate(tmp, -1.f * VRP);
	
	OBS = glm::mat3(tmp) * OBS;
	up = glm::inverse(glm::transpose(glm::mat3(tmp))) * up;
		
	VM = glm::lookAt(OBS, VRP, up);
}

void Camera::rotateZ(float offset, bool relative)
{
	glm::mat4 tmp(1.f);
	tmp = glm::translate(tmp, VRP);
	
	if(relative)
	{
		tmp = glm::rotate(tmp, -offset, glm::normalize(OBS - VRP));
	}
	else
	{
		tmp = glm::rotate(tmp, -offset, glm::vec3(0, 0, 1));
	}
	
	tmp = glm::translate(tmp, -1.f * VRP);
	
	OBS = glm::mat3(tmp) * OBS;
	up = glm::inverse(glm::transpose(glm::mat3(tmp))) * up;
		
	VM = glm::lookAt(OBS, VRP, up);
}

void Camera::applyResize(unsigned int w, unsigned int h)
{
	float tmp = float(w)/float(h);
	
	switch(optic)
	{
		case OpticType::ORTHOGONAL:
		{
			if(tmp >= 1.f)
			{
				l = -radius * tmp;
				r = radius * tmp;
			}
			else
			{
				b = -radius / tmp;
				t = radius / tmp;
			}
			
			PM = glm::ortho(l, r, b, t, zN, zF);
		}
			break;
		case OpticType::PERSPECTIVE:
		{
			ra = tmp;
			
			if(tmp < 1.f)
			{
				FOV = 2.f * atan(tan(alpha)/tmp);
			}
			
			PM = glm::perspective(FOV, ra, zN, zF);
		}
			break;
	};
}

GLfloat* Camera::getVM()
{
	return &VM[0][0];
}

GLfloat* Camera::getPM()
{
	return &PM[0][0];
}