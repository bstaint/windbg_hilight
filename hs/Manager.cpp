#include "StdAfx.h"

#include <assert.h>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"

#include "Manager.h"
#include "..\SciLexer\src\Catalogue.h"
#include "LexAsm.h"
#include <tchar.h>
#include "Config.h"

static const TCHAR _afxOldWndProc[] = _T("AfxOldWndProc456");

LRESULT CALLBACK CWndManager::_AfxActivationWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldWndProc = (WNDPROC)::GetProp(hWnd, _afxOldWndProc);
	if (nMsg == WM_NCDESTROY)
	{
		theWndManager.DelWnd(hWnd);
	}
	return  CallWindowProc(oldWndProc, hWnd, nMsg, wParam, lParam);
}
void CWndManager::AddWndToCache( HWND hWnd, WND_TYPE type)
{
	int which = type;
	assert(which != 0);

	// from Microsoft Visual Studio 9.0\VC\atlmfc\src\mfc\wincore.cpp
	WNDPROC oldWndProc;
	oldWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	if (oldWndProc != NULL && GetProp(hWnd, _afxOldWndProc) == NULL)
	{
		SetProp(hWnd, _afxOldWndProc, oldWndProc);
		if ((WNDPROC)GetProp(hWnd, _afxOldWndProc) == oldWndProc)
		{
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (DWORD_PTR)_AfxActivationWndProc);
			m_hwnd[hWnd] = which;
		}
	}
}

bool CWndManager::IsCached( HWND hWnd )
{
	return (0 != GetType(hWnd));
}

void CWndManager::DelWnd( HWND hWnd)
{
	RemoveHook(hWnd);
	m_hwnd.erase(hWnd);
}

CWndManager::~CWndManager()
{
	for (MAP_WND::const_iterator it = m_hwnd.begin()
		; it != m_hwnd.end()
		; )
	{
		RemoveHook(it->first);
		m_hwnd.erase(it++); // be careful!
	}
	m_hwnd.clear();
}

CWndManager& CWndManager::getInstance()
{
	static CWndManager a;
	return a;
}

CWndManager::CWndManager()
{

}

int CWndManager::GetType(HWND hWnd)
{
	MAP_WND::const_iterator ci = m_hwnd.find(hWnd);
	if (ci == m_hwnd.end())
	{
		return 0;
	}
	else
	{
		return ci->second;
	}
}

void CWndManager::RemoveHook(HWND hWnd)
{
	WNDPROC oldWndProc = (WNDPROC)::GetProp(hWnd, _afxOldWndProc);
	if (oldWndProc)
	{
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<INT_PTR>(oldWndProc));
		RemoveProp(hWnd, _afxOldWndProc);
	}
}


CLexManager::CLexManager(void)
{
	const LexerModule *lex = Catalogue::Find(str_asm);
	if (lex)
	{
		ILexer* p = lex->Create();
		if (p)
		{
			// Ìí¼Ó¹Ø¼ü×Ö
			for(int i = 0; i< 7; ++i)
			{
				p->WordListSet(i, theConfig.get_keywords(i));
			}
			lexCache[str_asm] = p;
		}
	}
	assert(lexCache[str_asm] != NULL);

	lex = Catalogue::Find(str_WindbgCmd);
	if (lex)
	{
		ILexer* p = lex->Create();
		if (p)
		{
			lexCache[str_WindbgCmd] = p;
		}
	}
	assert(lexCache[str_WindbgCmd] != NULL);

}

CLexManager::~CLexManager(void)
{
	for (LEXCACHE::iterator i = lexCache.begin()
		; i != lexCache.end()
		; ++i)
	{
		i->second->Release();
	}
	lexCache.clear();
}

CLexManager& CLexManager::getInstance()
{
	static CLexManager a;
	return a;
}

ILexer* CLexManager::getILexer(const char* name)
{
	return lexCache[name];
}

ILexer* CLexManager::getILexerDisassemblyWnd() 
{
	return getILexer(str_asm);
}

ILexer* CLexManager::getILexerCommandWnd() 
{
	return getILexer(str_WindbgCmd);
}

const char* CLexManager::str_WindbgCmd = "WindbgCmd";

const char* CLexManager::str_asm = "asm";
