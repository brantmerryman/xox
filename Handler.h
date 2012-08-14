/*
 *  Handler.h
 *  Xox
 *
 *  Created by Brant Sears on Tue May 20 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */

#ifndef _XOX_HANDLER
#define _XOX_HANDLER

#include "XoxException.h"
#include "DOM.h"

#include <map>
#include <string>
using std::map;
using std::string;

namespace Xox {

#include "Xox.h"

/*! Handler

    This is the Handler class from which you can derive your own SAX handler. You do not need
    to call the methods you are overriding and you do not need to override methods you are not
    interested in.
 */
class Handler : public Xox
{
    public:
	/*! startDoc
	    Called when starting to parse a document. 
	 */
        virtual void startDoc() const;
	/*! startDoc
	    Called when parsing is complete for a document. 
	 */
        virtual void endDoc() const;
	/*!
	    \param pDOM A pointer to a DOM object that was created by the parse.
	    Called when parsing is complete for a document. 
	*/
        virtual void endDoc(DOM * pDOM) const;
	/*!startElement
	    \param tag The XML tag name.
	    \param attributes The attributes of the tag.
	    
	    This is called when the beginning of an XML tag is encountered.
	 */
        virtual void startElement(const char *tag, map<string, string> & attributes);
	/*! elementPCData
	    \param tag The XML tag name.
	    \param pcData The pc data that was encountered.
	    PC Data is data the comes in between a set of tags but is not itself formatted
	    using tags. 
	 */
        virtual void elementPCData(const char * tag, const char * pcData);
	/*! endElement
	    \param tag The XML tag name.
	    
	    This is called when an end tag is encountered.
	 */
        virtual void endElement(const char * tag);
        
	/*!error
	    \param exception This is a Xox exception that contains some information about the error that was encountered.
	    
	    If you wish to receive errors in your error handler, you can override this method. Otherwise, these errors are thrown.
	    See the file XoxException.h for more information about the XoxException datatype.
	 */
        virtual void error(XoxException & exception) const;
};

}

#endif
