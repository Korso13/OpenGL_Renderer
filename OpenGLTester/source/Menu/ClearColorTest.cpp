#include "ClearColorTest.h"
#include <PCH.h>

#include "Defines.h"
#include "Utilities/GLErrorCatcher.h"

ClearColorTest::ClearColorTest()
    : m_color{0.2f, 0.3f, 0.8f, 1.f}
{
    DEBUG_UI->addFolder("Clear color test");
    DEBUG_UI->addDebugValueToFolder(
        "Clear color test",
        DebugDataType::COLOR,
        "Clear color",
        &m_color[0],
        4,
        ImGUI_ToolType::COLORPICKER
        );
}

ClearColorTest::~ClearColorTest()
{
}

void ClearColorTest::onUpdate(float dt)
{
}

void ClearColorTest::onRender()
{
    GLCall(glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
