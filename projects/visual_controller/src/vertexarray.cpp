#include "vertexarray.h"

VertexArray* VertexArray::create() {
	return new VertexArray();
}

void VertexArray::bind() {
	glBindVertexArray(id);
}

inline GLenum getType(AttributeType type) {
	switch (type) {
	case AttributeType::BYTE:			return GL_BYTE;
	case AttributeType::UBYTE:			return GL_UNSIGNED_BYTE;
	case AttributeType::SHORT:			return GL_SHORT;
	case AttributeType::USHORT:			return GL_UNSIGNED_SHORT;
	case AttributeType::INT:			return GL_INT;
	case AttributeType::UINT:			return GL_UNSIGNED_INT;
	case AttributeType::HALF_FLOAT:		return GL_HALF_FLOAT;
	case AttributeType::FLOAT:			return GL_FLOAT;
	case AttributeType::DOUBLE:			return GL_DOUBLE;
	default:
		// TODO: insert assert with message
		return 0;
	}
}

void VertexArray::layout(std::initializer_list<Attribute> layout) {

	u32 stride = 0;

	for (auto& attribute : layout) {
		stride += attribute.count * attribute.typeSize;
	}

	u32 offset = 0;
	u32 index = 0;

	for (auto& attribute : layout) {

		GLboolean normalization = attribute.normalization == Normalization::ON ? GL_TRUE : GL_FALSE;
		glVertexAttribPointer(index, attribute.count, getType(attribute.type), normalization, stride, (void*) offset);
		glEnableVertexAttribArray(index);

		offset += attribute.count * attribute.typeSize;
		index++;
	}

}

VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
}
