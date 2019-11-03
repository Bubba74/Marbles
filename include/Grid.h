
#ifndef GRID_H
#define GRID_H

#include <Shader.h>

class Grid {

	unsigned int m_vao, m_vertices;

	void LoadObjectModel ();
public:
	Grid ();

	void Render (Shader &shader);

};

#endif
