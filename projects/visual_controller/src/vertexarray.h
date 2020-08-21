#pragma once
#include <glad/glad.h>
#include "vertexbuffer.h"
#include <vector>

class VertexArray {
public:
	static VertexArray* create();
	void bind();
	void layout(std::initializer_list<Attribute> layout);

private:
	unsigned int id;
	VertexArray();
};