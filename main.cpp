#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "main.h"

ShapeType shapeType = ShapeType::Tetragon;


void Init()
{
	// �������������� �������
	InitShader();
	// �������������� ��������� �����
	InitVBO();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Window window(sf::VideoMode(600, 600), "Shapes", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true); // ������������ �������������
	window.setActive(true); // ������������� �������� OpenGL
	glewInit(); // �������������� GLEW
	Init(); // �������������� �������
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // ���� ������������ ������ ����
			{
				window.close(); // ��������� ����
				goto EXIT_IS_RIGHT_HERE; // ������� �� �����
			}
			else if (event.type == sf::Event::Resized) // ���� ������������ ������� ������ ����
			{
				glViewport(0, 0, event.size.width, event.size.height); // ������������� ������� ������
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Right)
				{
					shapeType = ShapeType((int)shapeType + 1);
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					shapeType = ShapeType((int)shapeType - 1);
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ����� ����� � ����� �������
		Draw(); // ������
		window.display(); // ������� �� �����
	}
EXIT_IS_RIGHT_HERE: // ����� ������
	Release(); // ������� �������
	return 0; // ������� �� ���������
}

void InitVBO()
{
	glGenBuffers(1, &VBO); // ���������� �����
	Vertex data[] = {
		// Tetragon
		{ -1.0, 1.0 }, // 0
		{ 1.0, 1.0 }, // 1
		{ 1.0, -1.0 }, // 2
		{ -1.0, -1.0 }, // 3
		// TriangleStrip
		// TODO: other shapes
		
	};
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� �����
	// ��������� ������ � �����
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	checkOpenGLerror();
}

void InitShader()
{
	// ������� ��������� ������
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// ��������� �������� ��� �������
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	// ����������� ������
	glCompileShader(vShader);
	// ��������� �� ������
	std::cout << "vertex shader \n";
	ShaderLog(vShader);
	// ������� ����������� ������
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	// ��������� �������� ��� �������
	glShaderSource(fShader, 1, &FragShaderSource, NULL);
	// ����������� ������
	glCompileShader(fShader);
	// ��������� �� ������
	std::cout << "fragment shader \n";
	ShaderLog(fShader);

	// ������� ��������� ���������
	Program = glCreateProgram();
	// ����������� ������� � ���������
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	// ������� ��������� ���������
	glLinkProgram(Program);
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	// ��������� �� ������
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}
	// ���������� ID �������� �� ��������� ���������
	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib" << attr_name << std::endl;
		return;
	}
	checkOpenGLerror();
}

void Draw()
{
	glUseProgram(Program); // ������������� ��������� ���������
	glEnableVertexAttribArray(Attrib_vertex); // �������� �������
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� �����
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); 	// ��������� ������ ��������
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���������� �����
	switch (shapeType)
	{
	case ShapeType::Tetragon:
		glDrawArrays(GL_QUADS, 0, 4); // ������
		break;
	case ShapeType::TriangleStrip:
		break;
	case ShapeType::Pentagon:
		break;
	default:
		break;
	}
	glDisableVertexAttribArray(Attrib_vertex); // ��������� �������
	glUseProgram(0); // ��������� ��������� ���������
	checkOpenGLerror(); // ��������� �� ������
}

void Release()
{
	ReleaseShader(); // ������� �������
	ReleaseVBO(); // ������� �����
}

void ReleaseVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���������� �����
	glDeleteBuffers(1, &VBO); // ������� �����
}

void ReleaseShader()
{
	glUseProgram(0); // ��������� ��������� ���������
	glDeleteProgram(Program); // ������� ��������� ���������
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

// ��������� x �� ��������� y � ������� ��������� ������
inline int leq_x(int x1, int y1, int x2, int y2, int y)
{
	return (y - y1) * (x2 - x1) / (y2 - y1) + x1;
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
