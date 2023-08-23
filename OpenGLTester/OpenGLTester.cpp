#include <cstdio>
#include "../external/include/GLEW/glew.h"
#include "../external/include/GLFW/glfw3.h"
#include "source/Utilities/ShaderUtilities.h"

#define TRIANGLE_VERTBUF_SIZE 8

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

    //creating layout:
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

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
    
    //generating shader
    //first, we extract source code:
    std::string vertex_shader;
    std::string fragment_shader;
    shaderUtils::GetSrcCode("shaders/stdVS.vs", vertex_shader);
    shaderUtils::GetSrcCode("shaders/stdFS.fs", fragment_shader);

    //now we create shader program
    const GLuint shader_program = shaderUtils::CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader_program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glfwTerminate();

    return 0;
}
