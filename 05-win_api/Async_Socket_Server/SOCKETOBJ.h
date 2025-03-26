#pragma once
#include <Windows.h>
typedef struct  _SOCKETOBJ
{
	SOCKET m_socket;//socket
	CHAR m_szIP[16]; //ip
	USHORT m_usPort; //port
	_SOCKETOBJ* m_pNext;//next node
}SOCKETOBJ, *PSOCKETOBJ;

PSOCKETOBJ g_pSocketObjHeader;//header
SIZE_T g_nTotalClient;//total count client

CRITICAL_SECTION g_cs;// shared section for sync obj

//create a socketobj

PSOCKETOBJ CreateSocketObj(SOCKET s)
{
	PSOCKETOBJ pSocketObj = new SOCKETOBJ;
	if (pSocketObj == NULL) {
		return NULL;
	}

	EnterCriticalSection(&g_cs);

	pSocketObj->m_socket = s;

	//add first node
	if (g_pSocketObjHeader == NULL)
	{
		g_pSocketObjHeader = pSocketObj;
		g_pSocketObjHeader->m_pNext = NULL;;
	}
	else
	{
		pSocketObj->m_pNext = g_pSocketObjHeader;
		g_pSocketObjHeader = pSocketObj;
	}

	g_nTotalClient++;

	LeaveCriticalSection(&g_cs);
	return pSocketObj;
}

//free a socket
VOID FreeSocketObj(PSOCKETOBJ pSocketObj)
{
	EnterCriticalSection(&g_cs);

	PSOCKETOBJ p = g_pSocketObjHeader;

	if (p == pSocketObj)//free target is header
	{
		g_pSocketObjHeader = g_pSocketObjHeader->m_pNext;
	}
	else
	{
		while (p != NULL)
		{
			if (p->m_pNext == pSocketObj)
			{
				p->m_pNext = pSocketObj->m_pNext;
				break;
			}

			p = p->m_pNext;
		}
	}
	if (pSocketObj->m_socket != INVALID_SOCKET)
		closesocket(pSocketObj->m_socket);
	delete pSocketObj;

	g_nTotalClient--;

	LeaveCriticalSection(&g_cs);
}

//find a socket
PSOCKETOBJ FindSocketObj(SOCKET s)
{
	EnterCriticalSection(&g_cs);

	PSOCKETOBJ pSocketObj = g_pSocketObjHeader;
	while (pSocketObj != NULL)
	{
		if (pSocketObj->m_socket == s)
		{
			LeaveCriticalSection(&g_cs);
			return pSocketObj;
		}
		pSocketObj = pSocketObj->m_pNext;
	}
	LeaveCriticalSection(&g_cs);
	return NULL;
}

//delete all
VOID DeleteAllSockets()
{
	SOCKETOBJ socketobj;
	PSOCKETOBJ pSocketObj = g_pSocketObjHeader;

	while (pSocketObj != NULL)
	{
		socketobj = *pSocketObj;
		if (pSocketObj->m_socket != INVALID_SOCKET)
		{
			closesocket(pSocketObj->m_socket);
		}
		delete pSocketObj;

		pSocketObj = socketobj.m_pNext;
	}
}