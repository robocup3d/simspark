/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: behavior.cpp,v 1.2 2004/02/12 14:07:21 fruit Exp $

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
#include "behavior.h"

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;

Behavior::Behavior()
{
}

Behavior::~Behavior()
{
}

void Behavior::ProcessInitMessage()
{
    // Ddata

    // After startup, this initialization data message is sent to the
    // agent. On a normal startup, data will be empty. On an agent
    // migration (see Section 5.3), the data returned by the old agent
    // processes will be given to the new agent. An initialization done
    // message should be sent once the initialization data has been
    // processed and all other startup is complete.

    GetLog()->Debug()
        << "(Behavior) received init message.\n";

    mComm->PutOutput(DoneInitMessage());
}

void Behavior::ProcessThinkTimeMessage()
{
    // Ktime

    // This is a think time message that notifies the agent how much
    // thinking time was used for the last thinking cycle. These are
    // only sent if send agent think times is on.

    GetLog()->Debug()
        << "(Behavior) think Time Message.\n";

    // still unsure what the correct behavior is...
    mComm->PutOutput(DoneThinkingMessage());
}

void Behavior::Think()
{
    // Stime time data

    // This is a sensation to be given to the agent. It begins a
    // thinking cycle. The first time is the simulation time the
    // sensation was generated (also known as the send time) and the
    // second is the time that the sensation is delivered to the agent
    // (also known as the arrive time). If the parameter send agent send
    // time is off, -1 is always sent as the send time, and if the
    // parameter send agent arrive time is off, -1 is always sent as the
    // arrive time. The data is an arbitrary data string generated by
    // the world model. The agent can reply with act messages, and must
    // finish with a done thinking message.

    mComm->PutOutput(DoneThinkingMessage());
}

void Behavior::OnLink()
{
    mWM = shared_dynamic_cast<WorldModel>
        (GetCore()->Get("/sys/WorldModel"));
    if (mWM.get() == 0)
        {
            GetLog()->Error() << "ERROR: (Behavior) cannot find WorldModel\n";
        }

    mComm = shared_dynamic_cast<CommServer>
        (GetCore()->Get("/sys/CommServer"));

    if (mComm.get() == 0)
        {
            GetLog()->Error() << "ERROR: (Behavior) cannot find CommServer\n";
        }
}

void Behavior::OnUnlink()
{
    mWM.reset();
    mComm.reset();
}



