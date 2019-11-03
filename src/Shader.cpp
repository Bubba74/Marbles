
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "Shader.h"

void Shader::use (){
  glUseProgram(m_ID);
}

Shader::Shader (string vert_shader, string frag_shader) {
  m_ID = glCreateProgram();

  string vertexSource   = readFile(vert_shader);
  string fragmentSource = readFile(frag_shader);

  unsigned int vertexShader, fragmentShader;
  const char *source;

  source = vertexSource.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);

  source = fragmentSource.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);

	//    ------  Error Checking ------ //
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
	    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}

	// --- Shader Linking --- //
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	glLinkProgram(m_ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
			printf("EROOR::SHADER::LINKER::COMPILATION_FAILED\n%s", infoLog);
	}
}

void Shader::setMatrix (string type, glm::mat4 transform) {
  unsigned int loc = glGetUniformLocation(m_ID, type.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)&transform[0][0]);
}
void Shader::setPerspective(glm::mat4 transform) {
  setMatrix("perspective", transform);
}
void Shader::setView(glm::mat4 transform) {
  setMatrix("view", transform);
}
void Shader::setModel(glm::mat4 transform) {
  setMatrix("model", transform);
}
void Shader::setLocal(glm::mat4 transform) {
  setMatrix("local", transform);
}

void Shader::setInt (string key, int value){
  unsigned int vertexLocation = glGetUniformLocation(m_ID, key.c_str());
  glUniform1i(vertexLocation, value);
}
void Shader::setBool (string key, bool value){
  unsigned int vertexLocation = glGetUniformLocation(m_ID, key.c_str());
  glUniform1i(vertexLocation, value);
}
void Shader::setFloat (string key, float value){
  unsigned int vertexLocation = glGetUniformLocation(m_ID, key.c_str());
  glUniform1f(vertexLocation, value);}

//#define DEBUG
string Shader::readFile(string path){

  #ifdef DEBUG
  cout << "Reading from: " << path << endl;
  #endif

  string text;

  {
    ifstream my_file;
    my_file.open(path);

    if (!my_file.is_open()) {
      cout << "Failed to open: " << path << endl;
      return "";
    } else
      cout << "Reading shader file: " << path << endl;

    string line;
    while ( getline(my_file, line) ) {
      text += line;
      text += '\n';
    }
    my_file.close();
  }

  #ifdef DEBUG
  cout << text;
  #endif

  return text;
}
