#include "TowerData_Insulator.h"

double TowerData_Insulator::Get_y(double x)
{
    double k, b, y;
    k = (m_node2->y - m_node1->y) / (m_node2->x - m_node1->x);
    b = m_node1->y - k * m_node1->x;
    y = k * x + b;
    return y;
}
