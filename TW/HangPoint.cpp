#include "HangPoint.h"

HangPoint::HangPoint(int id, QString iStringClass, QString iWireLoge, int iNodeId)
{
	m_id = id;
	StringClass = iStringClass;
	WireLoge = iWireLoge;
	NodeId = iNodeId;
}

HangPoint::HangPoint()
{
}
