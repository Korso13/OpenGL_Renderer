#pragma once
#include "Defines.h"

class MeshesFactory
{
public:
    static MeshesFactory& get()
    {
        static MeshesFactory instance;
        return instance;
    }
    
private:

    MeshesFactory() = default;
    ~MeshesFactory() = default;

private:
};
