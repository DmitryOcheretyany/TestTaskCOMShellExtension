#ifndef __CLASSFACTORY_HPP__
#define __CLASSFACTORY_HPP__

#include <iostream>
#include "ComBase.hpp"

template<class comObj>
class CSingleCreator
{
protected:
	CSingleCreator(): m_pObj(0) {};

	comObj *CreateObject()
	{
		if(!m_pObj)
		{
			m_pObj = new comObj;
		}
		return m_pObj;
	}
	comObj * m_pObj;
};

template<class comObj>
class CMultiCreator
{
protected:
	CMultiCreator():m_pObj(0) {};
	comObj *CreateObject()
	{
		return new comObj;
	}
	comObj * m_pObj;
};
 
template <class comObj, class creatorClass  = CMultiCreator < comObj > >
class CClassFactory :  public CComBase<>, public InterfaceImpl<IClassFactory>, public creatorClass 
{
public:
	CClassFactory() {};
	virtual ~CClassFactory() {};

	STDMETHOD(QueryInterface)(REFIID riid,LPVOID *ppv)
	{
		*ppv = nullptr;
		if(IsEqualIID(riid,IID_IUnknown) || IsEqualIID(riid,IID_IClassFactory))
		{
			*ppv = (IClassFactory *) this;
			_AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppvObj)
	{
		*ppvObj = nullptr;
		if (pUnkOuter)
    		return CLASS_E_NOAGGREGATION;
		m_pObj = CreateObject();  // m_pObj is defined in creatorClass 
		if (!m_pObj)
    		return E_OUTOFMEMORY;
		HRESULT hr = m_pObj->QueryInterface(riid, ppvObj);
		if(hr != S_OK)
		{
			delete m_pObj;
		}
		return hr;
	}

	STDMETHODIMP LockServer(BOOL) {	return S_OK; }  // not implemented
};

#endif // __CLASSFACTORY_HPP__
