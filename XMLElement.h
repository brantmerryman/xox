/*
 *  XMLElement.h
 *  Xox
 *
 *  Created by Brant Sears on Thu Jun 26 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */

#ifndef _XML_ELEMENT
#define _XML_ELEMENT
#include "Xox.h"
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

namespace Xox {

/*! XMLElement
    The XMLElement class is used to allow the DOM structure to be built. Each XMLElement represents the beginning
    and end tag for an element and contains its attributes and subtags.
 */
class XMLElement : public Xox
{
    public:
        virtual ~XMLElement();
        
	/*! GetTag
	    Returns the name of the tag.
	*/
        inline const string & GetTag() const;
	/*! GetKids
	    Returns a vector containing the sub elements.
	*/
        inline vector<XMLElement*> & GetKids();
	/*! GetAttributes
	    Returns the attributes and values in the form of a map.
	*/
        inline map<string, string> & GetAttributes();
	/*! GetParent
	    Returns the parent element of the element (allowing for reverse traversal) if there is one.
	    There isn't one for the base element.
	 */
        inline const XMLElement * GetParent() const { return mParent; }
	/*! GetPCData
	    Returns the PC data for the element. Should be an empty string if there isn't any pc data.
	*/
	inline const string & GetPCData() const;
	
    private:
        XMLElement(const char * tag, const map<string, string> & attributes);
        XMLElement();
        string mTag;
        map<string, string> mAttributes;
        string mPCData;
        vector<XMLElement*> mKids;
	XMLElement * mParent;
    friend class XoxParser;
};

inline const string & XMLElement::GetPCData() const
{
    return mPCData;
}

inline const string & XMLElement::GetTag() const
{
    return mTag;
}

inline vector<XMLElement*> & XMLElement::GetKids()
{
    return mKids;
}

inline map<string, string> & XMLElement::GetAttributes()
{
    return mAttributes;
}

}
#endif