#include <algorithm>
#include <PCH.h>
#include <Defines.h>
#include <cstdio>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include "ImGUI/imgui_impl_opengl3.h"
#include "Menu/ClearColorTest.h"
#include "Menu/TextureRenderTest.h"
#include "source/Utilities/GLErrorCatcher.h"

enum class MMSelector
{
    TEXTURE_RENDER,
    CLEAR_COLOR
};

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
    
    //Creating ImGUI debugger
    DEBUG_UI->initImGUI(window);

    TextureRenderTest* TextureRender = new TextureRenderTest();
    ClearColorTest* ClearColor = new ClearColorTest();

    //setting up main menu buttons-selectors
   MMSelector RenderSelector;
    DEBUG_UI->addMainMenuItem("Texture render tester", [&RenderSelector]()
    {
        RenderSelector = MMSelector::TEXTURE_RENDER;
        
    });
    DEBUG_UI->addMainMenuItem("Clear color tester", [&RenderSelector]()
    {
        RenderSelector = MMSelector::CLEAR_COLOR;
    });
//============================================================================================================
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        switch (RenderSelector)
        {
        case MMSelector::TEXTURE_RENDER:
            TextureRender->onRender();
            break;
        case MMSelector::CLEAR_COLOR:
            ClearColor->onRender();
            break;
        default:
            break;
        }
        
        //Rendering ImGUI debugger
        DEBUG_UI->imGUI_Render();
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();

    return 0;
}
