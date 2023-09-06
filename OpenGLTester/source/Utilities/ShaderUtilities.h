#pragma once
#include <Defines.h>
#include <fstream>

namespace shaderUtils
{
    //compiles provided string with source code into specified type of shader. Returns 0 on error and prints it in console
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

    //extracts uniform variables (int, float, vec2,3,4) from shader source files and returns them as a single struct.
    //does not support uniform declarations followed by a comment or ending in any way different from a semicolon!
    static Uniforms GetUniforms(const std::string& _vsPath, const std::string& _fsPath)
    {
        Uniforms foundUniforms;
        std::string sourcePaths[2];
        sourcePaths[0] = _vsPath;
        sourcePaths[1] = _fsPath;
        std::ifstream ShaderSrcFile;
        std::string line;
        size_t pos;
        std::string varName;

        for (size_t i = 0; i < 2; i++)
        {
            ShaderSrcFile.open(sourcePaths[i]);

            while(!ShaderSrcFile.eof())
            {
                line.clear();
                std::getline(ShaderSrcFile, line);
                if(line.substr(0, 7) == "uniform")
                {
                    if(line.substr(8, 4) == "vec4")
                    {
                        pos = line.find("vec4") + 4 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.v4Uniforms.emplace(varName, -1);
                        continue;
                    }
                    else if(line.find("vec3") != std::string::npos)
                    {
                        pos = line.find("vec3") + 4 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.v3Uniforms.emplace(varName, -1);
                        continue;
                    }
                    else if(line.find("vec2") != std::string::npos)
                    {
                        pos = line.find("vec2") + 4 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.v2Uniforms.emplace(varName, -1);
                        continue;
                    }
                    else if(line.find("float") != std::string::npos)
                    {
                        pos = line.find("float") + 5 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.fUniforms.emplace(varName, -1);
                        continue;
                    }
                    else if(line.find("int") != std::string::npos)
                    {
                        pos = line.find("int") + 3 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.iUniforms.emplace(varName, -1);
                        continue;
                    }
                    else if(line.find("sampler2D") != std::string::npos)
                    {
                        pos = line.find("sampler2D") + 9 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.iUniforms.emplace(varName, -1);
                        continue;
                    }
                    else if(line.find("mat4") != std::string::npos)
                    {
                        pos = line.find("mat4") + 4 + 1; //symbol after data type plus space
                        varName = line.substr(pos);
                        varName.pop_back();
                        foundUniforms.m4Uniforms.emplace(varName, -1);
                        continue;
                    }
                }
            } //end of while loop reading file
            
            ShaderSrcFile.close();
        } //end of source files' loop 
        
        return foundUniforms;
    }
    
}
