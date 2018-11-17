#ifndef __ALog_h__
#define __ALog_h__

#include "APlatform.h"

namespace Atom {

class ALog
{
public:
	static void output(const wchar_t *format, ...);
	static void output(const char *format, ...);

protected:
private:
};

}

#define aLog Atom::ALog::output

#endif