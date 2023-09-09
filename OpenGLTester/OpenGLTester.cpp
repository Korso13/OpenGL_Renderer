#include <PCH.h>
#include <Defines.h>
#include <cstdio>

#include "BaseClasses/Texture.h"
#include "BaseClasses/VertexAttributes.h"
#include "ManagingClasses/Renderer.h"
#include "source/ManagingClasses/ShaderMachine.h"
#include "source/Utilities/GLErrorCatcher.h"
#include "source/BaseClasses/IndexBuffer.h"
#include "source/BaseClasses/VertexBuffer.h"
#include "source/ManagingClasses/VertexAO.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

#define RECT_VERTBUF_SIZE 8
#define TRIANGLE_VERTBUF_SIZE 6

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
    window = glfwCreateWindow(960, 540, "OpenGL Test", NULL, NULL);
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

    //blend function
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    
//============================================================================================================
    
    //creating vertex buffer:
    float rect1_verts[RECT_VERTBUF_SIZE * 2] =     //array of vertices
        {
            200.0, 200.0,   0.0, 0.0, //one record. 2 float coordinates of vertice, 2 float UV coordinates
            300.0, 200.0,   1.0, 0.0,
            300.0, 300.0,   1.0, 1.0,
            200.0, 300.0,   0.0, 1.0
        };
    
    //creating index buffer:
    unsigned int rect1Indices[3 * 2] =
        {
            0, 1, 2,
            0, 2, 3
        };

    //creating vertex and index buffer objects
    VertexBuffer vBuffer1(&rect1_verts, RECT_VERTBUF_SIZE * 2 * sizeof(float));
    IndexBuffer iBuffer1(rect1Indices, 6);

    //creating vertex attributes\layout
    VertexAttributes VAttributes1;
    VAttributes1.addAttribute({GL_FLOAT, 2, GL_FALSE}); //2 float coordinates
    VAttributes1.addAttribute({GL_FLOAT, 2, GL_FALSE}); //2 float UV coordinates

    //Packing buffers and attributes into vertex array abstraction object
    VertexAO* vao1 = new VertexAO();
    vao1->addBuffer(&vBuffer1, &iBuffer1, &VAttributes1);

    //making MVP matrix
    glm::mat4 projection = glm::ortho(0.f, 960.f, 0.0f, 540.0f, -1.f, 1.f);

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(-200.f, 0.f, 0.f));

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(400.f, 0.f, 0.f));
    
    glm::mat4 MVP =  projection * view * model;
    
    //generating shader
    ShaderMachine::get()->setShader(ShaderType::TEXTURE_STD);

    Texture* texture1 = new Texture("resources/textures/logo.png");
    texture1->bind(0);
    ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_Texture", 0);
    ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_MVP", MVP);
    
    //Creating Renderer
    Renderer* renderer = new Renderer;
    
//============================================================================================================
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer->clear();

        //setting rectangle color
        //cycleRectColor(ShaderMachine::get()->getShader(ShaderType::SIMPLE));
        texture1->bind(0);
        renderer->draw(vao1, ShaderType::TEXTURE_STD);
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    
    delete vao1;
    glfwTerminate();

    return 0;
}
