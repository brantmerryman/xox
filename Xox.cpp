/*
 *  Xox.cpp
 *  Xox
 *
 *  Created by Brant Sears on Tue May 20 2003.
 *  Copyright (c) 2003 Brant Sears. All rights reserved.
 *
 */

#include "Xox.h"
#include <typeinfo>

namespace Xox {

Xox::Xox()
{
}

Xox::~Xox()
{
}

const char * Xox::description()
{
    return typeid(*this).name();
}

}