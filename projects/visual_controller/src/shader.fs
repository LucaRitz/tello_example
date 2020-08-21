# version 330 core

out vec4 FragColor;

uniform vec3 droneColor;

void main() {
	FragColor = vec4(droneColor, 1.0);
}