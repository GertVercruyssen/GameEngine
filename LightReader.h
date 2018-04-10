#pragma once


#ifdef _UNICODE
	#define XML_UNICODE_WCHAR_T
#endif
#include "TextReader.h"
#include "expatpp.h"
#include <vector>
#include "LightManager.h"
using namespace std;

#include "util.h"

struct SpotLight;
/**
* Reads the lights in an xml file.
* @author Koen Samyn
*/
class LightReader: public TextReader,expatpp
{
public:
	/**
	* Creates a new LightReader object.
	*/
	LightReader(tstring filename);
	/**
	* Destroys the LightReader object.
	*/
	virtual ~LightReader(void);
	/**
	* Reads the xml file that contains the lights.
	*/
	bool Read();
	/**
	* Returns the spotlights that were read.
	* @return a reference to the spotlight vector.
	*/
	vector<SpotLight*>& GetLights(){
		return m_SpotLights;
	}
	/**
	* This method is called when a new xml element is encountered.
	* @param name the name of the element.
	* @param atts the attributes for the element.
	*/
	void startElement(const XML_Char *name, const XML_Char **atts);
	/**
	* This method is called when an xml element ends.
	* @param name the name of the element.
	*/
	void endElement(const XML_Char* name);
	/**
	* This method is called for the contents of the element.
	* @param s the contents of an element.
	* @param len the length of this element.
	*/
	void charData(const XML_Char *s, int len);

	void startNamespace(const XML_Char* /* prefix */, const XML_Char* /* uri */){}

	void endNamespace(const XML_Char*){}

	void processingInstruction(const XML_Char*, const XML_Char*){}
	
	void defaultHandler(const XML_Char*, int){}

	void unparsedEntityDecl(const XML_Char*, const XML_Char*, const XML_Char*, const XML_Char*, const XML_Char*){}

	void notationDecl(const XML_Char*, const XML_Char*, const XML_Char*, const XML_Char*){}
private:
	vector<SpotLight*> m_SpotLights;
};
