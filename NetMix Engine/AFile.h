#ifndef __AFile_h__
#define __AFile_h__

#include "AString.h"
#include "APlatform.h"
//#include "ATypes.h"

namespace Atom {

class AFile
{
public:
	AFile(AString filename);

	int open(void);
	int create(void);
	int close(void);

	AString filename(void) { return m_filename; }

	int setPosition(int fromFlag, long position);
	enum {
		fromBeginning = 0,
		fromCurrent = 1
	};
	long position(void);
	long long size(void);

	int readBytes(int numBytes, void *buffer);
	int writeBytes(int numBytes, void *buffer);

	//int readFloatLsb(float *value);
	//int writeFloatLsb(float value);

	int readU16LSB(unsigned short *value);
	int readU32LSB(unsigned long *value);
	int read32LSB(long *value);

	int readU16MSB(unsigned short *value);
	int readU32MSB(unsigned long *value);
	int read32Msb(long *value);

	int writeU16LSB(unsigned short value);
	int writeU32LSB(unsigned long value);
	int write32LSB(long value);

	int readChunkName(char *chunkName);
	int writeChunkName(char *chunkName);
	bool compareChunkName(char *chunk1, char *chunk2);

	

protected:
private:
	AString m_filename;
	HANDLE hFile;
};

}

#endif // __AFile_h__