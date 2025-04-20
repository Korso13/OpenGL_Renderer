#pragma once
#include "MainMenuItem.h"

class ClearColorTest : public MainMenuItem
{
public:
    ClearColorTest();
    ~ClearColorTest();

    void onUpdate(float dt) override;
    void onRender() override;

private:
    float m_color[4];
};
