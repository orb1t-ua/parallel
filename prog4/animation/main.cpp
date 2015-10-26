#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

#include <omp.h>

#include "lifematrix.h"

using namespace std;


const GLchar* vertexShaderSource = "#version 330 core\n"
    "in vec2 vertex;\n"
    "out vec2 uv;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(vertex, 0.0, 1.0);\n"
    "uv = vertex * 0.5 + vec2(0.5, 0.5);\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
	"in vec2 uv;\n"
    "out vec4 color;\n"
    "uniform sampler2D tex;\n"
    "void main()\n"
    "{\n"
    "color = texture(tex, uv);\n"
    "}\n\0";
    
const GLfloat vbo_data[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,
        -1.0f, -1.0f
};
GLuint vbo, vao, vs, fs;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
    
int main(int argc, char* argv[]){
	if(argc != 5){
		cout << "Usage: ./program <width> <height> <generations> <threads>" << endl;
		exit(1);
	}
	const GLuint WIDTH = atoi(argv[1]);
	const GLuint HEIGHT = atoi(argv[2]);
	const GLuint N = atoi(argv[3]);
	const GLuint P = atoi(argv[4]);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CA Visualizer", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSource, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSource, NULL);
	glCompileShader(fs);
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	unsigned char* data = new unsigned char[WIDTH*HEIGHT];
	LifeMatrix lm[2];
	lm[0].init(WIDTH, HEIGHT);
	lm[1].init(WIDTH, HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glUseProgram(prog);
	unsigned i = 0;
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		
		if(i >= N){
			lm[0].reset();
			lm[1].reset();
			i = 0;
		}
		
		#pragma omp parallel for num_threads(P)
		for(unsigned y = 0; y < HEIGHT; y++){
			for(unsigned x = 0; x < WIDTH; x++){
				int j = i%2;
				int k = (i+1)%2;
				int neighbors = lm[j].getNumNeighbors(x, y);
				bool alive = lm[j].at(x, y);
				bool shouldLive = lm[j].shouldLive(alive, neighbors);
				lm[k].set(x, y, shouldLive);
				if(shouldLive){
					*(data + x + y*WIDTH) = 255;
				}
				else{
					*(data + x + y*WIDTH) = 0;
				}
			}
		}
		
		if(P == 1){
			sleep(1);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(glGetUniformLocation(prog, "tex"), 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glfwSwapBuffers(window);
		i++;
	}
	
	lm[1].destroy();
	lm[0].destroy();
	delete[] data;

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
