/*
 *  Xox.h
 *  Xox
 *
 *  Created by Brant Sears on Tue May 20 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */
#ifndef _XOX
#define _XOX


namespace Xox	{

/*! \mainpage Xox parser
    The Xox parser is a portable C++ XML Parser. To use Xox, you should determine whether you would
    prefer to have SAX style evenets returned to you or would rather have a DOM tree that you can traverse. 
    If you want SAX style events, derive a class from the Handler class and override those methods that 
    you are interested in. 
    Next, create a XoxParser instance. You can call SetCreateDOM(false) to suppress the creation of the 
    DOM if you are not interested in the DOM. Finally, call ParseFile (on a path) or Parse (on an XML string).
    
    Note that if Xox encounters an error, you will receive the error in the form of a XoxException. If you do
    not receive error events as part of your handler, they will be thrown. 

 */

/*! Xox
    
    This is a common base class for Xox objects used to keep track of them and debug.
 */
class Xox {
    public:
        Xox();
        virtual ~Xox();
	
	/*!description
	    Call this method for debug purposes to get info about the object.
	 */
        virtual const char * description();
};


}

#endif
