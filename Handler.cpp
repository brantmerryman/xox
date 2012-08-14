/*
 *  Handler.cpp
 *  Xox
 *
 *  Created by Brant Sears on Tue May 20 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */

#include "Handler.h"

namespace Xox {

void Handler::startDoc() const
{
}

void Handler::endDoc() const
{
}

void Handler::endDoc(DOM * pDom) const
{
}

void Handler::startElement(const char *tag, map<string, string> & attributes)
{
}

void Handler::elementPCData(const char * tag, const char * pcData)
{

}

void Handler::endElement(const char * tag)
{

}

void Handler::error(XoxException & exception) const
{
    throw exception;
}

}