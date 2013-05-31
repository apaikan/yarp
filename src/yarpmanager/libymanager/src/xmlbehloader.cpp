/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <map>
#include <tinyxml.h>
#include "utility.h"
#include "ymm-dir.h"
#include "xmlbehloader.h"

using namespace std;


/**
 * load only one module indicated by its xml file name 
 */
XmlBehModelLoader::XmlBehModelLoader(const char* szFileName)
{
    if(szFileName)
        strFileName = szFileName;   
}


XmlBehModelLoader::~XmlBehModelLoader() { }


bool XmlBehModelLoader::init(void)
{
    behaviors.clear();
    groups.clear();
    variables.clear();
    if(strFileName.length() == 0)
        return false;

    return parsXml(strFileName.c_str());
}


bool XmlBehModelLoader::parsXml(const char* szFile)
{

    ErrorLogger* logger  = ErrorLogger::Instance();
    
    TiXmlDocument doc(szFile);
    if(!doc.LoadFile()) 
    {
        OSTRINGSTREAM err;
        err<<"Syntax error while loading "<<szFile<<" at line "\
           <<doc.ErrorRow()<<": ";
        err<<doc.ErrorDesc();
        logger->addError(err);
        return false;
    }
    
    /* retrieving root module */
    TiXmlElement *root = doc.RootElement();
    if(!root)    
    {
        OSTRINGSTREAM err;
        err<<"Syntax error while loading "<<szFile<<" . ";
        err<<"No root element.";
        logger->addError(err);
        return false;
    }

    if(!compareString(root->Value(), "model"))
    {        
        OSTRINGSTREAM msg;
        msg<<szFile<<" is not a behavioral description.";
        logger->addWarning(msg);        
        return false;
    }
 
    /* retrieving variables*/
    for(TiXmlElement* def = root->FirstChildElement(); def;
        def = def->NextSiblingElement())
    {
        if(compareString(def->Value(), "define"))
        {
            if(def->Attribute("name") && def->GetText())
               variables[string("${") + string(def->Attribute("name")) + string("}")] = def->GetText();
            else
            {
                OSTRINGSTREAM war;
                war<<"Unrecognized tag from "<<szFile<<" at line "<<def->Row()<<".";
                logger->addWarning(war);                                
            }
        }
    }

    /* retrieving behvaiors*/
    for(TiXmlElement* beh = root->FirstChildElement(); beh;
            beh = beh->NextSiblingElement())
    {
        if(compareString(beh->Value(), "behavior"))
        {
            if(beh->Attribute("name"))
            {
                Behavior behavior(beh->Attribute("name"));
                
                for(TiXmlElement* element = beh->FirstChildElement(); element;
                    element = element->NextSiblingElement())
                {
                    if(compareString(element->Value(), "condition"))
                        behavior.setCondition(expandVariables(element->GetText()));
                   
                   
                    if(compareString(element->Value(), "configuration"))
                    {
                        if(element->Attribute("at"))
                        {
                            Configuration config;
                            config.setAt(expandVariables(element->Attribute("at")));
                            config.setPort(expandVariables(element->GetText()));
                            if(element->Attribute("blocker"))
                                config.setBlocker(expandVariables(element->Attribute("blocker")));                         
                            behavior.addConfiguration(config);
                        }
                        else
                        {
                            OSTRINGSTREAM war;
                            war<<"Ignoring wrong configuration tag from "<<szFile<<" at line "<<element->Row()<<".";
                            logger->addWarning(war);
                        }
                    }
                    
                    if(compareString(element->Value(), "inhibition"))
                        behavior.addInhibition(expandVariables(element->GetText()));
                }
                // pushing the behavior to the list
                behaviors.push_back(behavior);
            }
            else
            {
                OSTRINGSTREAM war;
                war<<"Ignoring unnamed behavior from "<<szFile<<" at line "<<beh->Row()<<".";
                logger->addWarning(war);
            }
        }       
    } 

    /* retrieving groups*/
    for(TiXmlElement* grp = root->FirstChildElement(); grp;
            grp = grp->NextSiblingElement())
    {
        if(compareString(grp->Value(), "group"))
        {
            if(grp->Attribute("name"))
            {
                BehaviorGroup group(grp->Attribute("name"));
                
                for(TiXmlElement* element = grp->FirstChildElement(); element;
                    element = element->NextSiblingElement())
                {
                    if(compareString(element->Value(), "condition"))
                        group.setCondition(expandVariables(element->GetText()));                                      
                   
                    if(compareString(element->Value(), "behavior"))
                        group.addBehavior(expandVariables(element->GetText()));

                    if(compareString(element->Value(), "group"))
                        group.addBehaviorGroup(expandVariables(element->GetText()));
                }

                // pushing the group to the list
                groups.push_back(group);
            }
            else
            {
                OSTRINGSTREAM war;
                war<<"Ignoring unnamed group from "<<szFile<<" at line "<<grp->Row()<<".";
                logger->addWarning(war);
            }
        }       
    } 

    return true;
}

const char* XmlBehModelLoader::expandVariables(const char* expr) 
{  
    if(!expr)
        return NULL;

    ErrorLogger* logger  = ErrorLogger::Instance();
    
    string str = expr;   
    for(map<string,string>::iterator iter = variables.begin(); iter != variables.end(); iter++)
        searchReplace(str, iter->first, iter->second);
    
    if(str.find('$') != string::npos)
    {
        OSTRINGSTREAM war;
        war<<"Unknown variables in "<<expr;
        logger->addWarning(war);
        return NULL;
    }

    return str.c_str();
}

void XmlBehModelLoader::searchReplace(string& str, const string& oldStr, const string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}



