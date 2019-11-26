
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include <Marble.h>

Marble::Marble (glm::vec3 pos, double rad):
	m_pos(pos), m_vel(0.0f),
	m_radius(rad),
	m_rgb(0.f)
{
	LoadObjectModel();
}

void Marble::Update (float dt) {
	m_pos += m_vel*dt;
}

void Marble::SetPos (glm::vec3 pos) {
	m_pos = pos;
}
void Marble::AddPos (glm::vec3 rel_pos) {
	m_pos += rel_pos;
}
void Marble::SetRad (double radius) {
	m_radius = radius;
}
void Marble::SetRGB (glm::vec3 rgb) {
	m_rgb = rgb;
}

void Marble::Render (Shader &shader) {
	glm::mat4 model =
		glm::scale(
			glm::translate(
				glm::mat4(1.0f),
				m_pos
			),
			glm::vec3((float)m_radius, (float)m_radius, 1)
	);

	glm::mat4 local(1.0f);

	shader.setModel(model);
	shader.setLocal(local);
	shader.setVec3("rgb", m_rgb);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_vertices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
} // Render

double Marble::TimeUntil (const Marble &other) {
	// Two lines, starting each m_pos and other.m_pos
	// Going in direction m_vel and other.m_vel

	/*
	ind t;
	this = m_pos + t*m_vel
		x = m_x + t*m_u
		y = m_y + t*m_v
		z = m_z + t*m_w
	that = other.m_pos + t*other.m_vel
		x = m_x + t*m_u
		y = m_y + t*m_v
		z = m_z + t*m_w
	diff = this-that
		dx = (m_x - o.m_x) + t*(m_u - o.m_u)
		dy = (m_y - o.m_y) + t*(m_v - o.m_v)
		dz = (m_z - o.m_z) + t*(m_w - o.m_w)
	d2 = dot(diff, diff)
	d2 = [(m_x - o.m_x) + t*(m_u - o.m_u)]^2 + [(m_y - o.m_y) + t*(m_v - o.m_v)]^2 + [(m_z - o.m_z) + t*(m_w - o.m_w)]^2
	d = sqrt(d2)
	[m_r + o.m_r]^2 = [(m_x - o.m_x) + t*(m_u - o.m_u)]^2 + [(m_y - o.m_y) + t*(m_v - o.m_v)]^2 + [(m_z - o.m_z) + t*(m_w - o.m_w)]^2
	[m_r*o.m_r + 2*m_r*o.m_r + o.m_r*o.m_r] = [(m_x-o.m_x)^2 + 2*t*(m_u-o.m_u)*(m_x-o.m_x) + t^2*(m_u-o.m_u)^2] +
						  [(m_y-o.m_y)^2 + 2*t*(m_v-o.m_v)*(m_y-o.m_y) + t^2*(m_v-o.m_v)^2] +
						  [(m_z-o.m_z)^2 + 2*t*(m_w-o.m_w)*(m_z-o.m_z) + t^2*(m_w-o.m_w)^2] +
	t^2 *  [(m_u-o.m_u)^2 + (m_v-o.m_v)^2 + (m_w-o.m_w)^2] +
	t   * 2[(m_u-o.m_u)*(m_x-o.m_x) + (m_v-o.m_v)*(m_y-o.m_y) + (m_w-o.m_w)*(m_z-o.m_z)] +
	1   *  [(m_x-o.m_x)^2 + (m_y-o.m_y)^2 + (m_z-o.m_z)^2 - (m_r+o.m_r)^2]
	a, b, c = ...

	inner = b^2 - 4*a*c
	if inner < 0: no solution
		tmin = -1
		tmax = -1
	if inner = 0: 1 solution:
		tmin = -b / 2*a
		tmax = tmin
	if inner > 0: 2 solutions:
		tmin = [-b - sqrt(b^2 - 4*a*c)] / 2*a
		tmax = [-b + sqrt(b^2 - 4*a*c)] / 2*a
	objective: d = m_radius + other.m_radius
	*/
	/*
	vec3 dpos, dvel;
	vec3_sub(dpos, m_pos, other.m_pos);
	vec3_sub(dvel, m_vel, other.m_vel);

	double rad = m_radius + other.m_radius;

	double a =     vec3_mul_inner(dvel, dvel);
	double b = 2 * vec3_mul_inner(dpos, dvel);
	double c =     vec3_mul_inner(dpos, dpos) - rad*rad;

	double inner = b*b - 4*a*c;
	if (inner < 0)
		return -1;

	double one_over_2a = 0.5 / a;
	if (inner == 0)
		return -b * one_over_2a;

	double sqrt_inner = sqrt(inner);

	double tmin = (-b - sqrt_inner) * one_over_2a;
	double tmax = (-b + sqrt_inner) * one_over_2a;

	if (tmin < 0)
		return tmax;
	return tmin;
	*/
	return -1;

}

void Marble::Collide (const Marble &other) {

}

void Marble::LoadObjectModel () {

	/*
		//Model will be a circle made from a triangle fan,
		//each triangle with one vertex at the center.

		//There will be 1 (center) + n

		Model will be a poly, with (deprecated) side-length 1/sqrt(2)
		Thus, it's circumscribed circle will have radius 1
	*/
	const int segments = 2400;
	glm::vec3 poly[1+segments];

	poly[0] = glm::vec3(0.f,0.f,0.1f);
	for (int i=0; i<segments; i++) {
		float theta = i*2.0f*3.14159f/segments;
		poly[i+1] =
			glm::vec3(
				cosf(theta),
				sinf(theta),
				0.1f
		);
	}
	// {
	// 	  1.0, 0.0, 0.1,
	// 	  0.0, 1.0, 0.1,
	// 	 -1.0, 0.0, 0.1,
	// 	  0.0,-1.0, 0.1
	// };

	m_vertices = 3*segments;
	int indices[m_vertices];
	for (int i=0; i<segments; i++) {
		indices[3*i] = 0;
		indices[3*i+1] = i+1;
		indices[3*i+2] = i+2 <= segments ? i+2 : 1;
	}

	// {
	// 	0, 1, 2,
	// };

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	unsigned int vbo, ebo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(poly), poly, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
