#if defined(_WIN64)
 typedef unsigned __int64 UINT_PTR;
#else
 typedef unsigned int UINT_PTR;
#endif

#include <ComDef.h>
#include <shlobj.h>

// GUID for COM Generated with "Create GUID" tool from "Microsoft Windows SDK"

_declspec(selectany) GUID CLSIDm = { 0xff02f40e, 0x29df, 0x4003, { 0x94, 0x18, 0xff, 0x6b, 0xb6, 0x51, 0xba, 0x6d } };

interface __declspec(uuid("FF02F40E-29DF-4003-9418-FF6BB651BA6D")) ImyInterfaceShell : public IShellExtInit, IContextMenu
{
	// IUnknown methods
	STDMETHOD (QueryInterface) (REFIID riid, LPVOID * ppvObj) override PURE;
	STDMETHOD_ (ULONG, AddRef) (void) override PURE;
	STDMETHOD_ (ULONG, Release) (void) override PURE;
	
    // IShellExtInit methods
    STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY) override PURE;

    // IContextMenu methods
    STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT) override PURE;
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO) override PURE;
    STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT) override PURE;
};

