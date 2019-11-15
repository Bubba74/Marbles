
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include <Grid.h>

Grid::Grid ()
{
	LoadObjectModel();
}

void Grid::Render (Shader &shader) {
	glBindVertexArray(m_vao);

	float small = 0.05f, large = 10.0f;
	for (int i=-10; i<=10; i++) {
		glm::mat4 model =
			glm::scale(
				glm::translate(
					glm::mat4(1.0f),
					glm::vec3((float)i, 0, 0)
				),
				glm::vec3(small, large, 1.0f)
		);

		shader.setModel(model);
		glDrawElements(GL_TRIANGLE_STRIP, m_vertices, GL_UNSIGNED_INT, (void*)0);
	}
	for (int i=-10; i<=10; i++) {
		glm::mat4 model =
			glm::scale(
				glm::translate(
					glm::mat4(1.0f),
					glm::vec3(0, (float)i, 0)
				),
				glm::vec3(large, small, 1.0f)
		);

		shader.setModel(model);
		glDrawElements(GL_TRIANGLE_STRIP, m_vertices, GL_UNSIGNED_INT, (void*)0);
	}
	glBindVertexArray(0);

} // Render

void Grid::LoadObjectModel () {

	float square[12] = {
		  1.0, 1.0,-0.1,
		 -1.0, 1.0,-0.1,
		  1.0,-1.0,-0.1,
		 -1.0,-1.0,-0.1
	};

	int indices[] = {
		0, 1, 2, 3
	};
	m_vertices = 4;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	unsigned int vbo, ebo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
