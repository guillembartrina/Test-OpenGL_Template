
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GLM/gtc/matrix_transform.hpp"

enum OpticType
{
	ORTHOGONAL = 0,
	PERSPECTIVE
};

class Camera
{
public:
	Camera();
	~Camera();

	void setFocus(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up);

	void setOptic_Perspective(float FOV, float ra, float zN, float zF);
	void setOptic_Orthogonal(float l, float r, float b, float t, float zN, float zF);

	void setAs3PC_Default(glm::vec3 min, glm::vec3 max, OpticType type);
	void setAs3PC_Custom(glm::vec3 min, glm::vec3 max, OpticType type, glm::vec3 OBSdir, float dist, glm::vec3 up);

	void setPosition(glm::vec3 offset, bool relative = false);
	void move(glm::vec3 offset);
	void move_OBS(glm::vec3 offset, bool relative = false);
	//void move_VRP(glm::vec3 offset, bool relative = false);

	void rotateX_OBS(float offset, bool relative = true);
	void rotateY_OBS(float offset, bool relative = true);
	void rotateZ_OBS(float offset, bool relative = true);

	void rotateX_VRP(float offset, bool relative = true);
	void rotateY_VRP(float offset, bool relative = true);
	void rotateZ_VRP(float offset, bool relative = true);

	void applyResize(unsigned int w, unsigned int h);

	float *getVM();
	float *getPM();

private:
	OpticType optic;

	/* focus vars */

	glm::vec3 OBS;
	glm::vec3 VRP;
	glm::vec3 up;

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