#include "NmNetworkAudioClient.h"
#include "CsDebug.h"
#include "NmEngine.h"
#include "CsSpeakerConfig.h"

NmNetworkAudioClient::NmNetworkAudioClient(NmEngine *engine, char *serverIpAddress)
{
	m_engine = engine;
	m_serverIpAddress = serverIpAddress;
	m_serverAddress = inet_addr(m_serverIpAddress);
	m_packetReady = false;

	m_receiveBuffer = (char*)malloc(4*(8192*24)+24);

	waitForPacket = new CsWaitEvent();

	m_latePacketCounter = 0;

	m_msToWait = 0;

	m_active = true;
	m_audioActive = false;
}

NmNetworkAudioClient::~NmNetworkAudioClient()
{
}

void NmNetworkAudioClient::init(int bufferSize, double sampleRate)
{
	csDebug("NmNetworkAudioClient::init()");

	CsAudioTool::init(bufferSize, sampleRate); // Added

	if(m_receiveBuffer) {
		free(m_receiveBuffer);
		m_receiveBuffer = 0;
	}
	m_receiveBuffer = (char*)malloc(4*(bufferSize*16)+24);

	m_msToWait = (int)((double)bufferSize/(sampleRate/(double)1000.0))+1;
	csDebug("msToWait = %d", m_msToWait);

	csDebug("EXIT: NmNetworkAudioClient::init()");
}

int NmNetworkAudioClient::numOutputs(void)
{
	return 4;
}

int NmNetworkAudioClient::outputConfiguration(int outputId)
{
	return CsSpeakerConfig::Surround40;
}

void NmNetworkAudioClient::processAudio(CsAudioBuffer *inputs, CsAudioBuffer **outputs, int numOutputs, double timestamp, double samples)
{
	//csDebug("NmNetworkAudioClient::processAudio()");

	//m_engine->requestNextBlockFromServer(this); 
	if(!(m_active && m_audioActive)) {
		return;
	}

	int ret;
	int st = timeGetTime();
	int et;
	if(ret = waitForPacket->wait(m_msToWait)) {
		//csDebug("past the loop");
		

		int bufferCounter = 0; 

		//if(!m_receiveBuffer) {
		//	init(outputs->numFrames(), 44100.0);
		//}


		int inStream = 0;
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				float *out = outputs[i]->getStream(j);
				for(int k=0; k<outputs[i]->numFrames(); k++) {
					floatchar fc;
					for(int l=0; l<4; l++) {
						//if(fc.c[l]!=0) csDebug("fc.c = %d", fc.c[l]);
						fc.c[l] = m_receiveBuffer[bufferCounter+24];
						bufferCounter++;
					}
					out[k] += fc.f;
					//if(out[k]!=0.0f) {
					//	csDebug("out[k]=%f", out[k]);
					//}
				}
			}
		}

		
		/*for(int i=0; i<16; i++) {
			if(i<outputs->numStreams()) {
				float *out = outputs->getStream(i);
				for(int j=0; j<outputs->numFrames(); j++) {
					floatchar cf;
					for (int k=0; k<4; k++) {
						cf.c[k] = m_receiveBuffer[bufferCounter+24];
						bufferCounter++;
					}
					out[j] += cf.f;//*0.1;
				}
			}
		} */

			//break;

	} else {
		et = timeGetTime();
		csDebug("Late Packet [%s] %d (duration = %d)", m_serverIpAddress, m_latePacketCounter, et-st);
		m_latePacketCounter++;
	}

	
	/*if(ret==WAIT_ABANDONED) {
			CSDEBUG("WAIT_ABANDONED");
		} else if(ret==WAIT_OBJECT_0) {
			CSDEBUG("WAIT_OBJECT_0");
		} else if(ret==WAIT_TIMEOUT) {
			CSDEBUG("WAIT_TIMEOUT");
		}*/
	
}