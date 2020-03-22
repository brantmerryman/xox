/*
 *  XoxParser.cpp
 *  XMLParseTest
 *
 *  Created by Brant Sears on 3/31/05.
 *  Copyright (c) 2005 Brant Sears. All rights reserved.
 *
 */

#include "XoxParser.h"

#define _BACKWARD_BACKWARD_WARNING_H

#include <fstream>
using std::ifstream;

#include <strstream>
using std::ostrstream;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <cassert>

#include "XoxException.h"

namespace Xox {

XoxParser::XoxParser() : fDOM(true), mTag(NULL), mHandler(NULL), mDOM(NULL)
{

}

XoxParser::XoxParser(Handler * theHandler) : fDOM(true), mTag(NULL), mHandler(theHandler), mDOM(NULL)
{

}

XoxParser::~XoxParser()
{
    if (mTag) { free(mTag); }
}

void XoxParser::SetCreateDOM(bool fCreateDOM)
{
    fDOM = fCreateDOM;
}

const char * XoxParser::description()
{
    return "No description yet.";
}

void XoxParser::SetHandler(Handler * theHandler)
{
    mHandler = theHandler;
}

DOM * XoxParser::parseFile(const char *fileName)
{
    ifstream myFile(fileName);
    ostringstream contents;
    
    contents << myFile.rdbuf();

    return parse(contents.str().c_str());
}

DOM * XoxParser::parse(const char *contents)
{
    char * myLine;
    int s, rs, f;
    int rc = 0;
    int mode; // 0 = nothing special, 1 = PCData, 2 = inside attribute
    bool fEscaped = false;
    size_t len = strlen(contents);
    s = 0;

    bool fStart, fFin;
    mode = 0;
    if (fDOM) {
	mDOM = new DOM();
    }
    else {
	mDOM = NULL;
    }
    if (mHandler) {
	mHandler->startDoc();
    }
    
    while (s < len) {
	for (fStart=false, rs = s;;++s) {
	    switch(contents[s])
	    {
		case '<':
		    // means we started a nice xml line.
		    mode = 0;
		    fStart = true;
		    break;
		case ' ':
		case '	':
		case '\n':
		case '\r':
		    break;
		default:
		    mode = 1;
		    fStart = true;
		    s = rs;
		    break;
	    }
	    if (fStart) {
		break;
	    }
	}
	fEscaped = false;
	for (f=s,fFin=false;;++f) {
	    if (f > len ) 
		goto DONE;
	    switch(contents[f])
	    {
		case '\\':
		    fEscaped = !fEscaped;
		    break;
		case '>':
		    if (0==mode && !fEscaped) {
			fFin = true;
		    } else if (fEscaped) {
			fEscaped =false;
		    }
		    break;
		case '"':
		    if (fEscaped) {
			fEscaped = false;
		    } else if (0==mode) {
			mode = 2;
		    } else if (2==mode) {
			mode = 0;
		    }
		    break;
		case '<':
		    if (1==mode) {
			--f;
			fFin = true;
		    }
		    break;
		default:
		    if (fEscaped) {
			fEscaped = false;
		    }
		    break;
		
	    }
	    if (fFin) {
		break;
	    }
	}
    
	// now capture the line
	myLine = (char*)malloc(2+f-s);
	memset(myLine, 0, 2+f-s);
	strncpy(myLine, contents+s, f-s+1);
	rc = parse3(myLine);
	free(myLine);
	if (0 != rc) {
	    return NULL;
	}
	s = f+1;
    }
    
DONE:
    if (fDOM && !mElements.empty()) {
	ostringstream err;
	err << "Error: Elements mismatched somehow. Reached end of the document and found that there were more open element tags than close element tags.";

	XoxException myException(err.str().c_str());
	if (mHandler) {
	    mHandler->error(myException);
	} else {
	    throw myException;
	}
    }

    if (mHandler) {
	mHandler->endDoc();
	mHandler->endDoc(mDOM);
    }

    return mDOM;
}


int XoxParser::parse3(const char *aLine)
{
 // determine the kind of line
 size_t len = strlen(aLine);
 if (aLine[0] == '<') {
    if (aLine[len-1] != '>') {
	return -1;
    }
    
    if (aLine[1] == '?' || aLine[1] == '!') {
	// ignore
	return 0;
    }
    else if (aLine[len-2] == '/') {
	return parse4c(aLine); // start & end
    } else if (aLine[1] == '/') {
	return parse4b(aLine); // end of element
    } else {
	// start of an element
	return parse4a(aLine);
    }
    
 } else {
    // should be PC data.
    return parse4d(aLine);
 }
 return 0;
}

// start line
int XoxParser::parse4a(const char *aLine)
{
    int rc = VerifyAttributesIntegrity(aLine);
    
    // get tag / get attributes
    
    if (!rc) {
	rc = ParseForTagAttributes(aLine);
    } else {
	ostringstream err;
	err << "Problem verifying attributes for line: ";
	err << aLine;
	XoxException myException(err.str().c_str());
	if (mHandler) {
	    mHandler->error(myException);
	} else {
	    throw myException;
	}
    }
    
    if (fDOM) {
	XMLElement * pElem = new XMLElement(mTag, mAttributes);

	if (!mDOM->mBaseElement) {
	    mDOM->mBaseElement = pElem;
	    assert(0==mElements.size());
	} else {
	    pElem->mParent = mElements.top();
	    mElements.top()->mKids.push_back(pElem);
	}
	mElements.push(pElem);
    }
    if (mHandler) {
	mHandler->startElement(mTag, mAttributes);
    }

    
    return rc;
}

// end line
int XoxParser::parse4b(const char *aLine)
{
    int rc=0;

    int f=3,s=2;
    char * myTag;
    for(bool fInTag = true;fInTag;) {
	switch(aLine[f]) {
	    case ' ':
	    case '	':
	    case '>':
	    case '/':
	    case '\n':
		fInTag = false;
		break;
	    default:
		++f;
		break;
	}
    }
    myTag = (char*)malloc(f-s+2);
    memset(myTag, 0, f-s+2);
    strncpy(myTag,aLine+s, f-s);

    if (fDOM && 0!=strcmp(myTag, mElements.top()->GetTag().c_str() )) {
	ostringstream err;
	err << "Tag mismatch error: ";
	err << myTag;
	err << " and ";
	err << mTag;
	XoxException myException(err.str().c_str());
	if (mHandler) {
	    mHandler->error(myException);
	} else {
		free(myTag);
	    throw myException;
	}
	rc =-1;
    } else if (mHandler) {
	mHandler->endElement(myTag);
    }
    if (fDOM && mElements.empty()) {
	ostringstream err;
	err << "Error: End element with no matching start element: ";
	err << mTag;
	XoxException myException(err.str());
	if (mHandler) {
	    mHandler->error(myException);
	} else {
		free(myTag);
	    throw myException;
	}
    }
	if (fDOM) {
		mElements.pop();
    }
	free(myTag);
    return rc;
}

// single line
int XoxParser::parse4c(const char *aLine)
{

    int rc = VerifyAttributesIntegrity(aLine);
    
    // get tag / get attributes
    
    if (!rc) {
	rc = ParseForTagAttributes(aLine);
    } else {
	ostringstream err;
	err << "Problem verifying attributes for line: ";
	err << aLine;

	XoxException myException(err.str().c_str());
	
	if (mHandler) {
	    mHandler->error(myException);
	} else {
	    throw myException;
	}
    }
    
    if (fDOM) {
	XMLElement * pElem = new XMLElement(mTag, mAttributes);
	if (!mDOM->mBaseElement) {
	    mDOM->mBaseElement = pElem;
	    assert(0==mElements.size());
	} else {
	    pElem->mParent = mElements.top();
	    mElements.top()->mKids.push_back(pElem);
	}
	mElements.push(pElem);
    }
    if (mHandler) {
	mHandler->startElement(mTag, mAttributes);
    }
    if (mHandler) {
	mHandler->endElement(mTag);
    }
    if (fDOM) {
	mElements.pop();
    }
    
    return rc;
}

// PC Data
int XoxParser::parse4d(const char *aLine)
{
    if (fDOM && mElements.empty()) {
	ostringstream err;
	err << "Error: found PC Data outside of a tag.\n";
	err << aLine; 
	XoxException myException(err.str().c_str());
	if (mHandler) {
	    mHandler->error(myException);
	} else {
	    throw myException;
	}	
    }
    if (fDOM) {
	mElements.top()->mPCData = aLine;
    }
    if (mHandler) {
	mHandler->elementPCData(mTag,aLine);
    }

    return 0;
}

int XoxParser::VerifyAttributesIntegrity(const char *aLine)
{
    // Verify the line's integrity.
    int i;
    int numEquals=0;
    int numQuotes = 0;
    size_t len = strlen(aLine);
    bool fEscaped = false;
    for (i=0;i<len;++i) {
	switch(aLine[i]) {
	    case '=':
		++numEquals;
		break;
	    case '\\':
		fEscaped = !fEscaped;
		break;
	    case '"':
		if (fEscaped) {
		    fEscaped = false;
		} else {
		    numQuotes++;
		}
		break;
	    default:
		fEscaped = false;
		break;
	}
    }
    if (numEquals != (numQuotes/2)) {
	ostringstream err;
	err << "Error: Line is not OK. ";
	err << aLine;
	XoxException myException(err.str());
	if (mHandler) {
	    mHandler->error(myException);
	} else {
	    throw myException;
	}
	return -1;
    }
    return 0;
}



int XoxParser::ParseForTagAttributes(const char *aLine)
{
    size_t len = strlen(aLine);
    int i, numAtts=0;
    int s=1;
    int f=1;

    if('<'!=aLine[0]) { return -1; }
    if ('>' != aLine[len-1]) { return -1; }

    mAttributes.clear();
    if (mTag) {
	free(mTag);
    }
    
    for(bool fInTag = true;fInTag;) {
	switch(aLine[f]) {
	    case ' ':
	    case '	':
	    case '>':
	    case '/':
	    case '\n':
		fInTag = false;
		break;
	    default:
		++f;
		break;
	}
    }
    mTag = (char*)malloc(f-s+2);
    memset(mTag, 0, f-s+2);
    strncpy(mTag,aLine+s, f-s);
    
    
    // count attributes;
    for (i=0;i<len;++i) {
	if ('=' == aLine[i]) {
	    ++numAtts;
	}
    }
    
    // now get attributes
    s=f+1;
    bool fFinished, fEscaped = false;
    for (i=0;i<numAtts;++i) {
	while (isWhiteSpace(aLine[s])) { ++s; }
	for(f=s+1, fFinished = false;;++f) {
	    switch(aLine[f]) {
		case ' ':
		case '	':
		case '\n':
		    fFinished = true;
		    break;
		case '=':
		    fFinished = true;
		    break;
		default:
		    break;
	    }
	    if (fFinished) { break; }
	}
	string attName(aLine + s, 0, f-s);
	for(s=f+1, fFinished = false;;++s) {
	    switch(aLine[s]) {
		case '\\':
		    fEscaped = !fEscaped;
		    break;
		case '"':
		    if (fEscaped) {
			fEscaped = false;
		    } else {
			++s;
			fFinished = true;
		    }
		    break;
		default:
		    fEscaped = false;
		    break;
	    }
	    if (fFinished) { break; }
	}
	for(f=s, fFinished = false;;++f) {
	    switch(aLine[f]) {
		case '\\':
		    fEscaped = !fEscaped;
		    break;
		case '"':
		    if (fEscaped) {
			fEscaped = false;
		    } else {
			fFinished = true;
		    }
		    break;
		default:
		    fEscaped = false;
		    break;
	    }
	    if (fFinished) { break; }
	}
	string attValue(aLine + s, 0, f-s);
	mAttributes[attName]=attValue;
	s=f+1;
    }
    
    return 0;
}


bool XoxParser::isWhiteSpace(char c)
{
    switch(c) {
	case ' ':
	case '	':
	case '\n':
	    return true;
	    break;
	default:
	    return false;
	    break;
    }
}

}

