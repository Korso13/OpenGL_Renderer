#pragma once
#include <PCH.h>
#include <fstream>

namespace shaderUtils
{
    //compiles provided string with source code into specified type of shader. Returns 0 on error and prints it in console
    GLuint CompileShader(const std::string& source, GLuint type)
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

    //reads source code from provided path to shader source code and writes it into provided string (_srcString)
    static void GetSrcCode(const std::string& _path, std::string& _srcString)
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

    //creates shader program build using provided source code for vertex and fragment shaders. Returns program id
    static GLuint CreateShader(const std::string& _vertexShader, const std::string& _fragmentShader)
    {
        GLuint program = glCreateProgram();
        GLuint vs = shaderUtils::CompileShader(_vertexShader, GL_VERTEX_SHADER);
        GLuint fs = shaderUtils::CompileShader(_fragmentShader, GL_FRAGMENT_SHADER);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);
    
        return program;
    }

}
