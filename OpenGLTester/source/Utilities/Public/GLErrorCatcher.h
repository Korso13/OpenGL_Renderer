#pragma once
#include <pch.h>

#define ASSERT(x) if(!(x)) __debugbreak();

static void PrintError(const GLenum& _errorType, const char* _funcName, const int& _line)
{
    std::string ErrorText = "OpenGL error in function " + std::string{_funcName} + " in line " + std::to_string(_line) + ". ";
    switch (_errorType)
    {
    case (GL_INVALID_ENUM):
        {
            ErrorText += "GL_INVALID_ENUM. An unacceptable value is specified for an enumerated argument. The offending command is ignored.";
            break;
        }
    case (GL_INVALID_VALUE):
        {
            ErrorText += "GL_INVALID_VALUE. A numeric argument is out of range. The offending command is ignored.";
            break;
        }
    case (GL_INVALID_OPERATION):
        {
            ErrorText += "GL_INVALID_OPERATION. The specified operation is not allowed in the current state. The offending command is ignored.";
            break;
        }
    case (GL_INVALID_FRAMEBUFFER_OPERATION):
        {
            ErrorText += "GL_INVALID_FRAMEBUFFER_OPERATION. The framebuffer object is not complete. The offending command is ignored.";
            break;
        }
    case (GL_OUT_OF_MEMORY):
        {
            ErrorText += "GL_OUT_OF_MEMORY. There is not enough memory left to execute the command. The state of the GL is undefined.";
            break;
        }
    case (GL_STACK_UNDERFLOW):
        {
            ErrorText += "GL_STACK_UNDERFLOW. An attempt has been made to perform an operation that would cause an internal stack to underflow.";
            break;
        }
    case (GL_STACK_OVERFLOW):
        {
            ErrorText += "GL_STACK_OVERFLOW. An attempt has been made to perform an operation that would cause an internal stack to overflow.";
            break;
        }
    default:
        {
            ErrorText += "Unspecified error " + std::to_string(_errorType) + "!";
            break;
        }
    }
    
    std::cout << "OpenGL Error: \n" << ErrorText << std::endl;
}

static void GLGetAllErrors(const char* _funcName, const int _line)
{
    GLenum ErrorType;
    while ((ErrorType = glGetError()) != GL_NO_ERROR)
    {
        PrintError(ErrorType, _funcName, _line);
    }
}

#define PRINT_ERRS GLGetAllErrors(__FUNCSIG__, __LINE__ - 1)

#ifdef DEBUG 
    #define GLCall(func) PRINT_ERRS; func; PRINT_ERRS
#else DEBUG
    #define GLCall(func) func
#endif

