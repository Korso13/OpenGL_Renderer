#pragma once
#include "pch.h"
#include "Defines.h"
#include "MainMenuItem.h"

class Quad;

class NewBatchPipelineTest : public MainMenuItem
{
public:
    NewBatchPipelineTest();
    ~NewBatchPipelineTest();
    
    void onUpdate(float dt) override;
    void onRender() override;

private:

    SPTR<Node> m_root = nullptr;
    SPTR<Quad> m_quad1 = nullptr;
    SPTR<Quad> m_quad2 = nullptr;
};
