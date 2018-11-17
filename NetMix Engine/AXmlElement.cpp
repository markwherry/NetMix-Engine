#include "AXmlElement.h"

#include "ALog.h"

namespace Atom {

AXmlAttribute::AXmlAttribute(AString name, AString value)
: m_name(name)
, m_value(value)
{
}

AXmlElement::AXmlElement(AString tag)
: m_tag(tag)
{
}

void AXmlElement::setAttribute(AString name, AString value)
{
	AXmlAttribute *attribute = 0;

	for(int i=0; i<m_attributes.size(); i++) {
		if(m_attributes[i]->name().isEqualTo(name)) {
			m_attributes[i]->setValue(value);
		}
	}

	if(!attribute) {
		attribute = new AXmlAttribute(name, value);
		m_attributes.append(attribute);
	}
}

void AXmlElement::setAttribute(AString name, int value)
{
	setAttribute(name, AString::number(value));
}

AString AXmlElement::attribute(AString name, AString defaultValue)
{
	for(int i=0; i<m_attributes.size(); i++) {
		//aDebug("Find attribute [%s,%s]", name.toChar(), m_attributes[i]->name().toChar());
		if(m_attributes[i]->name().isEqualTo(name)) {
			return m_attributes[i]->value();
		}
	}

	return defaultValue;
}

bool AXmlElement::hasAttribute(AString name)
{
	for(int i=0; i<m_attributes.size(); i++) {
		if(m_attributes[i]->name().isEqualTo(name)) {
			return true;
		}
	}

	return false;
}

void AXmlElement::addChildElement(AXmlElement *element)
{
	m_elements.append(element);
}

}