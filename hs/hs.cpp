#include "stdafx.h"

#define extc extern "C"

extc HRESULT DebugExtensionInitialize(PULONG Version, PULONG Flags)
{
	*Version = 0x10001;
	*Flags = 0;
	return S_OK;
}

extc void DebugExtensionUninitialize(void)
{
}

extc HRESULT help(void* pDebugClient, PCSTR args)
{
	return S_OK;
}

extc void CALLBACK DebugExtensionNotify(ULONG Notify, ULONG64 Argument)
{
}
