#pragma once

class MainMenuItem
{
public:
    MainMenuItem() = default;
    virtual ~MainMenuItem(){}

    virtual void onUpdate(float dt) = 0;
    virtual void onRender() = 0;
    
};
