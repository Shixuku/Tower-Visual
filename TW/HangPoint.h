#pragma once
#include "Base.h"
class HangPoint :
    public Base
{
public:
    QString Wire;
    QString StringClass;
    QString WireLoge;
    int NodeId;
    HangPoint(int id, QString iWire, QString StringClass, QString WireLoge, int iNodeId);
};

