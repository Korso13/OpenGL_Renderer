#include <algorithm>
#include <pch.h>
#include <Defines.h>
#include <cstdio>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>

#include "ImGUI/imgui_impl_opengl3.h"
#include "ManagingClasses/Public/Renderer.h"
#include "ManagingClasses/Public/ResourceLibrary.h"
#include "Menu/Public/ClearColorTest.h"
#include "Menu/Public/NewBatchPipelineTest.h"
#include "source/Utilities/Public/GLErrorCatcher.h"

enum class MMSelector
{
    NONE,
    CLEAR_COLOR,
    NEW_PIPELINE
};

static MMSelector RenderSelector = MMSelector::NONE;
static ClearColorTest* ClearColor = nullptr;
static NewBatchPipelineTest* NewRender = nullptr;

static void SwitchTesters(MMSelector _tester)
{
    switch (RenderSelector)
    {
    case MMSelector::NONE:
        break;
    case MMSelector::CLEAR_COLOR:
        delete ClearColor;
        break;
    case MMSelector::NEW_PIPELINE:
        delete NewRender;
        break;
    }

    RenderSelector = _tester;
    
    switch (_tester)
    {
    case MMSelector::NONE:
        break;
    case MMSelector::CLEAR_COLOR:
        ClearColor = new ClearColorTest();
        break;
    case MMSelector::NEW_PIPELINE:
        NewRender = new NewBatchPipelineTest();
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
    
    //NOTE: used in debugging
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) //returning false for some reason :(
    {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(GLDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(START_RESOLUTION, "OpenGL Test", NULL, NULL);
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
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
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
        SwitchTesters(MMSelector::CLEAR_COLOR);
    });
    DEBUG_UI->addMainMenuItem("New Batch Render Pipeline tester",
    []()
    {
        SwitchTesters(MMSelector::NEW_PIPELINE);
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
        case MMSelector::NEW_PIPELINE:
            if(NewRender)NewRender->onRender();
            break;
        case MMSelector::NONE:
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
