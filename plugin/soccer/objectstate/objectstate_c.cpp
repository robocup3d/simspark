/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: objectstate_c.cpp,v 1.2 2004/02/12 14:07:26 fruit Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "objectstate.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setPerceptName)
{
    if (in.size() == 1)
    {
        ObjectState* os = static_cast<ObjectState*>(obj);
        os->SetPerceptName(boost::any_cast<char*>(in[0]));
    }
}

FUNCTION(setID)
{
    if (in.size() == 1)
    {
        ObjectState* os = static_cast<ObjectState*>(obj);
        os->SetID(boost::any_cast<char*>(in[0]));
    }
}

void
CLASS(ObjectState)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/BaseNode);
    DEFINE_FUNCTION(setPerceptName);
    DEFINE_FUNCTION(setID);
}
