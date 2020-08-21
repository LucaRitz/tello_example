#include "vertexbuffer.h"

VertexBuffer* VertexBuffer::of(size_t size, void* data) {
	return new VertexBuffer(size, data);
}

void VertexBuffer::reset(size_t size, void* data) {
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

VertexBuffer::VertexBuffer(size_t size, void* data) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

Attribute Attribute::of(u16 count, AttributeType type, Normalization normalization) {
	return Attribute(count, type, normalization);
}

size_t getTypeSize(AttributeType type) {
	switch (type) {
	case AttributeType::BYTE:			return 1;
	case AttributeType::UBYTE:			return 1;
	case AttributeType::SHORT:			return 2;
	case AttributeType::USHORT:			return 2;
	case AttributeType::INT:			return 4;
	case AttributeType::UINT:			return 4;
	case AttributeType::HALF_FLOAT:		return 2;
	case AttributeType::FLOAT:			return 4;
	case AttributeType::DOUBLE:			return 8;
	default:
		// TODO: insert assert with message
		return 0;
	}
}

Attribute::Attribute(u16 count, AttributeType type, Normalization normalization) : count(count), type(type), typeSize(getTypeSize(type)), normalization(normalization) {
	
}
