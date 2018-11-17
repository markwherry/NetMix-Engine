#include "NmEngine.h"

int main(void)
{
	SIZE_T dwMin, dwMax;
	GetProcessWorkingSetSize(GetCurrentProcess(), &dwMin, &dwMax);
	csDebug ("GetProcessWorkingSetSize(%ld,%ld)",dwMin/1024, dwMax/1024);


	//bool bRet = SetProcessWorkingSetSizeEx(GetCurrentProcess(),(1024*1024*1024)*1.0,(1024*1024*1024)*1.0,0);
	bool bRet = SetProcessWorkingSetSize(GetCurrentProcess(),(1024*1024*1024)*1.0,(1024*1024*1024)*1.0);
	if (bRet) csDebug ("SetProcessWorkingSetSize() OK");
	else {
		csDebug ("SetProcessWorkingSetSize () FAIL");
		csDebug ("%d",GetLastError ());
		//ErrorExit2(0);
	}

	GetProcessWorkingSetSize(GetCurrentProcess(), &dwMin, &dwMax);
	csDebug ("GetProcessWorkingSetSize(%lld,%lld)",dwMin/1024, dwMax/1024);

	timeBeginPeriod(1);

	NmEngine *e = new NmEngine();
	e->init();

	getchar();

	e->exit();

	delete e;

	return 1;
}