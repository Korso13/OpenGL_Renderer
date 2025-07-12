#include <algorithm>
#include <pch.h>
#include <Defines.h>
#include <cstdio>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include "ImGUI/imgui_impl_opengl3.h"
#include "ManagingClasses/Public/Renderer.h"
#include "ManagingClasses/Public/ResourceLibrary.h"
#include "Menu/Public/BatchRenderTest.h"
#include "Menu/Public/ClearColorTest.h"
#include "source/Utilities/Public/GLErrorCatcher.h"

enum class MMSelector
{
    NONE,
    CLEAR_COLOR,
    BATCH_RENDER
};

static MMSelector RenderSelector = MMSelector::NONE;
static ClearColorTest* ClearColor = nullptr;
static BatchRenderTest* BatchRender = nullptr;

static void switchTesters(MMSelector tester)
{
    switch (RenderSelector)
    {
    case MMSelector::NONE:
        break;
    case MMSelector::CLEAR_COLOR:
        delete ClearColor;
        break;
    case MMSelector::BATCH_RENDER:
        delete BatchRender;
        break;
    default:
        break;
    }

    RenderSelector = tester;
    
    switch (tester)
    {
    case MMSelector::NONE:
        break;
    case MMSelector::CLEAR_COLOR:
        ClearColor = new ClearColorTest();
        break;
    case MMSelector::BATCH_RENDER:
        BatchRender = new BatchRenderTest();
        break;
    default: 
        break;
    }
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

    //Initializing resources
    ResourceLibrary::get().init();
    RENDERER;
    
    //Creating ImGUI debugger
    DEBUG_UI->initImGUI(window);

    //setting up main menu buttons-selectors
    DEBUG_UI->addMainMenuItem("Clear color tester",
    []()
    {
        switchTesters(MMSelector::CLEAR_COLOR);
    });
    DEBUG_UI->addMainMenuItem("Batch render tester",
    []()
    {
        switchTesters(MMSelector::BATCH_RENDER);
    });
//============================================================================================================
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        switch (RenderSelector)
        {
        case MMSelector::CLEAR_COLOR:
            if(ClearColor)ClearColor->onRender();
            break;
        case MMSelector::BATCH_RENDER:
            if(BatchRender)BatchRender->onRender();
            break;
        case MMSelector::NONE:
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
