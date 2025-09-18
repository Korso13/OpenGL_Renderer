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

static void APIENTRY GLDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "[OpenGL] Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "[OpenGL]Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "[OpenGL]Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "[OpenGL]Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "[OpenGL]Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "[OpenGL]Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "[OpenGL]Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "[OpenGL]Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "[OpenGL]Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "[OpenGL]Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "[OpenGL]Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "[OpenGL]Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "[OpenGL]Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "[OpenGL]Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "[OpenGL]Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "[OpenGL]Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "[OpenGL]Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "[OpenGL]Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "[OpenGL]Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "[OpenGL]Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

#define PRINT_ERRS GLGetAllErrors(__FUNCSIG__, __LINE__ - 1)

#ifdef DEBUG 
    #define GLCall(func) PRINT_ERRS; func; PRINT_ERRS
#else DEBUG
    #define GLCall(func) func
#endif

