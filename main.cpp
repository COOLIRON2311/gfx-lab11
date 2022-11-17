#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <corecrt_math_defines.h>
#include "main.h"

ShapeType shapeType = ShapeType::Tetragon;


void Init()
{
	// Инициализируем шейдеры
	InitShader();
	// Инициализируем вершинный буфер
	InitVBO();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Window window(sf::VideoMode(600, 600), "Shapes", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true); // Вертикальная синхронизация
	window.setActive(true); // Устанавливаем контекст OpenGL
	glewInit(); // Инициализируем GLEW
	Init(); // Инициализируем ресурсы
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // Если пользователь закрыл окно
			{
				window.close(); // Закрываем окно
				goto EXIT_IS_RIGHT_HERE; // Выходим из цикла
			}
			else if (event.type == sf::Event::Resized) // Если пользователь изменил размер окна
			{
				glViewport(0, 0, event.size.width, event.size.height); // Устанавливаем область вывода
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Right)
				{
					int t = (int)shapeType + 1;
					if (t == SHAPE_TYPES)
						t = 0;
					shapeType = ShapeType(t);
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					int t = (int)shapeType - 1;
					if (t < 0)
						t = SHAPE_TYPES - 1;
					shapeType = ShapeType(t);
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем буфер цвета и буфер глубины
		Draw(window); // Рисуем
		window.display(); // Выводим на экран
	}
EXIT_IS_RIGHT_HERE: // Метка выхода
	Release(); // Очищаем ресурсы
	return 0; // Выходим из программы
}

void InitVBO()
{
	glGenBuffers(1, &VBO); // Генерируем буфер
	PentagonFunc f;
	Vertex data[] = {
		// Tetragon
		{ -1.0 / 2, 1.0 / 2, red}, // 0
		{ 1.0 / 2, 1.0 / 2, green}, // 1
		{ 1.0 / 2, -1.0 / 2, blue}, // 2
		{ -1.0 / 2, -1.0 / 2, yellow}, // 3
		// TriangleStrip
		{ 0.0f / 2, -1.0f / 2, red }, // 0
		{ -1.0f / 2, 0.0f / 2, green }, // 1
		{ -0.77f / 2, 0.77f / 2, blue }, // 2
		{ 0.0f / 2, 1.0f / 2, yellow }, // 3
		{ 0.77f / 2, 0.77f / 2, red }, // 4
		{ 1.0f / 2, 0.0f / 2, green }, // 5
		// Pentagon
		{ cosf(f(1)) / 2, sinf(f(1)) / 2, red }, // 0
		{ cosf(f(2)) / 2, sinf(f(2)) / 2, green }, // 1
		{ cosf(f(3)) / 2, sinf(f(3)) / 2, blue }, // 2
		{ cosf(f(4)) / 2, sinf(f(4)) / 2, yellow }, // 3
		{ cosf(f(5)) / 2, sinf(f(5)) / 2, green }, // 4
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем буфер
	// Загружаем данные в буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	checkOpenGLerror();
}

void InitShader()
{
	// Создаем вершинный шейдер
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// Загружаем исходный код шейдера
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	// Компилируем шейдер
	glCompileShader(vShader);
	// Проверяем на ошибки
	std::cout << "vertex shader \n";
	ShaderLog(vShader);
	// Создаем фрагментный шейдер
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Загружаем исходный код шейдера
	glShaderSource(fShader, 1, &FragShaderSource, NULL);
	// Компилируем шейдер
	glCompileShader(fShader);
	// Проверяем на ошибки
	std::cout << "fragment shader \n";
	ShaderLog(fShader);

	// Создаем шейдерную программу
	Program = glCreateProgram();
	// Прикрепляем шейдеры к программе
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	// Линкуем шейдерную программу
	glLinkProgram(Program);
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	// Проверяем на ошибки
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}
	// Вытягиваем ID атрибута из шейдерной программы
	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib" << attr_name << std::endl;
		return;
	}

	const char* attr_name2 = "color";
	Attrib_color = glGetAttribLocation(Program, attr_name2);
	if (Attrib_color == -1)
	{
		std::cout << "could not bind attrib" << attr_name2 << std::endl;
		return;
	}

	checkOpenGLerror();
}

void Draw(sf::Window& window)
{
	glUseProgram(Program); // Устанавливаем шейдерную программу
	glEnableVertexAttribArray(Attrib_vertex); // Включаем атрибут
	glEnableVertexAttribArray(Attrib_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем буфер
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0); 	// Указываем данные атрибута
	glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Отвязываем буфер
	glUniform4f(glGetUniformLocation(Program, "ucol"), (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);
	switch (shapeType)
	{
	case ShapeType::Tetragon:
		window.setTitle("Tetragon");
		glDrawArrays(GL_QUADS, 0, 4); // Рисуем
		break;
	case ShapeType::TriangleStrip:
		window.setTitle("Triangle Strip");
		glDrawArrays(GL_TRIANGLE_FAN, 4, 6);
		break;
	case ShapeType::Pentagon:
		window.setTitle("Pentagon");
		glDrawArrays(GL_POLYGON, 10, 5);
		break;
	default:
		break;
	}
	glDisableVertexAttribArray(Attrib_vertex); // Отключаем атрибут
	glUseProgram(0); // Отключаем шейдерную программу
	checkOpenGLerror(); // Проверяем на ошибки
}

void Release()
{
	ReleaseShader(); // Очищаем шейдеры
	ReleaseVBO(); // Очищаем буфер
}

void ReleaseVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Отвязываем буфер
	glDeleteBuffers(1, &VBO); // Удаляем буфер
}

void ReleaseShader()
{
	glUseProgram(0); // Отключаем шейдерную программу
	glDeleteProgram(Program); // Удаляем шейдерную программу
}

void ShaderLog(unsigned int shader)
{
	int infologLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 1)
	{
		int charsWritten = 0;
		std::vector<char> infoLog(infologLen);
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
		std::cout << "InfoLog: " << infoLog.data() << std::endl;
	}
}

void checkOpenGLerror()
{
	GLenum errCode;
	const GLubyte* errString;
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		std::cout << "OpenGL error: " << errString << std::endl;
	}
}

void SetIcon(sf::Window& wnd)
{
	sf::Image image;
	image.create(16, 16);
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			image.setPixel(i, j, { (uint8_t)(i * 16), (uint8_t)(j * 16), 0 });
		}
	}

	wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}
