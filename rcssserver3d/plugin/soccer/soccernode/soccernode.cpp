/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#include "soccernode.h"
#include <oxygen/sceneserver/transform.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

SoccerNode::SoccerNode() : BaseNode()
{
}

SoccerNode::~SoccerNode()
{
}

boost::shared_ptr<oxygen::Transform>
SoccerNode::GetTransformParent() const
{
    return mTransformParent;
}

void
SoccerNode::UpdateCached()
{
    BaseNode::UpdateCached();

    mTransformParent = dynamic_pointer_cast<Transform>
        (GetParentSupportingClass("Transform").lock());

    if (mTransformParent.get() == 0)
    {
        GetLog()->Error()
            << "Error: (SoccerNode: " << GetName()
            << ") parent node is not derived from TransformNode\n";
    }
}

void
SoccerNode::OnLink()
{
    BaseNode::OnLink();
    UpdateCached();
}

void
SoccerNode::OnUnlink()
{
    BaseNode::OnUnlink();
    mTransformParent.reset();
}
