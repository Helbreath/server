// XSocket.cpp: implementation of the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "XSocket.h"
#include "Client.h"
#include "Game.h"
#include "winmain.h"

#pragma warning (disable : 4996 4018)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSocket::XSocket(HWND hWnd, int iBlockLimit)
{
 int i;
	
	m_cType       = 0;
	m_pRcvBuffer  = 0;
	m_pSndBuffer  = 0;
	m_Sock        = INVALID_SOCKET;
	m_dwBufferSize = 0;

	m_cStatus   = DEF_XSOCKSTATUS_READINGHEADER;      
	m_dwReadSize = 3;    
	m_dwTotalReadSize = 0;

	for (i = 0; i < DEF_XSOCKBLOCKLIMIT; i++) {
		m_iUnsentDataSize[i] = 0;
		m_pUnsentDataList[i] = 0;
	}
	
	m_sHead = 0;
	m_sTail = 0;

	m_WSAErr = 0;

	m_hWnd = hWnd;
	m_bIsAvailable = false;

	m_iBlockLimit = iBlockLimit;
	IsSocketInUse = false;

	m_bWriteMessage = false;
}

XSocket::~XSocket()
{
 int i;
	
	if (m_pRcvBuffer != 0) delete m_pRcvBuffer;
	if (m_pSndBuffer != 0) delete m_pSndBuffer;

	for (i = 0; i < DEF_XSOCKBLOCKLIMIT; i++)
		if (m_pUnsentDataList[i] != 0) delete m_pUnsentDataList[i];

	// ¼ÒÄÏÀ» ¸¶Àú ÀÐ°í ´Ý´Â´Ù.
	_CloseConn(); 
}

bool XSocket::bInitBufferSize(DWORD dwBufferSize)
{
	if (m_pRcvBuffer != 0) delete m_pRcvBuffer;
	if (m_pSndBuffer != 0) delete m_pSndBuffer;

	m_pRcvBuffer = new char[dwBufferSize+8];
	if (m_pRcvBuffer == 0) return false;
	
	m_pSndBuffer = new char[dwBufferSize+8];
	if (m_pSndBuffer == 0) return false;

	m_dwBufferSize = dwBufferSize;

	return true;
}

int XSocket::iOnSocketEvent(WPARAM wParam, LPARAM lParam)
{
 int WSAEvent;

	// ¸®½º´× ¼ÒÄÏÀÇ ÀÌº¥Æ®´Â Ã³¸®ÇÒ ¼ö ¾ø´Ù.
	if (m_cType != DEF_XSOCK_NORMALSOCK) return DEF_XSOCKEVENT_SOCKETMISMATCH;
	// ÃÊ±âÈ­ µÇÁö ¾Ê¾Æ¼­ Ã³¸®ÇÒ ¼ö ¾ø´Ù.
	if (m_cType == 0) return DEF_XSOCKEVENT_NOTINITIALIZED;

	if ((SOCKET)wParam != m_Sock) return DEF_XSOCKEVENT_SOCKETMISMATCH;
	WSAEvent = WSAGETSELECTEVENT(lParam);

	switch (WSAEvent) {
	case FD_CONNECT:
		if (WSAGETSELECTERROR(lParam) != 0) { 
			// ÀÌ ¼ÒÄÏÀÇ Á¢¼ÓÀÌ ½ÇÆÐÇßÀ¸¹Ç·Î ÀçÁ¢¼ÓÀ» ½ÃµµÇÑ´Ù.
			if (bConnect(m_pAddr, m_iPortNum, m_uiMsg) == false) return DEF_XSOCKEVENT_SOCKETERROR;
				
			return DEF_XSOCKEVENT_RETRYINGCONNECTION;
		}
		else {
			m_bIsAvailable = true;
			return DEF_XSOCKEVENT_CONNECTIONESTABLISH;
		}
		break;

	case FD_READ:
		if (WSAGETSELECTERROR(lParam) != 0) { 
			// ÀÐ´Â µµÁß ¿¡·¯°¡ ¹ß»ýÇß´Ù.
			m_WSAErr = WSAGETSELECTERROR(lParam);
			return DEF_XSOCKEVENT_SOCKETERROR;
		}
		else return _iOnRead();
		break;
	
	case FD_WRITE:
		m_bWriteMessage = true; // changed
		return _iSendUnsentData();
		break;

	case FD_CLOSE:
		m_cType = DEF_XSOCK_SHUTDOWNEDSOCK; // ¼ÒÄÏÀÌ ´ÝÇûÀ¸¹Ç·Î ÀÌ ¼ÒÄÏÀº »ç¿ë ºÒ°¡´É. 
		return DEF_XSOCKEVENT_SOCKETCLOSED;
		break;
	}

	return DEF_XSOCKEVENT_UNKNOWN;
}

bool XSocket::bConnect(char * pAddr, int iPort, unsigned int uiMsg)
{
 SOCKADDR_IN	 saTemp;
 u_long          arg;
 int             iRet;
 DWORD			 dwOpt;

	// ¸®½º´× ¼ÒÄÏÀ¸·Î ÃÊ±âÈ­µÈ Å¬·¡½º´Â ÀÌ ÇÔ¼ö¸¦ »ç¿ëÇÒ ¼ö ¾ø´Ù.
	if (m_cType == DEF_XSOCK_LISTENSOCK) return false;
	if (m_Sock  != INVALID_SOCKET) closesocket(m_Sock);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET) 
		return false;
	
	// ¼ÒÄÏÀ» ³íºí·ÏÅ· ¸ðµå·Î 
	arg = 1;
	ioctlsocket(m_Sock, FIONBIO, &arg);
	
	// ÁÖ¼Ò¸¦ ¹ÙÀÎµåÇÑ´Ù.
	memset(&saTemp,0,sizeof(saTemp));
	saTemp.sin_family = AF_INET;
	saTemp.sin_addr.s_addr = inet_addr(pAddr);
	saTemp.sin_port = htons(iPort);
	
	iRet = connect(m_Sock, (struct sockaddr *) &saTemp, sizeof(saTemp));
	if (iRet == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			m_WSAErr = WSAGetLastError();
			return false;
		}
	}

	WSAAsyncSelect(m_Sock, m_hWnd, uiMsg, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	// ¼ÒÄÏ ¿É¼ÇÀ» Á¶Á¤ÇÑ´Ù. 
	dwOpt = 8192*5;
	setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));


	strcpy_s(m_pAddr, pAddr);
	m_iPortNum = iPort;

	m_uiMsg = uiMsg;
	m_cType = DEF_XSOCK_NORMALSOCK;

	return true;
}

int XSocket::_iOnRead()
{
 int iRet, WSAErr;
 WORD  * wp;	

	if (m_cStatus == DEF_XSOCKSTATUS_READINGHEADER) {
		
		iRet = recv(m_Sock, (char *)(m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);	
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			}
			else return DEF_XSOCKEVENT_BLOCK;
		}
		else  if (iRet == 0) {
			// ¼ÒÄÏÀÌ ´ÝÇû´Ù. 
			m_cType = DEF_XSOCK_SHUTDOWNEDSOCK;
			return DEF_XSOCKEVENT_SOCKETCLOSED;
		}
		
		m_dwReadSize      -= iRet;
		m_dwTotalReadSize += iRet;
		
		if (m_dwReadSize == 0) {
			// Çì´õ¸¦ ´Ù ÀÐ¾ú´Ù. 
			m_cStatus = DEF_XSOCKSTATUS_READINGBODY;
			// ÀÐ¾î¾ß ÇÒ ¸öÃ¼ »çÀÌÁî¸¦ °è»êÇÑ´Ù.
			wp = (WORD *)(m_pRcvBuffer + 1);
			m_dwReadSize = (int)(*wp - 3); // Çì´õ »çÀÌÁî´Â Æ÷ÇÔÇÏÁö ¾Ê´Â´Ù. 
			
			if (m_dwReadSize == 0) {
				// ¸öÃ¼ »çÀÌÁî°¡ 0ÀÌ¸é ¸öÃ¼ºÎºÐÀ» ÀÐÀ» ÇÊ¿ä°¡ ¾øÀ¸¹Ç·Î 
				m_cStatus        = DEF_XSOCKSTATUS_READINGHEADER;
				m_dwReadSize      = 3;
				m_dwTotalReadSize = 0;
				return DEF_XSOCKEVENT_READCOMPLETE;
			}
			else  if (m_dwReadSize > m_dwBufferSize) {
				m_cStatus        = DEF_XSOCKSTATUS_READINGHEADER;
				m_dwReadSize      = 3;
				m_dwTotalReadSize = 0;
				return DEF_XSOCKEVENT_MSGSIZETOOLARGE;
			}
		}
		return DEF_XSOCKEVENT_ONREAD;
	}
	else if (m_cStatus == DEF_XSOCKSTATUS_READINGBODY) {
		
		iRet = recv(m_Sock, (char *)(m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			}
			else return DEF_XSOCKEVENT_BLOCK;
		}
		else  if (iRet == 0) {
			// ¼ÒÄÏÀÌ ´ÝÇû´Ù. 
			m_cType = DEF_XSOCK_SHUTDOWNEDSOCK;
			return DEF_XSOCKEVENT_SOCKETCLOSED;
		}

		m_dwReadSize      -= iRet;
		m_dwTotalReadSize += iRet;
		
		if (m_dwReadSize == 0) {
			// ¸öÃ¼¸¦ ´Ù ÀÐ¾ú´Ù. ´ÙÀ½¹ø ÀÌº¥Æ®¸¦ À§ÇØ »óÅÂ¸¦ ¹Ù²Û´Ù. 
			m_cStatus        = DEF_XSOCKSTATUS_READINGHEADER;
			m_dwReadSize      = 3;
			m_dwTotalReadSize = 0;
		}
		else return DEF_XSOCKEVENT_ONREAD;
	}

	// ¸Þ½ÃÁö¸¦ ¸ðµÎ ÀÐ¾ú´Ù. ÀÌ ¸Þ½ÃÁö¸¦ ¹ÞÀ¸¸é Å¬·¡½º ¹öÆÛÀÇ ³»¿ëÀ» ÀÐ¾î Áï°¢ Ã³¸®ÇØ¾ß ÇÑ´Ù.
	return DEF_XSOCKEVENT_READCOMPLETE;
}



int XSocket::_iSend(char * cData, int iSize, bool bSaveFlag)
{
 int  iOutLen, iRet, WSAErr;

	if (m_pUnsentDataList[m_sHead] != 0) {
		if (bSaveFlag == true) {
			// ¸¸¾à ´ë±â¿­¿¡ µ¥ÀÌÅÍ°¡ ³²¾Æ ÀÖ°í ²À º¸³»¾ß ÇÏ´Â µ¥ÀÌÅÍ¶ó¸é 
			// ¸Þ½ÃÁöÀÇ ¼ø¼­¸¦ ¸ÂÃß±â À§ÇØ ¹«Á¶°Ç ´ë±â¿­¿¡ ÀúÀåÇØ¾ß ÇÑ´Ù. 
			iRet = _iRegisterUnsentData(cData, iSize);
			switch (iRet) {
			case -1:
				// ÇÒ´çÇÒ ¸Þ¸ð¸®°¡ ¾ø´Ù. ÀÌ°æ¿ì´Â º¹±¸ÇÒ ¹æ¹ýÀÌ ¾ø´Ù.
				return DEF_XSOCKEVENT_CRITICALERROR;
			case 0:
				// Å¥°¡ °¡µæÃ¡´Ù. ÀÌ ¼ÒÄÏ Å¬·¡½º´Â »èÁ¦µÇ¾î¾ß¸¸ ÇÑ´Ù.
				return DEF_XSOCKEVENT_QUENEFULL;
			case 1:
				// Á¤»óÀûÀ¸·Î µ¥ÀÌÅÍ¸¦ µî·ÏÇß´Ù.
				break;
			}
			return DEF_XSOCKEVENT_BLOCK;
		}
		else return 0;
	}

	iOutLen = 0;
	while (iOutLen < iSize) {
		
		iRet = send(m_Sock, (cData + iOutLen), iSize - iOutLen, 0); 
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				// ¼ÒÄÏ¿¡ ¿¡·¯°¡ ¹ß»ýÇß´Ù.
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			}
			else {
				// ºí·°»óÅÂÀÌ¸é ´õÀÌ»ó º¸³¾ ¼ö ¾øÀ¸¹Ç·Î ³²¾ÆÀÖ´Â µ¥ÀÌÅÍ¸¦ ¸®½ºÆ®¿¡ µî·ÏÇÏ°í ¸®ÅÏ 
				if (bSaveFlag == true) {
					iRet = _iRegisterUnsentData((cData + iOutLen), (iSize - iOutLen));
					switch (iRet) {
					case -1:
						// ÇÒ´çÇÒ ¸Þ¸ð¸®°¡ ¾ø´Ù. ÀÌ°æ¿ì´Â º¹±¸ÇÒ ¹æ¹ýÀÌ ¾ø´Ù.
						return DEF_XSOCKEVENT_CRITICALERROR;
						break;
					case 0:
						// Å¥°¡ °¡µæÃ¡´Ù. ÀÌ ¼ÒÄÏ Å¬·¡½º´Â »èÁ¦µÇ¾î¾ß¸¸ ÇÑ´Ù.
						return DEF_XSOCKEVENT_QUENEFULL;
						break;
					case 1:
						// Á¤»óÀûÀ¸·Î µ¥ÀÌÅÍ¸¦ µî·ÏÇß´Ù.
						break;
					}
				}
				return DEF_XSOCKEVENT_BLOCK;
			}
		} else iOutLen += iRet;
		
	}
	
	return iOutLen;
}


// ÀÌ ÇÔ¼ö´Â _SendUnsentData¿¡¼­¸¸ »ç¿ëÇÑ´Ù. ºí·ÏÀÌ ¾Æ´Ñ ¿¡·¯½Ã¿¡´Â À½¼ö°ªÀ», ºí·°»óÅÂ¸¦ Æ÷ÇÔÇÑ Àü¼Û»óÅÂ¿¡¼­´Â º¸³½ ¸¸Å­ÀÇ °ªÀ» ¹ÝÈ¯.
int XSocket::_iSend_ForInternalUse(char * cData, int iSize)
{
 int  iOutLen, iRet, WSAErr;

	iOutLen = 0;
	while (iOutLen < iSize) {
		
		iRet = send(m_Sock, (cData + iOutLen), iSize - iOutLen, 0); 
		
		if (iRet == SOCKET_ERROR) {
			WSAErr = WSAGetLastError();
			if (WSAErr != WSAEWOULDBLOCK) {
				// ¼ÒÄÏ¿¡ ¿¡·¯°¡ ¹ß»ýÇß´Ù.
				m_WSAErr = WSAErr;
				return DEF_XSOCKEVENT_SOCKETERROR;
			}
			else {
				// ºí·°»óÅÂÀÌ¸é ´õÀÌ»ó º¸³¾ ¼ö ¾øÀ¸¹Ç·Î Áö±Ý±îÁö º¸³½ µ¥ÀÌÅÍ »çÀÌÁî¸¸À» ¹ÝÈ¯  
				return iOutLen;
			}
		} else iOutLen += iRet;
	}
	
	return iOutLen;
}



int XSocket::_iRegisterUnsentData(char * cData, int iSize)
{
	// Å¥°¡ °¡µæÂ÷¼­ ´õÀÌ»ó µ¥ÀÌÅÍ¸¦ ´ë±â¿­¿¡ ÀúÀåÇÒ ¼ö ¾ø´Ù.
	if (m_pUnsentDataList[m_sTail] != 0) return 0;
	
	m_pUnsentDataList[m_sTail] = new char[iSize];
	if (m_pUnsentDataList[m_sTail] == 0) return -1; // ¸Þ¸ð¸® ÇÒ´ç¿¡ ½ÇÆÐÇß´Ù.

	// µ¥ÀÌÅÍ ÀúÀå 
	memcpy(m_pUnsentDataList[m_sTail], cData, iSize);
	m_iUnsentDataSize[m_sTail] = iSize;

	// Å×ÀÏ Æ÷ÀÎÅÍ Áõ°¡ 
	m_sTail++;
	if (m_sTail >= m_iBlockLimit) m_sTail = 0;

	return 1;
}



int XSocket::_iSendUnsentData()
{
 int iRet;
 char * pTemp;
	
	// °¡´ÉÇÑ ÇÑ ´ë±â¿­ÀÇ µ¥ÀÌÅÍ¸¦ º¸³½´Ù. 
	while (m_pUnsentDataList[m_sHead] != 0) {
		
		iRet = _iSend_ForInternalUse(m_pUnsentDataList[m_sHead], m_iUnsentDataSize[m_sHead]);

		if (iRet == m_iUnsentDataSize[m_sHead]) {
			// HeadÅ¥ÀÇ µ¥ÀÌÅÍ¸¦ ´Ù º¸³Â´Ù.	´ÙÀ½ µ¥ÀÌÅÍ¸¦ º¸³½´Ù.
			delete m_pUnsentDataList[m_sHead];
			m_pUnsentDataList[m_sHead] = 0;
			m_iUnsentDataSize[m_sHead] = 0;
			// Çìµå Æ÷ÀÎÅÍ Áõ°¡ 
			m_sHead++;
			if (m_sHead >= m_iBlockLimit) m_sHead = 0;
		}
		else {
			// º¸³»´ø Áß ¼ÒÄÏ ¿¡·¯°¡ ¹ß»ýÇÏ¸é ±×³É ¸®ÅÏÇÑ´Ù. (Á¦°ÅµÇ¾ß¸¸ ÇÑ´Ù)
			if (iRet < 0) 
				return iRet;

			// µ¥ÀÌÅÍ¸¦ ´Ù º¸³»Áö ¸øÇÏ°í ¶Ç ºí·Ï »óÅÂ°¡ ¹ß»ýÇß´Ù. º¸³»Áö ¸øÇÑ µ¥ÀÌÅÍ¸¸ ³²°Ü³õ´Â´Ù.
			pTemp = new char[m_iUnsentDataSize[m_sHead] - iRet];
			memcpy(pTemp, m_pUnsentDataList[m_sHead] + iRet, m_iUnsentDataSize[m_sHead] - iRet);

			delete m_pUnsentDataList[m_sHead];
			m_pUnsentDataList[m_sHead] = pTemp;

			return DEF_XSOCKEVENT_UNSENTDATASENDBLOCK;
		}
	}

	return DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE;
}


int XSocket::iSendMsg(char * cData, DWORD dwSize, char cKey)
{
 WORD * wp;
 int    i, iRet;

	if (dwSize > m_dwBufferSize) return DEF_XSOCKEVENT_MSGSIZETOOLARGE;

	if (m_cType != DEF_XSOCK_NORMALSOCK) return DEF_XSOCKEVENT_SOCKETMISMATCH;
	if (m_cType == 0) return DEF_XSOCKEVENT_NOTINITIALIZED;

	// Å° ÀÔ·Â 
	m_pSndBuffer[0] = cKey;

	wp  = (WORD *)(m_pSndBuffer + 1);
	*wp = (WORD)dwSize + 3;

	memcpy((char*)(m_pSndBuffer + 3), cData, dwSize);
	if (cKey != 0) { //Encryption
		for (i = 0; i < dwSize; i++) {
			m_pSndBuffer[3 + i] += (i ^ cKey);
			m_pSndBuffer[3 + i] = m_pSndBuffer[3 + i] ^ (cKey ^ ((char)dwSize - i));
		}
	}
	//change starts
	if (m_bWriteMessage == false) { // 2020
		iRet = _iRegisterUnsentData(m_pSndBuffer, dwSize + 3);
	}
	else {
		iRet = _iSend(m_pSndBuffer, dwSize + 3, true);
	}
	// change ends
	if (iRet < 0) return iRet;
	else return (iRet - 3);
}

bool XSocket::bListen(char * pAddr, int iPort, unsigned int uiMsg)
{
 SOCKADDR_IN	 saTemp;

	if (m_cType != 0) return false;
	if (m_Sock  != INVALID_SOCKET) closesocket(m_Sock);

	// ¼ÒÄÏÀ» »ý¼ºÇÑ´Ù. 
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET) 
		return false;
	
	// ÁÖ¼Ò¸¦ ¹ÙÀÎµåÇÑ´Ù.
	memset(&saTemp,0,sizeof(saTemp));
	saTemp.sin_family = AF_INET;
	saTemp.sin_addr.s_addr = inet_addr(pAddr);
	saTemp.sin_port = htons(iPort);
	if ( bind(m_Sock, (PSOCKADDR)&saTemp, sizeof(saTemp)) == SOCKET_ERROR) {
		closesocket(m_Sock);
		return false;
	}
	
	if (listen(m_Sock, 5) == SOCKET_ERROR) {
		closesocket(m_Sock);
		return false;
	}

	WSAAsyncSelect(m_Sock, m_hWnd, uiMsg, FD_ACCEPT);

	m_uiMsg = uiMsg;
	m_cType = DEF_XSOCK_LISTENSOCK;

	return true;
}

bool XSocket::bAccept(class XSocket * pXSock, unsigned int uiMsg)
{
 SOCKET			AcceptedSock;
 sockaddr		Addr;
 int	iLength;
 DWORD			dwOpt;

	if (m_cType != DEF_XSOCK_LISTENSOCK) return false;
	if (pXSock == 0) return false;

	iLength = sizeof(Addr);
	// Å¬¶óÀÌ¾ðÆ®ÀÇ Á¢¼ÓÀ» ¹Þ´Â´Ù . 
	AcceptedSock = accept(m_Sock, (struct sockaddr FAR *)&Addr,(int FAR *)&iLength);
	if (AcceptedSock == INVALID_SOCKET) 
		return false;
		
	pXSock->m_Sock = AcceptedSock;
	WSAAsyncSelect(pXSock->m_Sock, m_hWnd, uiMsg, FD_READ | FD_WRITE | FD_CLOSE);

	// AcceptµÈ ¼ÒÄÏÀº º¸Åë ¼ÒÄÏÀ¸·Î ÃÊ±âÈ­ µÈ´Ù.
	pXSock->m_uiMsg = uiMsg;
	pXSock->m_cType = DEF_XSOCK_NORMALSOCK;

	// ¼ÒÄÏ ¿É¼ÇÀ» Á¶Á¤ÇÑ´Ù. 
	dwOpt = 8192*5;
	setsockopt(pXSock->m_Sock, SOL_SOCKET, SO_RCVBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));
	setsockopt(pXSock->m_Sock, SOL_SOCKET, SO_SNDBUF, (const char FAR *)&dwOpt, sizeof(dwOpt));

	return true;
}

void XSocket::_CloseConn()
{
 char cTmp[100];
 bool bFlag = true;	
 int  iRet;

	if (m_Sock == INVALID_SOCKET) return; // v1.4

	shutdown(m_Sock, 0x01);
	while (bFlag == true) {
		iRet = recv(m_Sock, cTmp, sizeof(cTmp), 0);
		if (iRet == SOCKET_ERROR) bFlag = false;
		else if (iRet == 0) bFlag = false;
	}

	closesocket(m_Sock);

	m_cType = DEF_XSOCK_SHUTDOWNEDSOCK;
}

SOCKET XSocket::iGetSocket()
{
	return m_Sock;
}

char * XSocket::pGetRcvDataPointer(DWORD * pMsgSize, char * pKey)
{
 WORD * wp;
 DWORD  dwSize;
 int i;
 char cKey;
	
	cKey = m_pRcvBuffer[0];
	if (pKey != 0) *pKey = cKey;		// v1.4

	wp = (WORD *)(m_pRcvBuffer + 1);
	*pMsgSize = (*wp) - 3;				// Çì´õÅ©±â´Â Á¦¿ÜÇØ¼­ ¹ÝÈ¯ÇÑ´Ù. 
	dwSize    = (*wp) - 3;

	if (dwSize > DEF_MSGBUFFERSIZE) dwSize = DEF_MSGBUFFERSIZE;

	// v.14 : m_pSndBuffer +3 ºÎÅÍ dwSize±îÁö cKey°¡ 0ÀÌ ¾Æ´Ï¶ó¸é ¾ÏÈ£È­¸¦ Ç¬´Ù.
	if (cKey != 0) {//Encryption
		for (i = 0; i < dwSize; i++) {
			m_pRcvBuffer[3+i]  = m_pRcvBuffer[3+i] ^ (cKey ^ ((char)dwSize - i));
			m_pRcvBuffer[3+i] -= (i ^ cKey);
		}
	}
	return (m_pRcvBuffer + 3);
}

/*********************************************************************************************************************
**  bool _InitWinsock()																								**
**  last updated		:: March 11, 2005; 11:09 PM; Hypnotoad														**
**	return value		:: bool																						**
**********************************************************************************************************************/
bool _InitWinsock()
{
 int     iErrCode;
 WORD	 wVersionRequested;
 WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2); 
	iErrCode = WSAStartup(wVersionRequested, &wsaData);
	if (iErrCode) return false;

	return true;
}



///

int XSocket::iGetPeerAddress(char * pAddrString)
{
 SOCKADDR_IN sockaddr;
 int iRet, iLen;
	
	iLen = sizeof(sockaddr);
	iRet = getpeername(m_Sock, (struct sockaddr *)&sockaddr, &iLen);
	strcpy(pAddrString, (const char *)inet_ntoa(sockaddr.sin_addr));

	return iRet;
}

