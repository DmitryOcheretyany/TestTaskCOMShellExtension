/*
*	Adaptaded from codeproject.com
*/

#include "Registry.h"
long g_cRefThisDll;
HANDLE g_module;

#include "ClassFactory.hpp"
#include "MyInterface.hpp"
#include "Registrar.h"

long * CObjRoot::p_ObjCount = nullptr; // this is just because i didnt want to use any globals inside the
									// class framework.

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH: 
			g_module = hModule; 
			CObjRoot::p_ObjCount = &g_cRefThisDll; 
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
{
	*ppvOut = nullptr;
    if (IsEqualIID(rclsid, CLSIDm))
    {
       // declare a classfactory for CmyInterface class 
       CClassFactory<ShellEx> *pcf = new  CClassFactory<ShellEx>;
       return pcf->QueryInterface(riid,ppvOut);
    }
    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI  DllCanUnloadNow(void)
{
    return (g_cRefThisDll == 0 ? S_OK : S_FALSE);
}

STDAPI DllRegisterServer(void)
{
	CDllRegistrar registrar;  // this class should create standard entries in registry 
	char path[MAX_PATH];
	GetModuleFileName((HMODULE)g_module, path, MAX_PATH);

	return registrar.RegisterObject(CLSIDm, "*\\ShellEx\\ContextMenuHandlers\\COM_SHELL_EX", "COM_SHELL_EX", path) ? S_OK:S_FALSE;
}

STDAPI DllUnregisterServer(void)
{
	CDllRegistrar registrar;
	return registrar.UnRegisterObject(CLSIDm, "*\\ShellEx\\ContextMenuHandlers\\COM_SHELL_EX", "COM_SHELL_EX") ? S_OK:S_FALSE;
}

