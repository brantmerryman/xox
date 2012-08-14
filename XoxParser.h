/*
 *  XoxParser.h
 *  XMLParseTest
 *
 *  Created by Brant Sears on 3/31/05.
 *  Copyright (c) 2005 Brant Sears. All rights reserved.
 *
 */

#ifndef _XOX_PARSER
#define _XOX_PARSER
#include <map>
using std::map;
#include <string>
using std::string;
#include <stack>
using std::stack;

#include "Handler.h"
#include "DOM.h"

namespace Xox {

class XoxParser : public Xox
{
    public:
	
	/*! XoxParser
	    
	    Create a Xox parser that you can use to parse documents.
	    By default, it has no handler and returns a DOM *.
	 */
	XoxParser();
	/*! XoxParser
	    
	    Create a Xox parser that you can use to parse documents. This version
	    takes a handler *. Derive your own handler from the handler subclass (see Handler.h).
	    This version also returns a DOM *.
	    
	    \param handler The handler instance you want to use to receive SAX style events.
	 */
	XoxParser(Handler * handler);
	
	virtual ~XoxParser();
	
	/*! SetHandler
	    \param aHandler
	    
	    Sets the SAX event handler for the parser to be the handler you specify. 
	 */
	void SetHandler(Handler * aHandler);
	
	/*!SetCreateDOM
	    \param fCreateDOM
	    
	    Sets whether you want a DOM object returned from the parser.
	 */
	void SetCreateDOM(bool fCreateDOM);
	
	/*! parseFile
	    \param filePath The absolute path to the file.
	    Use this on file paths. Returns a DOM * or NULL. Will fire SAX events if a Handler was specified.
	 */
	DOM * parseFile(const char *filePath);
	/*! parse
	    \param contents 
	    Pass the contents of your XML document or a string formatted in XML to have it parsed.
	 */
	DOM * parse(const char *contents);
        
	/*! description
	    Returns a description of the parser.
	 */
	virtual const char * description();	
protected:
	// detailed parse functions
	int parse3(const char *aLine); // <-determines kind of line
	int parse4a(const char *aLine); // <- parses begin element
	int parse4b(const char *aLine); // <- parses end of element
	int parse4c(const char *aLine); // <- parses single elements (start+end in the same tag group)
	int parse4d(const char *aLine); // <- parses PC data
	
    private:
	bool isWhiteSpace(char c);
	int VerifyAttributesIntegrity(const char *aLine);
	int ParseForTagAttributes(const char *aLine);
	
	bool fDOM;
	char * mTag;
	Handler * mHandler;
	DOM * mDOM;
	map<string, string> mAttributes;
	stack<string> mTags;
	stack<XMLElement *> mElements;
};

}

#endif