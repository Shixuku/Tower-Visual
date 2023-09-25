#pragma once
#include "Instance.h"
#include "NominalHeight.h"
#include<QString.h>
class TowerList :
    public Instance
{
public:
    QString m_Name;
    Manage_Entity<Part_Base> TP_Part;
    Manage_Entity<NominalHeight>TP_Height;
};

