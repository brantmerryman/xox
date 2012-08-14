/*
 *  XoxException.h
 *  Xox
 *
 *  Created by Brant Sears on Tue May 20 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */
#ifndef XOX_EXCEPTION
#define XOX_EXCEPTION

#include <string>
using std::string;

#include "Xox.h"

namespace Xox {

    class XoxException : public Xox
    {
        public:
            virtual ~XoxException() {}
	    
	    /*!description
		Returns a description of the error that was encountered.
		Note that Xox is a non-validating parser in the sense that it does not use a 
		schema or DTD to validate the format of your XML. It is not guarenteed to find all mistakes in
		an XML file. However, some mistakes can't just be ignored and will result in a XoxException being thrown 
		or passed to you.
	    */
            virtual const char * description() { return mDescription.c_str(); }
            
        private:
             XoxException(const char * aDescription) : mDescription(aDescription) {}
             XoxException(const string & aDescription) : mDescription(aDescription)	{}
            string mDescription;
    
    friend class XoxParser;
    };
}

#endif