// DynamicObject.cpp: implementation of the CDynamicObject class.
//
//////////////////////////////////////////////////////////////////////

#include "DynamicObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDynamicObject::CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwRegisterTime, DWORD dwLastTime, int iV1, int iV2, int iV3)
{
	m_sOwner         = sOwner;
	m_cOwnerType     = cOwnerType;

	ZeroMemory(cOwnerName, sizeof(cOwnerName));

	m_sType          = sType;
	
	m_cMapIndex      = cMapIndex;
	m_sX             = sX;
	m_sY             = sY;
	
	m_dwRegisterTime = dwRegisterTime;
	m_dwLastTime     = dwLastTime;

	m_iCount         = 0;
	m_iV1            = iV1;
	m_iV2 = iV2;
	m_iV3 = iV3;
}										   

CDynamicObject::~CDynamicObject()
{

}
