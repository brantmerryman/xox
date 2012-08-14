/*
 *  DOM.h
 *  Xox
 *
 *  Created by Brant Sears on Thu Jun 26 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */

#ifndef _XOX_DOM
#define _XOX_DOM
#include "XMLElement.h"

namespace Xox {
class DOM : public Xox
{
    public:
        DOM();
        ~DOM();
	/*! GetBaseElement
	    
	    Returns the base element of the DOM structure.
	 */
        const XMLElement * GetBaseElement();
        
    private:
        XMLElement * mBaseElement;
    friend class XoxParser;
};

}


#endif