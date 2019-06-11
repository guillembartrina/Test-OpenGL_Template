
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GL/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

enum OpticType { ORTHOGONAL = 0, PERSPECTIVE };

class Camera
{
	public:
	
	Camera();
	~Camera();
	
	void setFocus(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up);
	
	void setOptic_Perspective(float FOV, float ra, float zN, float zF);
	void setOptic_Orthogonal(float l, float r, float b, float t, float zN, float zF);
	//void setOptics_Fustrum();
	
	void setAsDefault3PCOf(glm::vec3 min, glm::vec3 max, OpticType type);
	void setAs3PCOf(glm::vec3 min, glm::vec3 max, OpticType type, glm::vec3 OBSdir, float dist, glm::vec3 up);
	
	void rotateX(float offset);
	void rotateY(float offset);
	void rotateZ(float offset);
	
	void resize(unsigned int w, unsigned int h);
	
	GLfloat* getVM();
	GLfloat* getPM();
	
	
	private:
	
	OpticType optic;
	
	/* focus vars */
	
	glm::vec3 OBS;
	glm::vec3 VRP;
	glm::vec3 up;
	
	glm::mat4 ir;
	
	glm::mat4 VM;
	
	/* optic vars */
	
	float FOV;
	float ra;
	
	float alpha;
	float radius;
	
	float l;
	float r;
	float b;
	float t;
	
	float zN;
	float zF;
	
	glm::mat4 PM;
};

#endif