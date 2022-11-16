#pragma once
// ID шейдерной программы
GLuint Program;
// ID атрибута
GLint Attrib_vertex;
// ID вершинного буфера
GLuint VBO;

// Структура для хранения вершин
struct Vertex
{
	GLfloat x;
	GLfloat y;
};

// Вершинный шейдер
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

// Фрагментный шейдер
const char* FragShaderSource = R"(
#version 330 core
in lowp vec4 vcolor;
void main() {
// Задаем цвет фрагмента (красный)
gl_FragColor = vcolor;
}
)";


// Функция для установки иконки приложения
void SetIcon(sf::Window& wnd);
// Функция для проверки ошибок
void checkOpenGLerror();
//
void ShaderLog(unsigned int shader);
// Функция для загрузки шейдеров
void InitShader();
// Функция для инициализации вершинного буфера
void InitVBO();
// Функция для инициализации ресурсов
void Init();
// Функция для отрисовки
void Draw();
// Функция для очистки шейдеров
void ReleaseShader();
// Функция для очистки вершинного буфера
void ReleaseVBO();
// Функция для очистки ресурсов
void Release();

enum class ShapeType
{
	Tetragon = 0,
	TriangleStrip,
	Pentagon,
};