#ifndef SHADER_H
#define SHADER_H

#include <cwchar>
#include <glad/glad.h> 
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  
std::string get_file_contents(const std::string filename);

class Shader {
public:
    GLuint ID;
    Shader(const std::string vertexPath, const std::string fragmentPath); 
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void deactivate();
    

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
  
#endif
