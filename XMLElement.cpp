/*
 *  XMLElement.cpp
 *  Xox
 *
 *  Created by Brant Sears on Thu Jun 26 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */

#include <iostream>
using std::cout;
using std::endl;

#include "XMLElement.h"


namespace Xox {

XMLElement::XMLElement(const char * tag, const map<string, string> & attributes) : mTag(tag), mAttributes(attributes)
{

}

XMLElement::XMLElement() : mParent(NULL)
{

}

XMLElement::~XMLElement()
{
    std::vector<XMLElement*>::iterator iter;
    for (iter = mKids.begin(); iter != mKids.end(); ++iter) {
	delete *iter;
    }
}


}