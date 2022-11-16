#pragma once
// ID ��������� ���������
GLuint Program;
// ID ��������
GLint Attrib_vertex;
// ID ���������� ������
GLuint VBO;

// ��������� ��� �������� ������
struct Vertex
{
	GLfloat x;
	GLfloat y;
};

// ��������� ������
const char* VertexShaderSource = R"(
#version 330 core
in vec2 coord;
out lowp vec4 vcolor;
uniform vec4 ucol;
void main() {
gl_Position = vec4(coord, 0.0, 1.0);
vcolor = vec4(coord.x, coord.y, 1.0 - (coord.x + coord.y), 1.0);
}
)";

// ����������� ������
const char* FragShaderSource = R"(
#version 330 core
in lowp vec4 vcolor;
void main() {
// ������ ���� ��������� (�������)
gl_FragColor = vcolor;
}
)";


// ������� ��� ��������� ������ ����������
void SetIcon(sf::Window& wnd);
// ������� ��� �������� ������
void checkOpenGLerror();
//
void ShaderLog(unsigned int shader);
// ������� ��� �������� ��������
void InitShader();
// ������� ��� ������������� ���������� ������
void InitVBO();
// ������� ��� ������������� ��������
void Init();
// ������� ��� ���������
void Draw();
// ������� ��� ������� ��������
void ReleaseShader();
// ������� ��� ������� ���������� ������
void ReleaseVBO();
// ������� ��� ������� ��������
void Release();

enum class ShapeType
{
	Tetragon = 0,
	TriangleStrip,
	Pentagon,
};