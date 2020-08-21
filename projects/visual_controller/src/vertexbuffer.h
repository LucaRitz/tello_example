#pragma once
#include <glad/glad.h>
#include "types.h"

enum class AttributeType : u16 {
	// Note: there are more types, add them as needed
	BYTE,
	UBYTE,
	SHORT,
	USHORT,
	INT,
	UINT,
	HALF_FLOAT,
	FLOAT,
	DOUBLE,
	FIXED
};

enum class Normalization : u8 {
	ON,
	OFF
};

struct Attribute {
public:
	static Attribute of(u16 count, AttributeType type, Normalization normalization);
	const AttributeType type;
	const size_t typeSize;
	const u16 count;
	const Normalization normalization;
private:
	Attribute(u16 count, AttributeType type, Normalization normalization);
};

class VertexBuffer {
public:

	static VertexBuffer* of(size_t size, void* data);

	void reset(size_t size, void* data);
	void bind();

private:
	unsigned int id;

	VertexBuffer(size_t size, void* data);
};