    // -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#ifndef __XMLMODLOADER__
#define __XMLMODLOADER__

#include "ymm-types.h" 
#include "behavior.h"
#include "manifestloader.h"

#include <map>

//namespace ymm {


/**
 * Class XmlBehModelLoader  
 */
class XmlBehModelLoader : public BehModelLoader {

public:
    XmlBehModelLoader(const char* szFileName);
    virtual ~XmlBehModelLoader();
    virtual bool init(void);

    virtual void reset(void) { }
    virtual void fini(void) { }

    virtual std::vector<Behavior>& getBehaviors(void) {
        return behaviors;
    }

    virtual std::vector<BehaviorGroup>& getBehaviorGroups(void) {
        return groups;
    }
    
protected:

private:
    string strFileName;
    std::vector<Behavior> behaviors;
    std::vector<BehaviorGroup> groups;
    map<string, string> variables;

private:
    bool parsXml(const char* szFile);
    const char* expandVariables(const char* expr);
    void searchReplace(std::string& str, const std::string& oldStr,
                       const std::string& newStr);
};


 
//}

#endif //__XMLMODLOADER__
