#include "stdafx.h"
#include <windows.h>
#include "detours.h"

#include <assert.h>

#include "SciLexer.h"
#include "LexDbgCmd.h"
#include "TextBuffer.h"
#include "Manager.h"

#ifdef _WIN64
#pragma comment(lib, "Detours_x64.lib")
#else
#pragma comment(lib, "Detours_x86.lib")
#endif

EXTTEXTOUTW ExtTextOutW_Org = ExtTextOutW;

BOOL WINAPI ExtTextOutW_Hook(HDC hdc,          // handle to DC
                int X,            // x-coordinate of reference point
                int Y,            // y-coordinate of reference point
                UINT fuOptions,   // text-output options
                CONST RECT* lprc, // optional dimensions  
                LPCWSTR lpString, // string
                UINT cbCount,     // number of characters in string
                CONST INT* lpDx   // array of spacing values
                )
{
    if (cbCount <= 4 || fuOptions != 4)
    {
        return ExtTextOutW_Org(hdc, X, Y, fuOptions, lprc, lpString, cbCount, lpDx);
    }
    if (cbCount && (*lpString < 0x20 || *lpString > 0x7f))
    {
        return ExtTextOutW_Org(hdc, X, Y, fuOptions, lprc, lpString, cbCount, lpDx);
    }
    if (GetTextColor(hdc) == 0x0FFFFFF)
    {
        return ExtTextOutW_Org(hdc, X, Y, fuOptions, lprc, lpString, cbCount, lpDx);
    }
    else
    {
        HWND hWnd = WindowFromDC(hdc);

        if (!hWnd) // 如果是0就没法加入到cache中去
        {
            return ExtTextOutW_Org(hdc, X, Y, fuOptions, lprc, lpString, cbCount, lpDx);
        }

		int WndType = 0;
        if (!theWndManager.IsCached(hWnd))
        {
            HWND hParent = GetParent(hWnd);
			WndType = CWndManager::WND_OTHER; // other
            do 
            {
                char sname[100];
                GetClassNameA(hParent, sname, sizeof(sname));
                if (0 == _strnicmp(sname, "WinBaseClass", 12))
                {
                    GetWindowTextA(hParent, sname, sizeof(sname));
                    if (0 == _strnicmp(sname, "Disassembly", 11)) // 通常是 Disassembly - WinDbg:6.12.0002.633 X86 
                    {
                        WndType = CWndManager::WND_Disassembly;
                        break;
                    }
                    else if(0 == _strnicmp(sname, "Command", 7))
                    {
                        WndType = CWndManager::WND_COMMAND;
                        break;
                    }
                }
                hParent = GetParent(hParent);
            } while (hParent);

			theWndManager.AddWndToCache(hWnd, (CWndManager::WND_TYPE)WndType); // 添加到缓存中
        }
		WndType = theWndManager.GetType(hWnd);

        if ((WndType == 0) || (CWndManager::WND_OTHER == WndType))
        {
            return ExtTextOutW_Org(hdc, X, Y, fuOptions, lprc, lpString, cbCount, lpDx);
        }

        // Disassembly窗口
        if (CWndManager::WND_Disassembly == WndType)
        {
			ILexer* p = theLexManager.getILexerDisassemblyWnd();
			assert(p != NULL);

			static CTextBuffer::hook_data hd;
			hd.hdc = hdc;
			hd.X = X;
			hd.Y = Y;
			hd.fuOptions = fuOptions;
			hd.lprc = lprc;
			hd.lpString = lpString;
			hd.cbCount = cbCount;
			hd.lpDx = lpDx;
			hd.ExtTextOutW_Org = ExtTextOutW_Org;

			static class CTextBuffer c;
			c.SetHookData(hd);

			// 该函数在SetStyles中自己显示
			p->Lex(0, cbCount, SCE_ASM_DEFAULT, &c);
			return TRUE;
		}

		// Command窗口
		if (CWndManager::WND_COMMAND == WndType)
		{
			ILexer* p = theLexManager.getILexerCommandWnd();
			assert(p != NULL);

			static CTextBuffer::hook_data hd;
			hd.hdc = hdc;
			hd.X = X;
			hd.Y = Y;
			hd.fuOptions = fuOptions;
			hd.lprc = lprc;
			hd.lpString = lpString;
			hd.cbCount = cbCount;
			hd.lpDx = lpDx;
			hd.ExtTextOutW_Org = ExtTextOutW_Org;

			static class CTextBuffer c;
			c.SetHookData(hd);

			// 该函数在SetStyles中自己显示
			p->Lex(0, cbCount, SCE_DBGCMD_DEFAULT, &c);
			return TRUE;
		}
    }
    return FALSE;
}

struct st_iu_hooks
{
	st_iu_hooks()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)ExtTextOutW_Org, ExtTextOutW_Hook);
		DetourTransactionCommit();
	}
	~st_iu_hooks()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)ExtTextOutW_Org, ExtTextOutW_Hook);
		DetourTransactionCommit();
	}
}g_iu_hook;
