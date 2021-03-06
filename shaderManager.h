#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// シェーダのソースファイルを読み込む
// file_path: file path to shader source
// buffer: text of read source file
bool readShaderSource(const char *file_path, std::vector<GLchar> &buffer) {
  if (file_path == NULL)
    return false;

  // ソールファイルを開く
  std::ifstream file(file_path, std::ios::binary);
  if (file.fail()) {
    std::cerr << "cannot open source file: " << file_path << '\n';
    return false;
  }

  file.seekg(0L, std::ios::end);
  GLsizei length = static_cast<GLsizei>(file.tellg());

  buffer.resize(length + 1);

  file.seekg(0L, std::ios::beg);
  file.read(buffer.data(), length);
  buffer[length] = '\0';

  if (file.fail()) {
    std::cerr << "Error: cannot read source file: " << file_path << '\n';
    return false;
  }

  file.close();
  return true;
}

// シェーダオブジェクトのコンパイル結果を表示する
// shader: シェーダオブジェクト名
// str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char *str) {
  // コンパイル結果を取得する
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
    std::cerr << "Compile Error in " << str << std::endl;

  // シェーダのコンパイル時のログの長さを取得する
  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
  if (bufSize > 1) {
    // シェーダのコンパイル時のログの内容を取得する
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
    std::cerr << &infoLog[0] << std::endl;
  }
  return static_cast<GLboolean>(status);
}

// プログラムオブジェクトのリンク結果を表示する
// program: プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program) {
  // リンク結果を取得する
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
    std::cerr << "Link Error." << std::endl;

  // シェーダのリンク時のログの長さを取得する
  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
  if (bufSize > 1) {
    // シェーダのリンク時のログの内容を取得する
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
    std::cerr << &infoLog[0] << std::endl;
  }
  return static_cast<GLboolean>(status);
}

GLuint createProgram(const char *vsrc, const char *fsrc) {
  const GLuint program = glCreateProgram();

  if (vsrc != NULL) {
    const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(vobj, 1, &vsrc, NULL);
    glCompileShader(vobj);

    // バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
    if (printShaderInfoLog(vobj, "vertex shader"))
      glAttachShader(program, vobj);
    glDeleteShader(vobj);
  }

  if (fsrc != NULL) {
    // フラグメントシェーダのシェーダオブジェクトを作成する
    const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(fobj, 1, &fsrc, NULL);
    glCompileShader(fobj);
    // フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
    if (printShaderInfoLog(fobj, "fragment shader"))
      glAttachShader(program, fobj);
    glDeleteShader(fobj);
  }

  // プログラムオブジェクトをリンクする
  glLinkProgram(program);

  // 作成したプログラムオブジェクトを返す
  if (printProgramInfoLog(program))
    return program;

  // プログラムオブジェクトが作成できなければ 0 を返す
  glDeleteProgram(program);
  return 0;
}

GLuint loadProgram(const char *vert, const char *frag) {
  std::vector<GLchar> vsrc;
  const bool vstat(readShaderSource(vert, vsrc));
  std::vector<GLchar> fsrc;
  const bool fstat(readShaderSource(frag, fsrc));

  return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}
