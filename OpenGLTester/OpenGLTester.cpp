#include <PCH.h>
#include <Defines.h>
#include <cstdio>

#include "BaseClasses/VertexAttributes.h"
#include "source/BaseClasses/Shader.h"
#include "source/Utilities/GLErrorCatcher.h"
#include "source/BaseClasses/IndexBuffer.h"
#include "source/BaseClasses/VertexBuffer.h"
#include "source/BaseClasses/VertexAO.h"

#define RECT_VERTBUF_SIZE 8
#define TRIANGLE_VERTBUF_SIZE 6

#define COLOR_CYCLE_SPEED 0.01f
#define GB_BIAS_CYCLE_SPEED 0.005f
#define COLOR_MIN_VALUE 0.2f
#define COLOR_GB_BIAS_DEF 0.2f
#define COLOR_POOL_SIZE 2.0f

/**
 * global var for cycling color
 */
static vec3 used_color{0.f, 0.f, 0.f};
static float bias = COLOR_GB_BIAS_DEF;
static bool bBiasIncrement = true;
static bool bRedIncrement = true;

void cycleRectColor(Shader* _shaderProgram)
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

    vec4 color{used_color.x, used_color.y, used_color.z, 1.0f};
    _shaderProgram->setUniform("u_Color", color);
}

int main(void)
{
    //Init thingies

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    GLCall(glfwSwapInterval(1));
    
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

//============================================================================================================
    
    //creating vertex buffer:
    float rect1_verts[RECT_VERTBUF_SIZE] =     //array of vertices
        {
            -0.5f, -0.5,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };

    VertexBuffer vBuffer1(&rect1_verts, RECT_VERTBUF_SIZE * sizeof(float));
    
    //creating index buffer:
    unsigned int rect1Indices[3 * 2] =
        {
            0, 1, 2,
            0, 2, 3
        };

    IndexBuffer iBuffer1(rect1Indices, 6);

    //creating vertex attributes\layout
    VertexAttributes VAttributes1;
    VAttributes1.addAttribute({GL_FLOAT, 2, GL_FALSE});

    //Packing buffers and attributes into vertex array abstraction object
    VertexAO* vao1 = new VertexAO();
    vao1->addBuffer(vBuffer1, iBuffer1, VAttributes1);

    //resetting bindings
    vao1->unbind();
    
    //generating shader
    //first, we extract source code:
    Shader* shader1 = new Shader("shaders/stdVS.vs", "shaders/stdFS.fs");
    shader1->Bind();
    
//============================================================================================================
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        //binding back buffers-vertex array object
        vao1->bind();

        cycleRectColor(shader1);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    delete shader1;
    delete vao1;
    glfwTerminate();

    return 0;
}
