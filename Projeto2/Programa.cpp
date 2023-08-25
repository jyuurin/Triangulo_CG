#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void error_glfw(int error, const char* desc) 
{
	printf("%d: %s", error, desc);
}

GLuint shaderProgram, VAO, VBO;

//definicao de pontos x y que terão, 3 programas que serao executados
static const char * vShader = "                        \n\
#version 330				                           \n\
							                           \n\
layout(location = 0) in vec2 pos;                      \n\
							                           \n\
void main(){					                       \n\
	gl_position = vec4(pos.x, pos.y, 0.0, 1.0);	       \n\
							                           \n\
							                           \n\
}							                           \n\
";
// valor das cores que estao sendo implementadas
static const char* fShader = "                         \n\
#version 330				                           \n\
							                           \n\
uniform vec3 triColor;					               \n\
out vec4 color;							               \n\
void main(){					                       \n\
	color = vec4(triColor, 1.0);					   \n\
							                           \n\
							                           \n\
}							                           \n\
";

void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType) {
	GLuint _shader = glCreateShader(shaderType);
	
	const GLchar* code[1];
	code[0] = shaderCode;

	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(_shader, 1024, NULL, eLog);
		printf("Erro ao compilar %d: %s", shaderType, eLog);
		return;
	}

	glAttachShader(shaderProgram, _shader);
}

void CompileShader() {
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		printf("O programa nao foi iniciado\n");
		return;
	}

	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);
	
	glLinkProgram(shaderProgram);
}

void createTriangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f,
		0.0f, 1.0f,
		1.0f, -1.0f

	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
		//gera um espaco de memoria do tipo buffer
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

//VAO - varios vbos , esta na memoria e por isso precisa do buffer dele
//VectorBufferObject - info do vetor

int main () 
{
	// caso de erro a função vai ser executada
	glfwSetErrorCallback(error_glfw);

	if (!glfwInit()) {
		printf("GLFW não foi iniciado \n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* mainwindow = glfwCreateWindow(800, 600, "Aula 02", NULL, NULL);

	if (!mainwindow) {
		printf("GLFW não conseguiu criar a janela\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mainwindow);

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainwindow, &bufferWidth, &bufferHeight);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Erro ao iniciar o Glew");
		glfwDestroyWindow(mainwindow);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	
	createTriangle();
	CompileShader();


//eqto a janela nao precisa ser fechada...
	while (!glfwWindowShouldClose(mainwindow)) {
		glClearColor(0.3f, 0.8f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
			glBindVertexArray(VAO);

		//cores do triangulo
				GLuint uniColor = glGetUniformLocation(shaderProgram, "triColor");
				glUniform3f(uniColor, 1.0f, 2.0, 0.0f);

				glDrawArrays(GL_TRIANGLES, 0, 3);


			glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainwindow);
		glfwPollEvents();	
	}

	glfwTerminate();
	return 0;
}


