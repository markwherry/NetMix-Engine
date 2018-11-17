#include "NmEngine.h"
#include "CsSpeakerConfig.h"

#include "AFile.h"
#include "AXmlDocument.h"
#include "ALog.h"
using namespace Atom;

// TODO hello mark kjhkj 

//#define STUDIO_C1
//#define STUDIO_C2
//#define PASADENA1
//#define PASADENA2
//#define ANDREW
//#define ANDREW1
//#define ANDREW2
//#define MR_XL
//#define HENRY
//#define HENRYDOM
//#define HENRY_NEW
//#define HENRY_TEST
//#define HENRY_NEWRIG
//#define HENRY_POD
//#define HENRY_POD
//#define LORNE
//#define DOOLEY
//#define DOOLEY_A
//#define DOOLEY_B
//#define TEST
//#define HANS
//#define HANS2
//#define STUDIOA
//#define HENRY
//#define ATLI_NEWNEW
//#define ATLI_NEW
//#define ATLI
//#define ATLI_A
//#define ATLI_B
//#define ZANELLI_A
//#define ZANELLI_B
//#define BERGERAC
//#define RAMIN_B
//#define TREVOR
//#define JABLONSKY
//#define MAGICBOX
//#define DOM

NmEngine::NmEngine()
: CsEngine()
{
	setMultiCore(true);

	udpSocket = new CsUdpSocket();

	numOutputsPerServer = 16;

	//receiveBuffer = 0;
	receiveBuffer = (char*)malloc(4*(8192*numOutputsPerServer)+24); csDebug("3");

	waitForProcess = new CsWaitEvent(true);

	threadStarted = false;
}

NmEngine::~NmEngine()
{
	
}

int NmEngine::init(void)
{
	/*TCHAR moduleFilename[1024];
	GetModuleFileName(NULL, moduleFilename, 1024);

	AString filename(moduleFilename);

	AFile file(filename.path()+"mixer.xml");

	if(!file.open()) {
		aLog("mixer.xml could not be found or opened.");
		return 0;
	}

	char *data = (char*)malloc(file.size()+1);
	file.readBytes(file.size(), data);
	data[file.size()] = 0;

	file.close();

	AXmlDocument doc;
	doc.setSource(data);
	if(!doc.rootElement()) {
		aLog("!doc.element()");
		return 0;
	}

	AXmlElement *elementMixer = doc.rootElement();
	if(elementMixer->tag() == "Mixer") {
		if(elementMixer->hasAttribute("address")) {
			AString address = elementMixer->attribute("address");
			aLog("<Mixer> address=[%s]", address.toChar());
			udpSocket->open(9931, address.toChar());
		} else {
			aLog("Error reading mixer.xml: <Mixer> does not have address attribute.");
			return 0;
		}
	} else {
		aLog("Error reading mixer.xml: <Mixer> not found.");
		return 0;
	}

	AXmlElement *elementServers = doc.rootElement()->childElement(1);
	aLog("Number of servers = %d", elementServers->numChildElements());
	for(int i=0; i<elementServers->numChildElements(); i++) {
		AXmlElement *elementServer = elementServers->childElement(i);
		if(elementServer->tag() == "Server") {
			aLog("Creating Server %d", i+1);

			if(elementServer->hasAttribute("address")) {
				AString address = elementServer->attribute("address");
				aLog("<Server> address=[%s]", address.toChar());
				createServer(address.toChar());
			} else {
				aLog("Error reading mixer.xml: <Mixer> does not have address attribute.");
				return 0;
			}

		} else {
			aLog("Error reading mixer.xml: <Server> not found.");
			return 0;
		}
	}

	if(!CsEngine::init()) {
		aLog("***** FAILED TO INIT ENGINE *****");
		return 0;
	}

	

	AXmlElement *elementOutputs = doc.rootElement()->childElement(0);
	aLog("Number of outputs = %d", elementOutputs->numChildElements());
	for(int i=0; i<elementOutputs->numChildElements(); i++) {
		AXmlElement *elementOutput = elementOutputs->childElement(i);
		if(elementOutput->tag() == "Output") {
			aLog("Creating Output %d", i+1);
			if(elementOutput->hasAttribute("id")) {
				int id = elementOutput->attribute("id").toInt();
				AString description = elementOutput->attribute("description", "");
				executeRealTimeCommand(new CsRcCreateOutput(project(), id, description.toChar(), CsSpeakerConfig::Surround40, -1));
			} else {
				aLog("Error reading mixer.xml: <Output> does not have id attribute.");
				return 0;
			}
		} else {
			aLog("Error reading mixer.xml: <Output> not found.");
			return 0;
		}
	}

	//AXmlElement *elementServers = doc.rootElement()->childElement(1);
	aLog("Number of servers = %d", elementServers->numChildElements());
	for(int i=0; i<elementServers->numChildElements(); i++) {
		AXmlElement *elementServer = elementServers->childElement(i);
		if(elementServer->tag() == "Server") {
			//aLog("Creating Server %d", i+1);

			//if(elementServer->hasAttribute("address")) {
			//	createServer(elementServer->attribute("address").toChar());
			//} else {
			//	aLog("Error reading mixer.xml: <Mixer> does not have address attribute.");
			//	return 0;
			//}

			executeRealTimeCommand(new CsRcCreateChannelWithGenerator(project(), networkAudioClientList[i], audioDevice()->bufferSize(), audioDevice()->sampleRate()));

			for(int j=0; j<elementServer->numChildElements(); j++) {
				AXmlElement *elementPatch = elementServer->childElement(j);
				if(elementPatch->tag() == "Patch") {
					if(elementPatch->hasAttribute("source") && elementPatch->hasAttribute("destination")) {
						executeRealTimeCommand(new CsRcSetChannelAudioOutputBusId(project(), i, elementPatch->attribute("source").toInt(), elementPatch->attribute("destination").toInt()));
					} else {
						aLog("Error reading mixer.xml: <Patch> %d does not have source and destination attributes.", j+1);
						return 0;
					}
				} else {
					aLog("Error reading mixer.xml: <Patch> %d not found.", j+1);
					return 0;
				}
			}

		} else {
			aLog("Error reading mixer.xml: <Server> not found.");
			return 0;
		}
	}



	aLog("mixer.xml ok");


	if(!threadStarted) { 
		if(udpSocket->isConnected()) {
			startThread();
			setPriority(THREAD_PRIORITY_TIME_CRITICAL);
			//setPriority(THREAD_PRIORITY_HIGHEST);
		}
		threadStarted = true;
	}

	return 1;

	aLog("***** SHOULD NEVER EXECUTE *****");//*/

#ifdef MAGICBOX
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
	createServer("101.1.1.6");
	createServer("101.1.1.7");
	createServer("101.1.1.8");
	createServer("101.1.1.9");
#endif

#ifdef JABLONSKY
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
#endif

#ifdef TREVOR
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.20");
	createServer("101.1.1.40");
#endif

#ifdef RAMIN_B
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.40");
	createServer("101.1.1.41");
	createServer("101.1.1.60");
	createServer("101.1.1.61");
	createServer("101.1.1.62");
#endif

#ifdef ZANELLI_A
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.80");
#endif

#ifdef ZANELLI_B
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.20");
	createServer("101.1.1.21");
	createServer("101.1.1.40");
	createServer("101.1.1.41");
	createServer("101.1.1.42");
	createServer("101.1.1.60");
#endif

#ifdef PASADENA1
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
#endif

#ifdef PASADENA2
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.10");
	createServer("101.1.1.11");
	createServer("101.1.1.12");
#endif

#ifdef STUDIO_C1
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
#endif

#ifdef STUDIO_C2
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.5");
	createServer("101.1.1.6");
	createServer("101.1.1.7");
	createServer("101.1.1.8");
#endif

#ifdef ANDREW
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
#endif

#ifdef ANDREW1
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
#endif

#ifdef ANDREW2
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.10");
	createServer("101.1.1.11");
	createServer("101.1.1.12");
#endif

#ifdef MR_XL
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
	createServer("101.1.1.6");
#endif

#ifdef ATLI_NEWNEW
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.4");
#endif

#ifdef ATLI_NEW
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.20");
#endif

#ifdef ATLI
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");

	//createServer("101.1.1.40");
	createServer("101.1.1.41");
	//createServer("101.1.1.42");

	createServer("101.1.1.60");

	//createServer("101.1.1.80");
#endif

#ifdef ATLI_A
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
#endif

#ifdef ATLI_B
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.4");	
	createServer("101.1.1.41");
	createServer("101.1.1.60");	
#endif

#ifdef HENRY
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
#endif

#ifdef HENRYDOM
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
	createServer("101.1.1.6");
	createServer("101.1.1.7");
#endif

#ifdef HENRY_NEW
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
	createServer("101.1.1.6");
	createServer("101.1.1.7");
#endif

#ifdef HENRY_TEST
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
#endif

#ifdef HENRY_NEWRIG
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
#endif

#ifdef HENRY_POD
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
#endif

#ifdef HENRY_TWIN
	udpSocket->open(9931, "101.1.1.200");
	
	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
#endif

#ifdef LORNE
	//udpSocket->open(9931, "100.1.1.9");

	//createServer("100.1.1.6");
	//createServer("100.1.1.7");
	//createServer("100.1.1.8");
	//createServer("100.1.1.10");
	//createServer("100.1.1.11");
	//createServer("100.1.1.12");
	////createServer("100.1.1.13");

	udpSocket->open(9931, "103.1.1.101");

	createServer("103.1.1.1");
	createServer("103.1.1.2");
	createServer("103.1.1.3");
	createServer("103.1.1.4");
	createServer("103.1.1.5");
	createServer("103.1.1.6");
	createServer("103.1.1.7");
	createServer("103.1.1.8");
	createServer("103.1.1.9");
#endif

#ifdef JACOB
	udpSocket->open(9931, "104.1.1.101");

	createServer("104.1.1.1");
	createServer("104.1.1.2");
	createServer("104.1.1.3");
	createServer("104.1.1.4");
	createServer("104.1.1.5");
	createServer("104.1.1.6");
	createServer("104.1.1.7");
	createServer("104.1.1.8");
#endif

#ifdef JNHA
	udpSocket->open(9931, "201.1.1.101");

	createServer("201.1.1.1");
	createServer("201.1.1.2");
	createServer("201.1.1.3");
	createServer("201.1.1.4");
	createServer("201.1.1.5");
	createServer("201.1.1.6");
	createServer("201.1.1.7");
	createServer("201.1.1.8");
#endif

#ifdef JNHB
	udpSocket->open(9931, "202.1.1.101");

	createServer("202.1.1.1");
	createServer("202.1.1.2");
	createServer("202.1.1.3");
	createServer("202.1.1.4");
	createServer("202.1.1.5");
	createServer("202.1.1.6");
	createServer("202.1.1.7");
	createServer("202.1.1.8");
#endif

#ifdef DOM
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.5");
#endif

#ifdef HANS
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4"); // Full
	createServer("101.1.1.5"); // The new solo strings machine

	createServer("101.1.1.20");
	//createServer("101.1.1.21");

	//createServer("101.1.1.40");
	//createServer("101.1.1.41");
	//createServer("101.1.1.42");

	//createServer("101.1.1.60");

	//createServer("101.1.1.80");
#endif

#ifdef HANS2
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.21");

	createServer("101.1.1.40");
	createServer("101.1.1.41");
	createServer("101.1.1.42");

	createServer("101.1.1.60");

	createServer("101.1.1.80");

	createServer("101.1.1.100");
#endif

#ifdef BERGERAC
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.21");

	createServer("101.1.1.40");
	createServer("101.1.1.41");
	createServer("101.1.1.42");

	createServer("101.1.1.60");

	createServer("101.1.1.5");
#endif

#ifdef DOOLEY
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");

	createServer("101.1.1.20"); // Brass
	//createServer("101.1.1.21");

	createServer("101.1.1.40");
	createServer("101.1.1.41");
	//createServer("101.1.1.42"); // Percussion 3

	createServer("101.1.1.60");

	//createServer("101.1.1.80"); // Synth
#endif

#ifdef DOOLEY_A
	udpSocket->open(9931, "101.1.1.200");

	createServer("101.1.1.0");
	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
	createServer("101.1.1.20"); // Brass
#endif

#ifdef DOOLEY_B
	udpSocket->open(9931, "101.1.1.201");

	
	//createServer("101.1.1.21");
	createServer("101.1.1.60");

	createServer("101.1.1.40");
	createServer("101.1.1.41");
	createServer("101.1.1.42"); // Percussion 3

	

	//createServer("101.1.1.80"); // Synth
#endif

#ifdef STUDIOA
	udpSocket->open(9931, "101.1.1.101");

	createServer("101.1.1.1");
	createServer("101.1.1.2");
	createServer("101.1.1.3");
	createServer("101.1.1.4");
#endif

#ifdef TEST
	udpSocket->open(9931);

	createServer("10.17.0.167");
#endif



	if(!CsEngine::init()) {
		return 0;
	}

	

	//Sleep(100000);


	

	//m_networkAudioClient[0] = new NmNetworkAudioClient(this, "172.17.3.230");
	//m_networkAudioClient[1] = new NmNetworkAudioClient(this, "172.17.3.236");
	

	//
	csDebug("Before");

#ifdef JABLONSKY
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "01", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "02", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "03", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "04", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "05", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "06", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "07", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "08", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "09", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "10", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "11", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "12", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "13", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "14", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "15", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 116, "16", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef TREVOR
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Brass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Perc 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Perc 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Perc 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Perc 4", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef RAMIN_B
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Hi Perc", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Lo Perc", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "New Sampler 1a", CsSpeakerConfig::Surround40, -1)); // Metal
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Brass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "New Sampler 1b", CsSpeakerConfig::Surround40, -1)); // Tuned
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "New Sampler 2a", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "New Sampler 2b", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ZANELLI_A
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Full Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Solo Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Synth 1/2", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ZANELLI_B
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Brass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Percussion 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Percussion 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Percussion 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Metal/Synth Percussion", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Choir", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Extra", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef DOM

	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));

#endif

#ifdef HANS
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Choir", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Brass Short", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Percussion 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Percussion 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "Percussion 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "Synth Percussion 1/2", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "Synth 1/2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "Synth 3/4", CsSpeakerConfig::Surround40, -1));
#endif

	#ifdef HANS2
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Choir", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Brass Short", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Percussion 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Percussion 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "Percussion 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "Synth Percussion 1/2", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "Synth 1/2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "Synth 3/4", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "Woodwinds Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "Woodwinds Short", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef BERGERAC
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Choir", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Brass Short", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Percussion 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Percussion 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "Percussion 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "Synth Percussion 1/2", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "Synth 1/2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "Synth 3/4", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef DOOLEY
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Choir", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Brass Short", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Percussion 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Percussion 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "Percussion 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "Synth Percussion 1/2", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "Synth 1/2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "Synth 3/4", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef DOOLEY_A
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "FX/Other", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Brass Short Hi", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Brass Short Lo", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef DOOLEY_B
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Choir Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Choir Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Perc Lo", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Perc Mid", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Perc Hi", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "SmlAux", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef HENRY
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Full Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Solo Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Solo Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Brass Short", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef HENRYDOM
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Full Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Solo Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Solo Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Brass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "Full Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "Full Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "Solo Strings Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "Solo Strings Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "Brass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "Brass Short", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef HENRY_NEW
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef HENRY_TEST
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef TEST
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef HENRY_NEWRIG
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "01", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "02", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "03", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "04", CsSpeakerConfig::Surround40, -1));
#endif


#ifdef HENRY_POD
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "01", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "02", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "03", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "04", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "05", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "06", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "07", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "08", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "09", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "10", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "11", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "12", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "13", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "14", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "15", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 116, "16", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef HENRY_TWIN
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "01", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "02", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "03", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "04", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "05", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "06", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "07", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "08", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "09", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "10", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef PASADENA1
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef PASADENA2
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef STUDIO_C1
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef STUDIO_C2
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ANDREW
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 116, "", CsSpeakerConfig::Stereo, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ANDREW1
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 116, "", CsSpeakerConfig::Stereo, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ANDREW2
	executeRealTimeCommand(new CsRcCreateOutput(project(), 116, "", CsSpeakerConfig::Stereo, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef MR_XL
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ATLI_NEWNEW
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ATLI_NEW
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ATLI
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Solo Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Percussion", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Choir", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ATLI_A
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Solo Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Percussion", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Choir", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef ATLI_B
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Full Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Solo Strings", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Percussion", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Choir", CsSpeakerConfig::Surround40, -1));
#endif

#ifdef MAGICBOX
	executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 111, "", CsSpeakerConfig::Surround40, -1));
#endif

	//for(int i=0; i<8; i++) {
	//	executeRealTimeCommand(new CsRcCreateOutput(project(), 100+i, "Audio Output", CsSpeakerConfig::Surround40, -1));
	//}
	
	
	/*
	executeRealTimeCommand(new CsRcCreateBigOutput(project(), 100, "Violins/Violas", 16,0));
	executeRealTimeCommand(new CsRcCreateBigOutput(project(), 101, "Celli", 16,16));
	executeRealTimeCommand(new CsRcCreateBigOutput(project(), 102, "Bass", 16,32));
	executeRealTimeCommand(new CsRcCreateBigOutput(project(), 103, "Full Strings", 16, 48));
	*/

	//int outputs[5] = { 100, 100, 101, 102, 103 };

	/*executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violas", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Celli", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Basses", CsSpeakerConfig::Surround40, -1));*/


// Start of usual configuration --------------------------------------------------

	/*executeRealTimeCommand(new CsRcCreateOutput(project(), 100, "Violins/Violas Long", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Release", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Violins/Violas Short", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Celli/Bass Long", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 104, "Celli/Bass Release", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 105, "Celli/Bass Short", CsSpeakerConfig::Surround40, -1));

#ifdef HENRY
	executeRealTimeCommand(new CsRcCreateOutput(project(), 200, "Full Long", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins/Violas Release", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 202, "Full Short", CsSpeakerConfig::Surround40, -1));
#endif

	//executeRealTimeCommand(new CsRcCreateOutput(project(), 106, "Bass Long", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 107, "Bass Release", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 108, "Bass Short", CsSpeakerConfig::Surround40, -1));

#ifndef HENRY
	executeRealTimeCommand(new CsRcCreateOutput(project(), 109, "Full Strings", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 110, "Full Strings Release", CsSpeakerConfig::Surround40, -1));//Lorne
#endif
	

	executeRealTimeCommand(new CsRcCreateOutput(project(), 112, "Solo Strings Long", CsSpeakerConfig::Surround40, -1));
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 113, "Solo Strings Release", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 114, "Solo Strings Short", CsSpeakerConfig::Surround40, -1));


#ifndef ATLI
	//executeRealTimeCommand(new CsRcCreateOutput(project(), 115, "DDL Strings", CsSpeakerConfig::Surround40, -1));


	executeRealTimeCommand(new CsRcCreateOutput(project(), 120, "Perc Quad 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 121, "Perc Quad 2", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 122, "Perc Quad 3", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 123, "Perc Quad 4", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 130, "Synth 1", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 131, "Synth 2", CsSpeakerConfig::Surround40, -1));
#endif
*/
// End of usual configuration -------------------------------------------------- */

	
#ifdef STUDIOA

	executeRealTimeCommand(new CsRcCreateOutput(project(), 101, "Violins Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 102, "Violins Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 103, "Violins DDL", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 201, "Violas Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 202, "Violas Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 203, "Violas DDL", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 301, "Celli Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 302, "Celli Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 303, "Celli DDL", CsSpeakerConfig::Surround40, -1));

	executeRealTimeCommand(new CsRcCreateOutput(project(), 401, "Basses Short", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 402, "Basses Long", CsSpeakerConfig::Surround40, -1));
	executeRealTimeCommand(new CsRcCreateOutput(project(), 403, "Basses DDL", CsSpeakerConfig::Surround40, -1));

	//Studio
	int outputs[16] = {
		101, 102, 103, 101,
		201, 202, 203, 201,
		301, 302, 303, 304,
		401, 402, 403, 404
	};


#endif

	//*/

	

	// Studio
	/*int outputs[32] = {
		100, 101, 100, 101, // 2
		102, 103, 102, 103, // 3
		100, 100, 100, 100, // 4
		100, 100, 100, 100, // 5
		100, 100, 100, 100, // 6
		100, 100, 100, 100, // 7
		100, 100, 100, 100, // 8

	};*/

	// Lorne (Old)
	/*int outputs[32] = {
		100, 100, 102, 102, // 2
		100, 100, 102, 102, // 3
		103, 103, 105, 105, // 4
		103, 103, 105, 105, // 5
		109, 109, 109, 109, // 6
		112, 112, 114, 114, // 7
		116, 116, 116, 116, // 8

	};//*/

#ifdef MAGICBOX
	int outputs[40] = {
		100, 101, 110, 111,
		100, 101, 110, 111,
		100, 101, 110, 111,
		100, 101, 110, 111,
		100, 101, 110, 111,
		102, 103, 110, 111,
		104, 105, 103, 109,
		106, 107, 108, 109,
		106, 107, 108, 109,
		106, 107, 108, 109
	};
#endif

#ifdef JABLONSKY
	int outputs[32] = {
		101, 102, 101, 102,
		103, 104, 103, 104,
		105, 106, 105, 106,
		107, 108, 109, 110,
		107, 108, 111, 112,
		113, 114, 115, 116
	};
#endif

#ifdef TREVOR
	int outputs[32] = {
		100, 101, 100, 101,
		100, 101, 100, 101,
		100, 101, 100, 101,
		100, 101, 100, 101,
		102, 103, 102, 103,
		104, 105, 106, 107
	};
#endif
	
#ifdef RAMIN_B
	int outputs[20] = {
		100, 101, 100, 101, // 100, 101, 102, 105,
		100, 101, 100, 101, // 100, 101, 102, 105,
		103, 104, 103, 104, // 103, 104, 105, 105
		102, 105, 102, 105,
		106, 107, 106, 107
	};
#endif

#ifdef ZANELLI_A
	int outputs[24] = {
		100, 101, 100, 101,
		100, 101, 100, 101,
		102, 103, 102, 103, 
		102, 103, 106, 103,
		104, 105, 106, 106,
		107, 107, 107, 107
	};
#endif

#ifdef ZANELLI_B
	int outputs[24] = {
		100, 101, 100, 107,
		100, 101, 100, 107,
		102, 103, 104, 105,
		102, 103, 104, 105,
		102, 103, 104, 105,
		106, 106, 106, 107
	};
#endif

#ifdef MR_XL
	int outputs[28] = {
		100, 114, 104, 100,
		101, 115, 104, 100,
		102, 113, 104, 100,
		103, 103, 104, 100,
		105, 105, 106, 106,
		107, 107, 108, 108,
		109, 110, 111, 112
	};
#endif

#ifdef PASADENA1
	int outputs[12] = {
		100, 101, 100, 101,
		102, 103, 106, 103,
		104, 105, 107, 105
	};
#endif

#ifdef PASADENA2
	int outputs[12] = {
		100, 101, 106, 107,
		102, 103, 104, 105,
		102, 103, 104, 105
	};
#endif

#ifdef STUDIO_C1
	int outputs[20] = {
		100, 101, 102, 103,
		100, 101, 102, 103,
		100, 101, 102, 103,
		104, 105, 102, 103,
		108, 109, 110, 111
	};
#endif

#ifdef STUDIO_C2
	int outputs[16] = {
		100, 101, 102, 103,
		104, 105, 106, 107,
		108, 109, 110, 111,
		112, 113, 114, 115
	};
#endif

#ifdef ANDREW
	int outputs[20] = {
		100, 101, 102, 103,
		100, 101, 102, 103,
		100, 101, 102, 103,
		104, 105, 106, 107,
		108, 109, 110, 111
	};
#endif

#ifdef ANDREW1
	int outputs[12] = {
		100, 101, 102, 103,
		100, 101, 102, 103,
		100, 101, 102, 103
	};
#endif

#ifdef ANDREW2
	int outputs[12] = {
		104, 105, 102, 103,
		104, 105, 102, 103,
		110, 111, 114, 115
	};
#endif

#ifdef ATLI_NEWNEW
	int outputs[16] = {
		100, 101, 102, 103,
		100, 101, 102, 103,
		104, 105, 106, 107,
		108, 109, 110, 111
	};
#endif

#ifdef ATLI_NEW
	int outputs[20] = {
		100, 101, 100, 100,
		100, 101, 100, 100,
		100, 101, 100, 100,
		102, 103, 100, 100,
		104, 105, 100, 100
	};
#endif

#ifdef ATLI
	// Test
	int outputs[32] = {
		100, 101, 100, 100,
		100, 101, 100, 100,
		102, 103, 100, 100,
		102, 103, 100, 100,
		104, 105, 100, 100,
		106, 106, 106, 106,
		107, 107, 107, 107
	};//*/
#endif

#ifdef ATLI_A
	// Test
	int outputs[16] = {
		100, 101, 100, 100,
		100, 101, 100, 100,
		102, 103, 100, 100,
		102, 103, 100, 100
	};//*/
#endif

#ifdef ATLI_B
	// Test
	int outputs[12] = {
		104, 105, 100, 100,
		106, 106, 106, 106,
		107, 107, 107, 107
	};//*/
#endif

#ifdef OLD
	// Normal (New Batman)
	/*int outputs[36] = {
		100, 115, 102, 102, // 2 (Violins)
		100, 115, 102, 102, // 3 (Violas)
		103, 115, 105, 105, // 4 (Celli)
		103, 115, 105, 105, // 5 (Basses)
		109, 115, 109, 109, // 6 (Full)
		112, 115, 114, 114, // 7 (Solo)
		120, 121, 122, 123, // 8 (Perc)
		130, 131, 130, 130, // 9 (Synth)
		120, 121, 122, 123 // 10 (Other)

	};//*/

	// Henry (MVP)
	/*int outputs[24] = {
		100, 102, 200, 202, // 2 (Violins)
		100, 102, 200, 202, // 3 (Violas)
		103, 105, 200, 202, // 4 (Celli)
		103, 105, 200, 202, // 5 (Basses)
		200, 200, 200, 200, // 6 (Full)
		112, 112, 114, 114 // 7 (Solo)
		//120, 121, 122, 123, // 8 (Perc)
		//130, 131, 130, 130, // 9 (Synth)
		//120, 121, 122, 123 // 10 (Other)

	};//*/
	
#endif

	/*int outputs[21] = {
		100, 101, 102,
		100, 101, 102,
		103, 104, 105,
		106, 107, 108,
		109, 110, 111,
		112, 113, 114,
		112, 113, 114
	};*/

#ifdef HANS
	// Hans - Angels and Demons
	//int outputs[48] = {
	int outputs[28] = {
		100, 101, 102, 103, //  1 (Violins) 0
		100, 101, 102, 103, //  2 (Violas) 1
		100, 101, 102, 103, //  3 (Celli) 2
		100, 101, 102, 103, //  4 (Basses) 3
		100, 101, 102, 103, //  5 (Full/Solo) 4
		104, 105, 102, 103, //  5 (Full/Solo) // The new solo strings machine 5
		106, 107, 108, 109, //  6 (Brass 1) 20
		//106, 107, 106, 106, //  7 (Brass 2)
		//108, 109, 110, 111, //  8 (Percussion 1)
		//108, 109, 110, 111, //  9 (Percussion 2)
		//108, 109, 110, 111, // 10 (Percussion 3)
		//105, 105, 105, 105, // 11 (Choir)
		//112, 113, 112, 112  // 12 (Synth)

	};//*/
#endif

#ifdef HANS2
	// Hans - Angels and Demons
	int outputs[28] = {
		112, 113, 102, 103, //  7 (Brass 2)
		104, 105, 102, 103, //  8 (Percussion 1)
		104, 105, 102, 103, //  9 (Percussion 2)
		104, 105, 102, 103, // 10 (Percussion 3)
		110, 111, 102, 103, // 11 (Choir)
		100, 101, 102, 103,  // 12 (Synth)
		108, 109, 114, 115  // 13 (Woodwinds)
	};//*/
#endif

#ifdef BERGERAC
	// Hans - Angels and Demons
	int outputs[24] = {
		106, 107, 106, 106, //  7 (Brass 2)
		108, 109, 110, 111, //  8 (Percussion 1)
		108, 109, 110, 111, //  9 (Percussion 2)
		108, 109, 110, 111, // 10 (Percussion 3)
		105, 105, 105, 105, // 11 (Choir)
		104, 100, 102, 104  // 12 (New solo strings)

	};//*/
#endif

	#ifdef DOOLEY
	// Hans - Angels and Demons
	int outputs[4] = {

		108, 109, 110, 111 //  8 (Percussion 1)

	};//*/
#endif

#ifdef DOOLEY_A
	int outputs[24] = {
		100, 101, 104, 104,
		100, 101, 104, 104,
		102, 103, 104, 104,
		102, 103, 104, 104,
		100, 101, 102, 103,

		105, 106, 107, 107
	};
#endif

#ifdef DOOLEY_B
	int outputs[16] = {
		100, 101, 100, 101,
		102, 103, 104, 105,
		102, 103, 104, 105,
		102, 103, 104, 105,
	};
#endif

#ifdef HENRY
	int outputs[28] = {
		100, 101, 102, 103, //  1 (Violins)
		102, 103, 104, 105, //  2 (Violas)
		104, 105, 106, 107, //  3 (Celli)
		106, 107, 108, 109, //  4 (Basses)
	};//*/
#endif

#ifdef HENRYDOM
	int outputs[32] = {
		100, 101, 102, 103, 
		102, 103, 104, 105, 
		104, 105, 106, 107, 
		108, 109, 110, 111,

		106, 107, 106, 107, 
		106, 107, 106, 107, 
		106, 107, 112, 113, 
		113, 114, 115, 116, 
	};//*/
#endif

#ifdef HENRY_NEW
	int outputs[32] = {
		101, 103, 109, 110,
		101, 103, 109, 110,
		102, 104, 109, 110,
		102, 104, 109, 110,
		101, 102, 109, 110,
		105, 106, 101, 102,
		107, 108, 109, 110,
		109, 110, 111, 112
	};
#endif

#ifdef HENRY_NEWRIG
	int outputs[24] = {
		101, 102, 103, 104, //  1 (Vln/Vla)
		101, 102, 103, 104, //  2 (Vc/Vbs)
		101, 102, 103, 104, //  3 (Appass/Other)
		101, 102, 105, 106, //  4 (Solo/Other)
		107, 108, 109, 110, //  5 (Brass)
		107, 108, 109, 110, //  6 (Misc)
	};//*/
#endif

#ifdef HENRY_POD
	int outputs[16] = {
		101, 102, 103, 104, //  
		105, 106, 107, 108, //  
		109, 110, 111, 112, //  
		113, 114, 115, 116, //  
	};//*/
#endif

#ifdef HENRY_TWIN
	int outputs[16] = {
		101, 102, 103, 104, //  
		103, 104, 105, 106, //  
		105, 106, 107, 108, //  
		107, 108, 109, 110, //  
	};//*/
#endif

#ifdef DOM
	int outputs[24] = {
		101, 102, 103, 104,
		101, 102, 103, 104,
		101, 102, 101, 102,
		105, 106, 105, 106,
		107, 108, 107, 108,
		109, 110, 109, 110
	};
#endif

#ifdef HENRY_TEST
	int outputs[20] = {
		101, 102, 103, 104,
		101, 102, 103, 104,
		101, 102, 105, 106,
		107, 108, 109, 110,
		107, 108, 109, 110
	};
#endif

#ifdef TEST
	int outputs[20] = {
		101, 102, 103, 104
	};
#endif

	for(int i=0; i<numServers(); i++) {
		//executeRealTimeCommand(new CsRcCreateChannel(project(), 1, 16, 0, audioDevice()->bufferSize(), audioDevice()->sampleRate()));
		
		//executeRealTimeCommand(new CsRcCreateChannel(project(), networkAudioClientList[i], audioDevice()->bufferSize(), audioDevice()->sampleRate()));//OLD
		executeRealTimeCommand(new CsRcCreateChannelWithGenerator(project(), networkAudioClientList[i], audioDevice()->bufferSize(), audioDevice()->sampleRate()));
		
		//executeRealTimeCommand(new CsRcSetChannelDirectStreamOutput(project(), i, true));
		executeRealTimeCommand(new CsRcSetChannelAudioOutputBusId(project(), i, 0, outputs[(i*4)+0]));
		executeRealTimeCommand(new CsRcSetChannelAudioOutputBusId(project(), i, 1, outputs[(i*4)+1]));
		executeRealTimeCommand(new CsRcSetChannelAudioOutputBusId(project(), i, 2, outputs[(i*4)+2]));
		executeRealTimeCommand(new CsRcSetChannelAudioOutputBusId(project(), i, 3, outputs[(i*4)+3]));
		//executeRealTimeCommand(new CsRcSetChannelAudioTool(project(), i, networkAudioClientList[i]));
	}
	


	/*executeRealTimeCommand(new CsRcSetChannelDirectStreamOutputId(project(), 2, 0, 16));
	executeRealTimeCommand(new CsRcSetChannelDirectStreamOutputId(project(), 2, 1, 17));
	executeRealTimeCommand(new CsRcSetChannelDirectStreamOutputId(project(), 2, 2, 18));
	executeRealTimeCommand(new CsRcSetChannelDirectStreamOutputId(project(), 2, 3, 19));*/

	csDebug("After");

	if(!threadStarted) { 
		if(udpSocket->isConnected()) {
		startThread();
		setPriority(THREAD_PRIORITY_TIME_CRITICAL);
		//setPriority(THREAD_PRIORITY_HIGHEST);
	}
		threadStarted = true;
	}


	return 1;
}

int NmEngine::exit(void)
{
	CsEngine::exit();

	return 1;
}

void NmEngine::startingAudio(long bufferSize, double sampleRate)
{
	csDebug("NmEngine::startingAudio()");

	CsEngine::startingAudio(bufferSize, sampleRate); csDebug("1");

	

	//if(receiveBuffer) {
	//	free(receiveBuffer);
	//	receiveBuffer = 0;
	//} CSDEBUG("2");
	//receiveBuffer = (char*)malloc(4*(bufferSize*numOutputsPerServer)+24); CSDEBUG("3");

	sendResetToServers(bufferSize, sampleRate); csDebug("4");
	
	csDebug("EXIT: NmEngine::startingAudio()");

	

	
}

void NmEngine::preProcessAudio(void)
{
	static int counter = 0;
	static int client = 0;
	//waitForProcess->set();
	//for(int k=0; k<numServers(); k++) {
	//	requestNextBlockFromServer(networkAudioClientList[k]);
	//}
	waitForProcess->set();

	/*if(counter>99) {
		if(client<numServers()) {
			networkAudioClientList[client]->setActive(true);
		}
		client++;
		counter = 0;
	}

	counter++;*/

}

void NmEngine::createServer(char *ipAddress)
{
	aLog("***** NmEngine::createServer(%s)", ipAddress);
	NmNetworkAudioClient *client = new NmNetworkAudioClient(this, ipAddress);
	networkAudioClientList.push_back(client);
}

void NmEngine::requestNextBlockFromServer(NmNetworkAudioClient *fromClient)
{
	CsLock lock(&csRequest);

	char buffer[24];
	buildHeader(buffer, 101);

	for(int i=0; i<numServers(); i++) {
		if(fromClient == networkAudioClientList[i]) {
			//csDebug("Sending request to server #%d", i);
			udpSocket->sendBuffer(networkAudioClientList[i]->serverIpAddress(), buffer, 24);
			break;
		}
	}
}


/*
void NmEngine::threadProcess(void)
{
	for(;;) {
		waitForProcess->wait();
		unsigned long address = 0;
		udpSocket->receiveBuffer(receiveBuffer, 4*(audioDevice()->bufferSize()*numOutputsPerServer)+24, &address);

			for(int i=0; i<numServers(); i++) {
				//csDebug("Packet recevied (%ud,%ud)", address , networkAudioClientList[i]->serverAddress());
				if(address == networkAudioClientList[i]->serverAddress()) {
					//csDebug("Packet received from server #%d", i);
					//strcpy(m_client[i]->receiveBuffer(), receiveBuffer);
					char *rb = networkAudioClientList[i]->receiveBuffer();
					//if(rb) {
						for(int j=0; j<(4*(audioDevice()->bufferSize()*numOutputsPerServer)+24); j++) {
							rb[j] = receiveBuffer[j];

						}
					//}
					

					networkAudioClientList[i]->signalPacketReady();
					break;
				}
			//}
			}
	}
}//*/

void NmEngine::threadProcess(void)
{
	int dropped[128];
	int initPackets[128];
	for(int i=0; i<128; i++) {
		dropped[i] = 0;
		initPackets[i] = 0;
	}

	int waitTime = 6000;
	for(;;) {

		//csDebug("---Thread iteration---");
		//if(receiveBuffer) {

		waitForProcess->wait();
		waitForProcess->reset();

		//int t1 = timeGetTime();

		for(int k=0; k<numServers(); k++) {
			if(networkAudioClientList[k]->isActive()) {

				int t1 = timeGetTime();
				requestNextBlockFromServer(networkAudioClientList[k]);

				if(udpSocket->isSomethingThere(waitTime)) {
			
					initPackets[k]++;
					if(initPackets[k]>99) {
						networkAudioClientList[k]->setAudioActive(true);
					}
					unsigned long address = 0;
					
					if(!udpSocket->receiveBuffer(networkAudioClientList[k]->receiveBuffer(), 4*(audioDevice()->bufferSize()*numOutputsPerServer)+24, &address)) {
						networkAudioClientList[k]->setActive(false);
					} else {
						networkAudioClientList[k]->signalPacketReady();
						//csDebug("[%d] %dms", k, timeGetTime()-t1);
						dropped[k] = 0;
					}
				} else {
					if(networkAudioClientList[k]->isAudioActive()) {
						dropped[k]++;
						csDebug("Dropped [%d] %d", k+1, dropped[k]);
						
						if(dropped[k]>99) {
							networkAudioClientList[k]->setActive(false);
						}
					}
				}

				udpSocket->somethingThereCleanup();
			}


		}
		

		//csDebug("Network time = %d", timeGetTime()-t1);

	}
}//*/

void NmEngine::buildHeader(char *buffer, char message)
{
	for(int i=0; i<24; i++) {
		buffer[i] = 1;
	}

	buffer[0] = message;

	short bs = (short)audioDevice()->bufferSize();
	CsUdpSocket::shortToChar(&bs, 1, buffer, 11);

	short fs = (short)numOutputsPerServer;
	CsUdpSocket::shortToChar(&fs, 1, buffer, 13);
}

void NmEngine::sendResetToServers(int bufferSize, double sampleRate)
{
	csDebug("*************************> NmEngine::sendResetToServers() %d", numServers());
	char buffer[24];
	buildHeader(buffer, 100);	

	for(int i=0; i<numServers(); i++) {
		csDebug("*************************> RESET [%d]", i);
		udpSocket->sendBuffer(networkAudioClientList[i]->serverIpAddress(), buffer, 24);
	}
}