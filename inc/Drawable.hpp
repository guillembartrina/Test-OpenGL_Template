
#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <string>
#include <vector>

#include "GL/glew.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "model.hpp"

class Drawable
{
    public:

    Drawable();
    ~Drawable();

    void load_FromModel(Model* m, GLuint progid);

    void draw() const;
    

    private:

    GLuint VAO;
    std::vector<GLuint> VBO;
    unsigned int vertex;

    bool loaded;
};

#endif