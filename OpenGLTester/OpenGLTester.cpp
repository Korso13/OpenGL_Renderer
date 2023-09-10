#include <PCH.h>
#include <Defines.h>
#include <cstdio>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include "ImGUI/imgui_impl_opengl3.h"

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
            -50.0, -50.0,   0.0, 0.0, //one record. 2 float coordinates of vertice, 2 float UV coordinates
             50.0, -50.0,   1.0, 0.0,
             50.0,  50.0,   1.0, 1.0,
            -50.0,  50.0,   0.0, 1.0
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
    glm::vec3 viewTranslation(0.f, 0.f, 0.f);
    view = glm::translate(view, viewTranslation);

    glm::mat4 Logo1Trans = glm::mat4(1.f);
    glm::mat4 Logo2Trans = Logo1Trans;
    Logo1Trans = glm::translate(Logo1Trans, glm::vec3(0.f, 0.f, 0.f));
    
    glm::mat4 MVP =  projection * view * Logo1Trans;
    
    //generating shader
    ShaderMachine::get()->setShader(ShaderType::TEXTURE_STD);

    Texture* texture1 = new Texture("resources/textures/logo.png");
    texture1->bind(0);
    ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_Texture", 0);
    ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_MVP", MVP);
    
    //Creating Renderer
    Renderer* renderer = new Renderer;

    //Values for debugging UI
    bool show_2nd_logo = false;
    glm::vec3 Logo1Pos(60.f, 60.f, 0.f);
    glm::vec3 Logo2Pos(60.f, 60.f, 0.f);
    
    //Creating ImGUI debugger
    DEBUG_UI->initImGUI(window);
    DEBUG_UI->addFolder("Main");

    //adding values to debug
    DEBUG_UI->addDebugValueToFolder<bool>(
        "Main",
        DebugDataType::BOOL,
        "Show 2nd logo",
        &show_2nd_logo,
        1,
        ImGUI_ToolType::CHECKBOX);

    DEBUG_UI->addDebugValueToFolder<float>(
        "Main",
        DebugDataType::FLOAT,
        "Camera position",
        &viewTranslation.x,
        2,
        ImGUI_ToolType::SLIDER,
        -1000.f,
        1000.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Main",
        DebugDataType::FLOAT,
        "Logo 1 position",
        &Logo1Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        1000.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Main",
        DebugDataType::FLOAT,
        "Logo 2 position",
        &Logo2Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        1000.f);

//============================================================================================================
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer->clear();

        //Rendering ImGUI debugger
        DEBUG_UI->imGUI_Render();

        //setting first logo transform and calculating MVP
        Logo1Trans = glm::translate(glm::mat4(1.f), Logo1Pos);
        view = glm::translate(glm::mat4(1.f), viewTranslation);
        glm::mat4 MVP1 =  projection * view * Logo1Trans;
        ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_MVP", MVP1);

        //making a draw call
        texture1->bind(0); //binding texture
        renderer->draw(vao1, ShaderType::TEXTURE_STD);

        if(show_2nd_logo)
        {
            //setting second logo transform and calculating MVP
            Logo2Trans = glm::translate(glm::mat4(1.f), Logo2Pos);
            view = glm::translate(glm::mat4(1.f), viewTranslation);
            glm::mat4 MVP2 =  projection * view * Logo2Trans;
            ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_MVP", MVP2);

            //making a draw call
            renderer->draw(vao1, ShaderType::TEXTURE_STD);
        }
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    delete vao1;
    glfwTerminate();

    return 0;
}
