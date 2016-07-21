#include "myInterface.hpp"

ShellEx::ShellEx()
{

}

ShellEx::~ShellEx()
{

}

STDMETHODIMP ShellEx::QueryInterface(REFIID riid, LPVOID *retPointer)
{
	*retPointer = nullptr;

	if(IsEqualIID(riid,IID_IUnknown) || IsEqualIID(riid,__uuidof(ImyInterfaceShell)))
	{
		*retPointer = (ImyInterfaceShell*)this;
	}
	else if( IsEqualIID(riid, IID_IClassFactory) )
	{
		*retPointer = (IClassFactory*)this;
	}
	else if ( IsEqualIID(riid, IID_IShellExtInit))
	{
		*retPointer = (IShellExtInit*)this;
	}
	else if ( IsEqualIID(riid, IID_IContextMenu))
	{
		*retPointer = (IContextMenu*)this;
	}

	if(*retPointer)
    {
        LPUNKNOWN pUnk = (LPUNKNOWN)(*retPointer);
		pUnk->AddRef();
		return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(DWORD) ShellEx::AddRef(void)
{
	return ++m_ObjRefCount;
}

STDMETHODIMP_(DWORD) ShellEx::Release(void)
{
	if(--m_ObjRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_ObjRefCount;
}

STDMETHODIMP ShellEx::Initialize (
	LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )
{
	FORMATETC fmt = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	if ( FAILED( pDataObj->GetData ( &fmt, &stg ) ))
	{
		return E_INVALIDARG;
	}

	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	if ( nullptr == hDrop )
		return E_INVALIDARG;
	
	uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, nullptr, 0 );
	if ( 0 == uNumFiles )
	{
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg );
		return E_INVALIDARG;
	}
	selectedFilesName.resize(uNumFiles);

	for (int i = 0; i < uNumFiles; i++)
	{
		char fileTMP[MAX_PATH];
		DragQueryFile(hDrop, 0, fileTMP, MAX_PATH);
		selectedFilesName.push_back(std::string(fileTMP));
	}
	HRESULT hr = S_OK;

	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );

	return hr;
}

STDMETHODIMP ShellEx::QueryContextMenu (
	HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
	UINT uidLastCmd, UINT uFlags )
{
	if (CMF_DEFAULTONLY & uFlags)
    {
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, 0);
    }

    UINT uID = uidFirstCmd;
    UINT pos = uMenuIndex;

    MENUITEMINFO mii = { sizeof(mii) };
    mii.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_STATE;
    mii.fType = MFT_STRING;
    mii.dwTypeData = ("Avid the best!");
    mii.fState = MFS_ENABLED;
    mii.wID = uID++;

    if (!InsertMenuItem(hmenu, pos++, TRUE, &mii))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, uID - uidFirstCmd);
}

STDMETHODIMP ShellEx::GetCommandString (
	UINT_PTR idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax )
{
	if ( 0 != idCmd )
	{
		return E_INVALIDARG;
	}
	if ( uFlags & GCS_HELPTEXT )
	{
		LPCTSTR szText = ("help");

		if ( uFlags & GCS_UNICODE )
		{	
			DWORD len;
			len = MultiByteToWideChar(CP_ACP, 0, szText, -1, 0, 0);
			BSTR  strptr;
			strptr = SysAllocStringLen(0, len);
			MultiByteToWideChar(CP_ACP, 0, szText, -1, strptr, len);
			lstrcpynW ( (LPWSTR) pszName, (LPWSTR)(strptr), cchMax );
			SysFreeString(strptr);
		}
		else
		{
			lstrcpynA ( pszName, (LPSTR)szText, cchMax );
		}
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP ShellEx::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	if (0 != HIWORD(pCmdInfo->lpVerb))
	{
		return E_INVALIDARG;
	}

	switch ( LOWORD( pCmdInfo->lpVerb) )
	{
	case 0:
		{
			std::string fullnames;
			for(auto f: selectedFilesName)
			{
				fullnames.append(f);
				fullnames.append(", ");
			}
			const unsigned int length = fullnames.length();
			char* szMsg = new TCHAR[length];
			szMsg = const_cast<char*>(fullnames.c_str());

			MessageBox ( pCmdInfo->hwnd, szMsg, "Avid the best of the best!",
				MB_ICONINFORMATION );

			delete[] szMsg;
			return S_OK;
		}
		break;

	default:
		return E_INVALIDARG;
		break;
	}
}
