# version 330 core

layout (location = 0) in vec3 position;

uniform mat4 vTransform;

void main() {
	gl_Position = vTransform * vec4(position, 1.0);
}