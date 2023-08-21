#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "../external/include/GLEW/glew.h"
#include "../external/include/GLFW/glfw3.h"

#define TRIANGLE_VERTBUF_SIZE 6

static GLuint CompileShader(const std::string& source, GLuint type)
{
    GLuint shader_id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);

    //error handling:
    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int error_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &error_length);
        char* errMsg = (char*)alloca(error_length * sizeof(char));
        glGetShaderInfoLog(shader_id, error_length, &error_length, errMsg);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader!" << std::endl;
        std::cout << errMsg << std::endl;

        glDeleteShader(shader_id);
        return 0;
    }
    
    return shader_id;
}

void GetSrcCode(const std::string& _path, std::string& _srcString)
{
    std::ifstream ShaderSrcFile;
    ShaderSrcFile.open(_path);

    std::string line;
    while(!ShaderSrcFile.eof())
    {
        line.clear();
        std::getline(ShaderSrcFile, line);
        _srcString += line + "\n";
    }

    ShaderSrcFile.close();
}

static GLuint CreateShader(const std::string& _vertexShader, const std::string& _fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(_vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(_fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    GLenum GLEWStatus = glewInit();
    if(GLEWStatus != GLEW_OK) //checking if glewInit was ok
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(GLEWStatus));
        return -1;
    }
    else
    {
        std::cout << "OpenGL ver: " << glGetString(GL_VERSION) << std::endl;
    }

    float vertex1Verts[TRIANGLE_VERTBUF_SIZE] =     //array of vertices
        {
            -0.5f, -0.5,
            -0.2f,  0.3f,
             0.5f, -0.5f
        };
    int vertex1Indices[TRIANGLE_VERTBUF_SIZE/2] =
        {
            0, 1, 2
        };
    
    unsigned int vertex1Buffer; //buffer ID for triangle
    glGenBuffers(1, &vertex1Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex1Buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * TRIANGLE_VERTBUF_SIZE, vertex1Verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //generating shader
    //first, we extract source code:
    std::string vertex_shader;
    std::string fragment_shader;
    GetSrcCode("shaders/stdVS.vs", vertex_shader);
    GetSrcCode("shaders/stdFS.fs", fragment_shader);

    //now we create shader program
    const GLuint shader_program = CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader_program);

    bool bReverse = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 3, GL_FLOAT, &vertex1Indices);
        
        //adding movement to one of triangle's vertices
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * TRIANGLE_VERTBUF_SIZE, vertex1Verts, GL_STATIC_DRAW);
        if(!bReverse)
        {
            vertex1Verts[2] += 0.01f;
        }
        else
        {
            vertex1Verts[2] -= 0.01f;
        }

        if(vertex1Verts[2] >= 0.8f || vertex1Verts[2] <= -0.8f)
        {
            bReverse = !bReverse;
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glfwTerminate();

    return 0;
}
