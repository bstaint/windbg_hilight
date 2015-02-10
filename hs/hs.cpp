#include "stdafx.h"
#include <DbgEng.h>

IDebugControl3* gIDebugControl3 = NULL;

#define extc extern "C"

extc HRESULT DebugExtensionInitialize(PULONG Version, PULONG Flags)
{
	*Version = 0x10001;
	*Flags = 0;
	return S_OK;
}

extc void DebugExtensionUninitialize(void)
{
    if(gIDebugControl3 != NULL)
    {
        gIDebugControl3->Release();
        gIDebugControl3 = NULL;
    }
}

extc HRESULT help(void* pDebugClient, PCSTR args)
{
	return S_OK;
}

extc void CALLBACK DebugExtensionNotify(ULONG Notify, ULONG64 Argument)
{
}


extc HRESULT CALLBACK
KnownStructOutputEx(_In_ PDEBUG_CLIENT Client,
                    _In_ ULONG Flags,
                    _In_ ULONG64 Offset,
                    _In_opt_ PCSTR TypeName,
                     PSTR Buffer,
                    PULONG BufferChars)
{
    if (!gIDebugControl3)
    {
        Client->QueryInterface(__uuidof(IDebugControl3), (PVOID*)&gIDebugControl3);
    }
    return S_OK;
}
