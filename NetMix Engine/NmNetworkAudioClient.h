#ifndef __NMNETWORKAUDIOCLIENT_H__
#define __NMNETWORKAUDIOCLIENT_H__

#include "CsAudioTool.h"
//#include "CsOscillator.h"
#include "CsWaitEvent.h"

class NmEngine;

class NmNetworkAudioClient : public CsAudioTool
{
public:
	NmNetworkAudioClient(NmEngine *engine, char *serverIpAddress);
	~NmNetworkAudioClient();

	void init(int bufferSize, double sampleRate);
	void processAudio(CsAudioBuffer *inputs, CsAudioBuffer **outputs, int numOutputs, double timestamp, double samples);
	int numOutputs(void);
	int outputConfiguration(int outputId);

	char* serverIpAddress(void) { return m_serverIpAddress; }
	long serverAddress(void) { return m_serverAddress; }

	char* receiveBuffer(void) { return m_receiveBuffer; }

	void setPacketReady(bool packetReady = true) { m_packetReady = packetReady; }

	void signalPacketReady(void) { waitForPacket->set(); }

	bool isActive(void) { return m_active; }
	void setActive(bool active) { m_active = active; }

	bool isAudioActive(void) { return m_audioActive; }
	void setAudioActive(bool active) { m_audioActive = active; }

protected:
private:
	NmEngine *m_engine;
	char *m_serverIpAddress;
	long m_serverAddress;
	
	char *m_receiveBuffer;

	bool m_packetReady;
	CsWaitEvent *waitForPacket;

	int m_latePacketCounter;

	int m_msToWait;
	bool m_active;
	bool m_audioActive;
};

#endif