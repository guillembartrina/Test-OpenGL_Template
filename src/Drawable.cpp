
#include "Drawable.hpp"

#include <iostream>

Drawable::Drawable(GLuint TMlocation)
{
	scale_i = glm::vec3(1.0);

	translate = glm::vec3(0.0);
	scale = glm::vec3(1.0);

	TM = glm::mat4(1.0);
	this->TMlocation = TMlocation;

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

void Drawable::load_FromModel(Model* m, GLuint programID)
{
    if(loaded)
    {
        glDeleteBuffers(VBO.size(), &VBO[0]);
        glDeleteVertexArrays(1, &VAO);
        glGenVertexArrays(1, &VAO);
    }

	//Test phase

	GLint count;
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);

	if(count != 6)
	{
		std::cerr << "Not 6 attributes on shader to link model" << std::endl;
		return;
	}

	for(unsigned int i = 0; i < count; i++)
	{
		GLenum type;

		glGetActiveAttrib(programID, i, 0, NULL, NULL, &type, NULL);

		if(i < 5)
		{
			if(type != GL_FLOAT_VEC3)
			{
				std::cerr << "Not matching some vec3 on shader to link model" << std::endl;
				return;
			}
		}
		else
		{
			if(type != GL_FLOAT)
			{
				std::cerr << "Not matching the float on shader to link model" << std::endl;
				return;
			}
		}
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

	//---

	glm::vec3 min, max;
	getMM_FromModel(m, min, max);

	center_o = (min + max) / 2.f;
	size_o = max - min;

	computeTM();

	//---

    loaded = true;
}

void Drawable::draw() const
{
	if(loaded)
	{
		glUniformMatrix4fv(TMlocation, 1, GL_FALSE, &TM[0][0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertex);
		glBindVertexArray(0);
	}
	else std::cerr << "Drawing drawable while not loaded" << std::endl;
}

void Drawable::computeTM()
{
	TM = glm::mat4(1.0);
  
	for(int i = rotations.size()-1; i >= 0; i--)
	{
		TM = glm::rotate(TM, rotations[i].first, rotations[i].second);
	}
	
	TM = glm::scale(TM, scale);
	
	TM = glm::translate(TM, translate);
	
	for(int i = rotations_i.size()-1; i >= 0; i--)
	{
		TM = glm::rotate(TM, rotations_i[i].first, rotations_i[i].second);
	}
	
	TM = glm::scale(TM, scale_i);
	
	TM = glm::translate(TM, -1.f * center_o);
}

void Drawable::getMM_FromModel(Model* m, glm::vec3& min, glm::vec3& max)
{
	min.x = max.x = m->vertices()[0];
    min.y = max.y = m->vertices()[1];
    min.z = max.z = m->vertices()[2];
    
    for(unsigned int i = 3; i < m->vertices().size(); i+=3)
    {
        if(m->vertices()[i] < min.x) min.x = m->vertices()[i];
        if(m->vertices()[i] > max.x) max.x = m->vertices()[i];
        if(m->vertices()[i+1] < min.y) min.y = m->vertices()[i+1];
        if(m->vertices()[i+1] > max.y) max.y = m->vertices()[i+1];
        if(m->vertices()[i+2] < min.z) min.z = m->vertices()[i+2];
        if(m->vertices()[i+2] > max.z) max.z = m->vertices()[i+2];
    }
}