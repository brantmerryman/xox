//
//  main.cpp
//  xox-test
//
//  Created by Brant Sears on 3/9/12.
//  Copyright (c) 2012 ChristyBrantCo, LLC. All rights reserved.
//

#include <stdio.h>
#include "XoxParser.h"
#include "XoxException.h"
#include "XMLElement.h"
#include "DOM.h"

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using namespace Xox;


void recursivePrintElement(XMLElement * e)
{
    static int indent = 0;
    ++indent;
    for (int i=0;i<indent;++i) {
        printf("     ");
    }
    printf("%s ", e->GetTag().c_str());
    for (std::map<string, string>::iterator iter =  e->GetAttributes().begin(); iter != e->GetAttributes().end(); ++iter) {
        const string & attName = iter->first;
        string & attValue = iter->second;
        printf("%s=\"%s\" ", attName.c_str(), attValue.c_str());
    }
    printf("\n");
    for (std::vector<XMLElement*>::iterator iter2 = e->GetKids().begin(); iter2 != e->GetKids().end(); ++iter2) {
        XMLElement * kiddo = *iter2;
        recursivePrintElement(kiddo);
    }
    
    --indent;
}

void showUsage()
{
    printf("usage: xox-test <filename>\n");
}

int main(int argc, const char * argv[])
{
    // find the file path
    
    if (argc != 2) {
        showUsage();
        return 0;
    }
    const char * theFile = argv[1];
    
    printf("%s\n", theFile);
    
    XoxParser parser;
    parser.SetCreateDOM(true);
    XMLElement * element = NULL;
    
    try {
        DOM * d = parser.parseFile(theFile);
        element = (XMLElement *)d->GetBaseElement();
    } catch (XoxException & e) {
        printf("An exception occurred");
        return 1;
    }
    
    if (element) {
        recursivePrintElement(element);
    }

    
    return 0;
}

