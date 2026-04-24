#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec2 offset; // Η μεταβλητή για την κίνηση [cite: 27]
out vec3 ourColor;

void main() {
    // Εφαρμογή μετατόπισης [cite: 27]
    gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z, 1.0);
    ourColor = aColor;
}
