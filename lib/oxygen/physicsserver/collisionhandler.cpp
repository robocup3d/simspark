/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collisionhandler.cpp,v 1.1.2.2 2004/01/29 10:20:06 rollmark Exp $

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
#include "collisionhandler.h"
#include "world.h"
#include "space.h"
#include "collider.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>

using namespace oxygen;
using namespace boost;

void
CollisionHandler::OnLink()
{
    // setup the scene, world, space and collider references
    shared_ptr<Scene> scene = GetScene();

    mWorld = shared_static_cast<World>(scene->GetChildOfClass("World"));
    if (mWorld.get() == 0)
        {
            GetLog()->Debug()
                << "(CollisionHandler) found no World node\n";
        }

    mSpace = shared_static_cast<Space>(scene->GetChildOfClass("Space"));
    if (mSpace.get() == 0)
        {
            GetLog()->Debug()
                << "(CollisionHandler) found no Space node\n";
        }

    mCollider = shared_static_cast<Collider>(make_shared(GetParent()));
    if (mCollider.get() == 0)
        {
            GetLog()->Debug()
                << "(CollisionHandler) found no parent Collider node\n";
        }
}

void
CollisionHandler::OnUnlink()
{
    mCollider.reset();
    mWorld.reset();
    mSpace.reset();
}

