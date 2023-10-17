#pragma once
#include "Base.h"
class HangPoint :
    public Base
{
public:

    QString StringClass;
    QString WireLoge;
    int NodeId;
    HangPoint(int id, QString StringClass, QString WireLoge, int iNodeId);
    HangPoint();
};

