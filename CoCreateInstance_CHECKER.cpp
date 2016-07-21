/*
*	entry point for console app(checker COM)
*/

#include <ComDef.h>
#include <iostream>
//#include "../../../myServ/myInterface.h"
#include "../../COM_SHELL_EX/COM_SHELL_EX/include/myInterface.hpp"

using  namespace std;

void main()
{
	CoInitialize(0);
	HRESULT hr;
	ImyInterfaceShell *pmine=(0);
	CoInitialize(nullptr);
	hr = CoCreateInstance(CLSIDm,				// CLSID of COM server
						  NULL,						//
						  CLSCTX_INPROC_SERVER,		// it is a DLL 
						  __uuidof(ImyInterfaceShell),	// the interface IID
						  (void**)&pmine			// 
						  );
	if(FAILED(hr))
	{
		std::cout<<"Failed to initialize COM obj. !" <<std::endl;
		return ;
	}

	long value = 10;


	if(FAILED(hr))
		std::cout<<"failed square of ImyInterface!" <<std::endl;
	else
		std::cout<<"Cube of the number is %ld. \n" << value <<std::endl;


	// Release 
	pmine->Release();
	CoUninitialize();
}

