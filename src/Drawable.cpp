
#include "Drawable.hpp"

#include <iostream>

Drawable::Drawable()
{
    loaded = false;
}

Drawable::~Drawable()
{
    if(loaded)
    {
        glDeleteBuffers(VBO.size(), &VBO[0]);
        glDeleteVertexArrays(1, &VAO);
    }
}

void Drawable::load_FromModel(Model* m)
{
    if(loaded)
    {
        glDeleteBuffers(VBO.size(), &VBO[0]);
        glDeleteVertexArrays(1, &VAO);
        glGenVertexArrays(1, &VAO);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBO = std::vector<GLuint>(6);
    glGenBuffers(6, &VBO[0]);

	vertex = m->faces().size()*3;

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex*3, m->VBO_vertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex*3, m->VBO_normals(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex*3, m->VBO_matamb(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex*3, m->VBO_matdiff(), GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex*3, m->VBO_matspec(), GL_STATIC_DRAW);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex, m->VBO_matshin(), GL_STATIC_DRAW);

	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(5);

    glBindVertexArray(0);

    loaded = true;
}

void Drawable::draw() const
{
	if(loaded)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertex);
	}
	else std::cerr << "Drawing drawable while not loaded" << std::endl;
}