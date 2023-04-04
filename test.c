#include <SDL.h>
#include <SDL_opengl.h>

const char* vertex_shader_source =
"#version 120\n"
"attribute vec2 position;\n"
"attribute vec4 color;\n"
"varying vec4 vertex_color;\n"
"void main() {\n"
"   gl_Position = vec4(position, 0.0, 1.0);\n"
"   vertex_color = color;\n"
"}\n";

const char* fragment_shader_source =
"#version 120\n"
"varying vec4 vertex_color;\n"
"void main() {\n"
"   gl_FragColor = vertex_color;\n"
"}\n";

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GL_CreateContext(window);

	// Set up OpenGL
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Define the vertices of the rectangle
	GLfloat vertices[] = {
		100.0f, 100.0f,  // Top-left
		200.0f, 100.0f,  // Top-right
		200.0f, 200.0f,  // Bottom-right
		100.0f, 200.0f   // Bottom-left
	};

	// Define the colors of the rectangle
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,  // Top-left (red)
		0.0f, 1.0f, 0.0f, 1.0f,  // Top-right (green)
		0.0f, 0.0f, 1.0f, 1.0f,  // Bottom-right (blue)
		1.0f, 1.0f, 1.0f, 1.0f   // Bottom-left (white)
	};

	// Create and compile the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Check for vertex shader compilation errors
	GLint vertex_shader_status;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_status);
	if (vertex_shader_status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
		printf("Vertex shader compilation error: %s\n", buffer);
		exit(1);
	}

	// Create and compile the fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Check for fragment shader compilation errors
	GLint fragment_shader_status;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_status);
	if (fragment_shader_status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fragment_shader, 512, NULL, buffer);
		printf("Fragment shader compilation error: %s\n", buffer);
		exit(1);
	}

	// Create the shader program and attach the vertex and fragment shaders
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	// Bind the vertex attribute "position" to location 0
	glBindAttribLocation(shader_program, 0, "position");

	// Bind the vertex attribute "color" to location 1
	glBindAttribLocation(shader_program, 1, "color");

	// Link the shader program
	glLinkProgram(shader_program);

	// Check for shader program linking errors
	GLint shader_program_status;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_program_status);
	if (shader_program_status != GL_TRUE) {
		char buffer[512];
		glGetProgramInfoLog(shader_program, 512, NULL, buffer);
		printf("Shader program linking error: %s\n", buffer);
		exit(1);
	}

	// Get the location of the vertex attribute "position"
	GLint position_location = glGetAttribLocation(shader_program, "position");

	// Get the location of the vertex attribute "color"
	GLint color_location = glGetAttribLocation(shader_program, "color");

	// Set up the vertex array object (VAO)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Set up the vertex buffer object (VBO) for the vertices
	GLuint vertex_vbo;
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up the vertex buffer object (VBO) for the colors
	GLuint color_vbo;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	// Enable the vertex attribute "position"
	glEnableVertexAttribArray(position_location);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// Enable the vertex attribute "color"
	glEnableVertexAttribArray(color_location);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	// Render the rectangle
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
	SDL_GL_SwapWindow(window);

	// Clean up resources
	glDeleteProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteBuffers(1, &vertex_vbo);
	glDeleteBuffers(1, &color_vbo);
	glDeleteVertexArrays(1, &vao);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
