/*
 *  DOM.cpp
 *  Xox
 *
 *  Created by Brant Sears on Thu Jun 26 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */
#include <iostream>
using std::cout;
using std::endl;

#include "DOM.h"

namespace Xox {



DOM::DOM() : mBaseElement(NULL)
{

}

DOM::~DOM()
{
    
    if (mBaseElement) {
	delete mBaseElement;
    }
}

const XMLElement * DOM::GetBaseElement()
{
    return mBaseElement;
}

}