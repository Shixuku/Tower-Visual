#include "TowerData_Insulator.h"

double TowerData_Insulator::Get_y(double x)
{
    double k, b, y;
    k = (m_node2->y - m_node1->y) / (m_node2->x - m_node1->x);
    b = m_node1->y - k * m_node1->x;
    y = k * x + b;
    return y;
}

double TowerData_Insulator::Get_y_I(double x)
{//分四条斜率
    double m_pt1X, m_pt1Y, m_pt2X, m_pt2Y;

    if (m_node->x > 0)
    {
        m_pt1X = m_W1 / 2;       m_pt2X = m_L2;
        if (m_node->y > 0)
        {
            m_pt1Y = m_W1 / 2;   m_pt2Y = m_W2 / 2;
        }
        else if (abs(m_node->y - 0) < 1e-1)
        {
            m_pt1Y = 0;             m_pt2Y = 0;
        }
        else if (m_node->y < 0)
        {
            m_pt1Y = -m_W1 / 2;   m_pt2Y = -m_W2 / 2;
        }
    }
    if (m_node->x < 0)
    {
        m_pt1X = -m_W1 / 2;       m_pt2X = -m_L2;
        if (m_node->y > 0)
        {
            m_pt1Y = m_W1 / 2;   m_pt2Y = m_W2 / 2;
        }
        else if (abs(m_node->y - 0 )< 1e-1)
        {
            m_pt1Y = 0;             m_pt2Y = 0;
        }
        else if (m_node->y < 0)
        {
            m_pt1Y = -m_W1 / 2;   m_pt2Y = -m_W2 / 2;
        }
    }

    double k, b, y;
    k = (m_pt2Y - m_pt1Y) / (m_pt2X - m_pt1X);
    b = m_pt1Y - k * m_pt1X;
    y = k * x + b;
    return y;

}


