#include <cstdio>
#include "../external/include/GLEW/glew.h"
#include "../external/include/GLFW/glfw3.h"
#include "source/Utilities/ShaderUtilities.h"
#include "source/Utilities/GLErrorCatcher.h"

#define TRIANGLE_VERTBUF_SIZE 8

#define COLOR_CYCLE_SPEED 0.01f
#define GB_BIAS_CYCLE_SPEED 0.005f
#define COLOR_MIN_VALUE 0.2f
#define COLOR_GB_BIAS_DEF 0.2f
#define COLOR_POOL_SIZE 2.0f

// ReSharper disable once CppInconsistentNaming
struct vec3
{
    float x = COLOR_MIN_VALUE;
    float y = COLOR_MIN_VALUE;
    float z = COLOR_MIN_VALUE;
};

/**
 * global var for cycling color
 */
static vec3 used_color{0.f, 0.f, 0.f};
static float bias = COLOR_GB_BIAS_DEF;
static bool bBiasIncrement = true;
static bool bRedIncrement = true;

void cycleRectColor(const GLuint& _shaderProgram)
{
    //create color pool
    float pool = COLOR_POOL_SIZE;
    //check if need to change color dynamic direction
    if(used_color.x >= 1.f)
    {
        bRedIncrement = false;
    }
    else if(used_color.x <= COLOR_MIN_VALUE)
    {
        bRedIncrement = true;
    }

    //calculate new color based on red and color pool
    used_color.x += (bRedIncrement) ? (COLOR_CYCLE_SPEED) : (-COLOR_CYCLE_SPEED);
    pool -= used_color.x;
    used_color.y = pool * bias;
    pool -= used_color.y;
    used_color.z = pool;

    //check if need to change green and blue bias change direction
    if(bias >= 1.f)
    {
        bBiasIncrement = false;
    }
    else if(bias <= COLOR_GB_BIAS_DEF)
    {
        bBiasIncrement = true;
    }
    //calculate new bias for frame
    bias += (bBiasIncrement) ? (GB_BIAS_CYCLE_SPEED) : (-GB_BIAS_CYCLE_SPEED);
    
    GLCall(const GLint uColor = glGetUniformLocation(_shaderProgram, "u_Color"));
    if(uColor != -1)
        GLCall(glUniform4f(uColor, used_color.x, used_color.y, used_color.z, 1.0f));
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
    PRINT_ERRS;
    
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
    PRINT_ERRS;
    
    //creating vertex buffer:
    float vertex1Verts[TRIANGLE_VERTBUF_SIZE] =     //array of vertices
        {
            -0.5f, -0.5,
             0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f
        };
    
    unsigned int vertex1Buffer; //buffer ID for triangle
    glGenBuffers(1, &vertex1Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex1Buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * TRIANGLE_VERTBUF_SIZE, vertex1Verts, GL_STATIC_DRAW);
    PRINT_ERRS;
    
    //creating layout:
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    PRINT_ERRS;
    
    //creating index buffer:
    unsigned int rect1Indices[3 * 2] =
        {
            0, 1, 2,
            0, 2, 3
        };
    unsigned int index1Buffer; //buffer ID for vertex indices
    glGenBuffers(1, &index1Buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index1Buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 2, rect1Indices, GL_STATIC_DRAW);
    PRINT_ERRS;
    
    //generating shader
    //first, we extract source code:
    std::string vertex_shader;
    std::string fragment_shader;
    shaderUtils::GetSrcCode("shaders/stdVS.vs", vertex_shader);
    shaderUtils::GetSrcCode("shaders/stdFS.fs", fragment_shader);
    PRINT_ERRS;

    //now we create shader program
    const GLuint shader_program = shaderUtils::CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader_program);
    PRINT_ERRS;

    GLCall(const GLint uColor = glGetUniformLocation(shader_program, "u_Color"));
    if(uColor != -1)
        GLCall(glUniform4f(uColor, 1.f, 0.2f, 0.1f, 1.0f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        cycleRectColor(shader_program);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glDeleteProgram(shader_program);
    glfwTerminate();

    return 0;
}
