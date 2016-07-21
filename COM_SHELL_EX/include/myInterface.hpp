#include <string.h>
#include <vector>
#include "myInterfaceIUnknown.hpp"

#ifndef __MYINTERFACEIUNKNOWN_HPP__
#define __MYINTERFACEIUNKNOWN_HPP__

#include "ComBase.hpp"

class ShellEx: public ImyInterfaceShell
{
public:
	ShellEx();
	~ShellEx();
	// IUnknown methods
	STDMETHOD (QueryInterface) (REFIID riid, LPVOID * ppvObj) ;
	STDMETHOD_ (ULONG, AddRef) (void) final;
	STDMETHOD_ (ULONG, Release) (void) final;
	
    // IShellExtInit
    STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY) final;

    // IContextMenu
    STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT) final;
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO) final;
    STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT) final;
protected:
	DWORD m_ObjRefCount;
private:
    unsigned int uNumFiles;
	std::vector<std::string> selectedFilesName;
};

#endif // __MYINTERFACE_HPP__
