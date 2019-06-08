
#include "Camera.hpp"

#include <math.h>

Camera::Camera()
{
	optic = OpticType::ORTHOGONAL;
	
	//setdefaultFocus() VM = glm::mat4(1.f);
	//setdefaultOptic() PM = glm::mat4(1.f);
	
	ir = glm::mat4(1.0);
}

Camera::~Camera() {}

void Camera::setFocus(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up)
{
	
	this->OBS = OBS;
	this->VRP = VRP;
	this->up = up;
	
	VM = glm::lookAt(OBS, VRP, up);
	
	//--------------------------------------------
	
	ir = glm::mat4(1.f);
	
	glm::vec3 aux = OBS - VRP;
	
	glm::vec3 auxX = aux;
	auxX.y = 0.f;
	
	if(glm::length(auxX) > 0)
	{
		float tmp = acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(auxX)));
		if(auxX.x >= 0) ir = glm::rotate(ir, -tmp, glm::vec3(0, 1, 0));
		else ir = glm::rotate(ir, tmp, glm::vec3(0, 1, 0));
	}
	
	glm::vec3 auxY = aux;
	auxY.x = 0.f;
	
	if(glm::length(auxY) > 0)
	{
		float tmp = acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(auxY)));
		if(auxY.y >= 0) ir = glm::rotate(ir, tmp, glm::vec3(1, 0, 0));
		else ir = glm::rotate(ir, -tmp, glm::vec3(1, 0, 0));
	}
	
	
	glm::vec3 auxZ = glm::inverse(glm::transpose(glm::mat3(ir))) * up;
	auxZ.z = 0.f;
	
	if(glm::length(auxZ) > 0)
	{
		float tmp = acos(glm::dot(glm::vec3(0, 1, 0), glm::normalize(auxZ)));
		if(auxZ.x >= 0) ir = glm::rotate(ir, tmp, glm::vec3(0, 0, 1));
		else ir = glm::rotate(ir, -tmp, glm::vec3(0, 0, 1));;
	}
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

void Camera::rotateX(float offset)
{
	VM = glm::mat4(1.0);
	VM = glm::translate(VM, glm::vec3(0, 0, -1.f * glm::distance(OBS, VRP)));
	VM = glm::rotate(VM, -offset, glm::vec3(0, 1, 0));
	VM = VM * glm::inverse(ir);
	VM = glm::translate(VM, -1.f * VRP);
	
	ir = glm::rotate(ir, -offset, glm::vec3(0, 1, 0));
}

void Camera::rotateY(float offset)
{
	VM = glm::mat4(1.0);
	VM = glm::translate(VM, glm::vec3(0, 0, -1.f * glm::distance(OBS, VRP)));
	VM = glm::rotate(VM, -offset, glm::vec3(1, 0, 0));
	VM = VM * glm::inverse(ir);
	VM = glm::translate(VM, -1.f * VRP);
	
	ir = glm::rotate(ir, -offset, glm::vec3(1, 0, 0));
}

void Camera::rotateZ(float offset)
{
	VM = glm::mat4(1.0);
	VM = glm::translate(VM, glm::vec3(0, 0, -1.f * glm::distance(OBS, VRP)));
	VM = glm::rotate(VM, -offset, glm::vec3(0, 0, 1));
	VM = VM * glm::inverse(ir);
	VM = glm::translate(VM, -1.f * VRP);
	
	ir = glm::rotate(ir, -offset, glm::vec3(0, 0, 1));
}

void Camera::resize(unsigned int w, unsigned int h)
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