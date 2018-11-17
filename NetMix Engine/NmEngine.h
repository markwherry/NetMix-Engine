#ifndef __NMENGINE_H__
#define __NMENGINE_H__

#include "CsEngine.h"
using namespace CsEngineFramework;

#include "CsUdpSocket.h"
#include "CsWaitEvent.h"
using namespace CsCore;

#include "NmNetworkAudioClient.h"

#include <vector>

class NmEngine : public CsEngine, public CsThread
{
public:
	NmEngine();
	~NmEngine();

	int init(void);
	int exit(void);

	void startingAudio(long bufferSize, double sampleRate);
	
	void preProcessAudio(void);

	void createServer(char *ipAddress);
	int numServers(void) { return networkAudioClientList.size(); }
	void requestNextBlockFromServer(NmNetworkAudioClient *fromClient);

	NmNetworkAudioClient* networkAudioClient(int id) { return networkAudioClientList[id]; }

	void buildHeader(char *buffer, char message);

	CsCriticalSection csRequest;

protected:

private:
	void sendResetToServers(int bufferSize, double sampleRate);
	void threadProcess(void);

	CsUdpSocket *udpSocket;

	std::vector<NmNetworkAudioClient*> networkAudioClientList;

	char *receiveBuffer;

	int numOutputsPerServer;

	CsWaitEvent *waitForProcess;

	bool threadStarted;
};

#endif