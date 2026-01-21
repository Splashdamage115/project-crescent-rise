#pragma once
#include "ScriptObject.h"

class OrientToSurface : public ScriptObject
{
public:
    virtual void Update() override;
private:
    bool m_oriented = false;
};
