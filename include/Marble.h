
#ifndef MARBLE_H
#define MARBLE_H

#include <glm/vec3.hpp>
#include <Shader.h>

class Marble {

	unsigned int m_vao, m_vertices;

	double m_radius;
	glm::vec3 m_pos, m_vel;
	glm::vec3 m_rgb;

	void LoadObjectModel ();
public:
	Marble (glm::vec3 pos, double radius);

	void Update (float dt);
	void Render (Shader &shader);

	void SetPos (glm::vec3 pos);
  void AddPos (glm::vec3 rel_pos);
	void SetVel (glm::vec3 vel);
	void SetRad (double);
	void SetRGB (glm::vec3 rgb);

	double TimeUntil (const Marble &other);
	void Collide (const Marble &other);
};

#endif
