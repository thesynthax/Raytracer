#include "shader.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const std::string filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

GLuint ID;
Shader::Shader(const std::string vertexPath, const std::string fragmentPath) {

    std::string vertexCode = get_file_contents(vertexPath);
    std::string fragmentCode = get_file_contents(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment);

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkLinkErrors(ID);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() { 
    glUseProgram(ID); 
}
void Shader::setBool(const char* name, bool value) const {         
    glUniform1i(glGetUniformLocation(ID, name), (int)value); 
}
void Shader::setInt(const char* name, int value) const { 
    glUniform1i(glGetUniformLocation(ID, name), value); 
}
void Shader::setFloat(const char* name, float value) const { 
    glUniform1f(glGetUniformLocation(ID, name), value); 
}
void Shader::setVec2(const char* name, float v1, float v2) const { 
    glUniform2f(glGetUniformLocation(ID, name), v1, v2); 
}
bool Shader::getBool(const char* name) const {
    int *v;
    glGetUniformiv(ID, glGetUniformLocation(ID, name), v);
    return (bool)*v;
}
int Shader::getInt(const char* name) const {
    int *v;
    glGetUniformiv(ID, glGetUniformLocation(ID, name), v);
    return *v;
}
float Shader::getFloat(const char* name) const {
    float *v;
    glGetUniformfv(ID, glGetUniformLocation(ID, name), v);
    return *v;
}
void Shader::deactivate() {
    glDeleteProgram(ID);
}

void Shader::checkCompileErrors(GLuint shader) {
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

void Shader::checkLinkErrors(GLuint program) {
    int success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
