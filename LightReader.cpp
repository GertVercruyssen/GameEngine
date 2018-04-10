#include "StdAfx.h"
#include "LightReader.h"
#include "TextReader.h"
#include "io.h"
#include "D3DX10Core.h"
#include "SpotLight.h"

LightReader::LightReader(tstring filename):TextReader(filename)
{
}

LightReader::~LightReader(void)
{
}

bool LightReader::Read()
{
	tstring file = GetFileName();
	tifstream bestand(file.c_str());	// open het bestand

	if (!bestand) return false;	// exit als het bestand niet gevonden werd
	OutputDebugString(_T("bestand is open"));

	// lees het bestand in
	tstring sRegel;
	tstring sXMLText;
	while (!bestand.eof())
	{
		getline(bestand, sRegel);
		sXMLText += sRegel;
	}

	// parse de text
	//XMLParser parser(bedrijfsmap);
	skipWhiteSpace(sXMLText);
	XML_Parse(sXMLText, true);

	// sluit het bestand
	bestand.close();
	return true;
}

void LightReader::startElement(const XML_Char *name, const XML_Char **atts)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
	float range;
	D3DXCOLOR color;
	D3DXVECTOR3 attenuation;
	float spotpower; 

	tstring naam = name;
	if (naam == _T("spotlight"))
	{
		tstring tekst = atts[1];
		
		ParseFloat3(tekst, &pos.x, &pos.y, &pos.z); //position
		tekst = atts[3];
		ParseFloat3(tekst, &dir.x, &dir.y, &dir.z );//direction
		tekst = atts[5];
		range = ParseFloat(tekst);
		tekst = atts[7];
		ParseFloat3(tekst, &color.r, &color.g, &color.b);
		color.a = 1;
		tekst = atts[9];
		ParseFloat3(tekst, &attenuation.x, &attenuation.y, &attenuation.z);
		tekst = atts[11];
		spotpower = ParseFloat(tekst);
		SpotLight* nieuw = new SpotLight(pos, dir, attenuation, color, range, spotpower);
		LightManager::GetSingleton().AddLight(nieuw);
		m_SpotLights.push_back(nieuw);

	}
}

void LightReader::endElement(const XML_Char* name)
{
}

void LightReader::charData(const XML_Char *s, int len)
{
}
