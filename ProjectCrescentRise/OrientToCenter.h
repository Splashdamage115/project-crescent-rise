#pragma once
#include "ScriptObject.h"

class OrientToCenter : public ScriptObject
{
public:
    virtual void Update() override;
private:
    bool m_oriented = false;
};
