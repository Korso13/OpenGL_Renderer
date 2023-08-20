#include <cstdio>
#include <iostream>

#include "../external/include/GLEW/glew.h"
#include "../external/include/GLFW/glfw3.h"

#define TRIANGLE_VERTBUF_SIZE 6

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

    unsigned int vertex1Buffer = 1; //buffer ID for triangle
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
    
    glGenBuffers(1, &vertex1Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex1Buffer);
    glBufferData(vertex1Buffer, sizeof(float) * TRIANGLE_VERTBUF_SIZE, &vertex1Verts, GL_DYNAMIC_DRAW);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_FLOAT, &vertex1Indices);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
