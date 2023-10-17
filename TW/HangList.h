#pragma once
#include"Base.h"
#include<vector>

class HangList  : public Base
{
public:

    QString StringClass;
    QString WireLoge;
    std::vector<int>NodeId;
    std::vector<double>Angle;
    double length=0;
    int LineSegment;
    QString Material;
    QString Section;
    HangList(int id, QString StringClass, QString WireLoge);
};

