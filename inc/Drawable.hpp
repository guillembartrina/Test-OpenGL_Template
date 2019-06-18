
#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <string>
#include <vector>
#include <utility>

#include "GL/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "model.hpp"

class Drawable
{
    public:

    Drawable(GLuint TMlocation);
    ~Drawable();

    void load_FromModel(Model* m, GLuint programID);

    void draw() const;
    

    private:

    GLuint VAO;
    std::vector<GLuint> VBO;
    unsigned int vertex;

    glm::vec3 center_o;
	glm::vec3 size_o;

    glm::vec3 scale_i;
    std::vector<std::pair<float, glm::vec3>> rotations_i;

    glm::vec3 translate;
	glm::vec3 scale;
	std::vector<std::pair<float, glm::vec3>> rotations;

    glm::mat4 TM;
    GLuint TMlocation;

    bool loaded;

    void computeTM();

    static void getMM_FromModel(Model* m, glm::vec3& min, glm::vec3& max);
};

#endif