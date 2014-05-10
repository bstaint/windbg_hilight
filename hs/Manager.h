#pragma once
#include <map>

#define theWndManager (CWndManager::getInstance())
#define theLexManager (CLexManager::getInstance())

class CWndManager
{
public:
	enum WND_TYPE{
		WND_Disassembly=1,
		WND_COMMAND=2,
		WND_OTHER=10,
	};

	static CWndManager& getInstance();

	// WND_TYPE
	void AddWndToCache(HWND hWnd, WND_TYPE type);
	bool IsCached(HWND hWnd);

	int GetType(HWND hWnd);

protected:
	CWndManager();
	~CWndManager();


	void DelWnd(HWND);
	void RemoveHook(HWND);
	static LRESULT CALLBACK
		_AfxActivationWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	typedef std::map<HWND, int> MAP_WND;
	MAP_WND m_hwnd;

};

class ILexer;
class CLexManager
{
public:
	static CLexManager& getInstance();

	ILexer* getILexerDisassemblyWnd();
	ILexer* getILexerCommandWnd();

protected:

	ILexer* getILexer(const char* name);

	static const char* str_asm;
	static const char* str_WindbgCmd;;

	CLexManager(void);
	~CLexManager(void);
	typedef std::map<const char*, ILexer*> LEXCACHE;
	std::map<const char*, ILexer*> lexCache;
};
