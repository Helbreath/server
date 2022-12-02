// XSocket.h: interface for the XSocket class.
//
//////////////////////////////////////////////////////////////////////

// ����� Ȯ���ڵ�� 1����Ʈ, ũ��� WORD�� ����Ǿ���. �� ����� �� 3����Ʈ 


#if !defined(AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _WINSOCK2API_
//#define  FD_SETSIZE 2000
#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "DebugDialog.h"
#include <iostream>
#include <strstream>
#include <string>
#include "GlobalDef.h"

class XSocket  
{
public:
	int iGetPeerAddress(char * pAddrString);
	char * pGetRcvDataPointer(DWORD * pMsgSize, char * pKey = 0);
	SOCKET iGetSocket();
	bool bAccept(class XSocket * pXSock, unsigned int uiMsg);
	bool bListen(char * pAddr, int iPort, unsigned int uiMsg);
	int iSendMsg(char * cData, DWORD dwSize, char cKey = 0);
	bool bConnect(char * pAddr, int iPort, unsigned int uiMsg);
	int  iOnSocketEvent(WPARAM wParam, LPARAM lParam);
	bool bInitBufferSize(DWORD dwBufferSize);
	XSocket(HWND hWnd, int iBlockLimit);
	virtual ~XSocket();

	int  m_WSAErr;
	bool m_bIsAvailable;
	bool IsSocketInUse;

private:
	void _CloseConn();
	
	int _iSendUnsentData();
	int _iRegisterUnsentData(char * cData, int iSize);
	int _iSend(char * cData, int iSize, bool bSaveFlag);
	int _iSend_ForInternalUse(char * cData, int iSize);
	int _iOnRead();
	
	char   m_cType;
	char * m_pRcvBuffer;
	char * m_pSndBuffer;
	DWORD  m_dwBufferSize;
	
	SOCKET m_Sock;
	char   m_cStatus;
	DWORD  m_dwReadSize;
	DWORD  m_dwTotalReadSize;
	char   m_pAddr[30];
	int    m_iPortNum;

	char * m_pUnsentDataList[DEF_XSOCKBLOCKLIMIT];
	int    m_iUnsentDataSize[DEF_XSOCKBLOCKLIMIT];
	short  m_sHead, m_sTail;

	unsigned int m_uiMsg;
	HWND         m_hWnd;

	int			 m_iBlockLimit;

	bool m_bWriteMessage;
};


bool _InitWinsock();
void _TermWinsock();
extern void SafeCopy(char *c1, char *c2, DWORD lenght);

#endif // !defined(AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_)
