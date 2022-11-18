#pragma once
#define SHAPE_TYPES 3
// ID ��������� ���������
GLuint Program;
// ID ��������
GLint Attrib_vertex;
GLint Attrib_color;
// ID ���������� ������
GLuint VBO;

// ��������� ��� �������� ������
struct Vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

// ��������� ������
const char* VertexShaderSource = R"(
#version 330 core
in vec2 coord;
in vec4 color;
out vec4 vcolor;
void main() {
    gl_Position = vec4(coord, 0.0, 1.0);
    vcolor = color;
}
)";

// ����������� ������
const char* FragShaderSource = R"(
#version 330 core
uniform vec4 ucol;
in vec4 vcolor;
void main() {
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
void Draw(sf::Window& window);
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

#define red    1.0, 0.0, 0.0, 1.0
#define green  0.0, 1.0, 0.0, 1.0
#define blue   0.0, 0.0, 1.0, 1.0
#define yellow 1.0, 1.0, 0.3, 1.0
#define orange 1.0, 0.5, 0.0, 1.0
#define violet 0.5, 0.0, 1.0, 1.0

struct PentagonFunc
{
	inline float operator()(int i)
	{
		return 2 * M_PI / 5 * i;
	}
};