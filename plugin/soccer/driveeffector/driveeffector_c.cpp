/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: driveeffector_c.cpp,v 1.3 2004/02/21 15:46:34 fruit Exp $

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

#include "driveeffector.h"

using namespace oxygen;

FUNCTION(setForceFactor)
{
    if (in.size() == 1)
    {
        DriveEffector* de = static_cast<DriveEffector*>(obj);
        de->SetForceFactor(boost::any_cast<float>(in[0]));
    }
}

FUNCTION(setSigma)
{
    if (in.size() == 1)
    {
        DriveEffector* de = static_cast<DriveEffector*>(obj);
        de->SetSigma(boost::any_cast<float>(in[0]));
    }
}

FUNCTION(setMaxPower)
{
    if (in.size() == 1)
    {
        DriveEffector* de = static_cast<DriveEffector*>(obj);
        de->SetMaxPower(boost::any_cast<float>(in[0]));
    }
}

FUNCTION(setConsumption)
{
    if (in.size() == 1)
    {
        DriveEffector* de = static_cast<DriveEffector*>(obj);
        de->SetConsumption(boost::any_cast<float>(in[0]));
    }
}

void
CLASS(DriveEffector)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Effector);
    DEFINE_FUNCTION(setForceFactor);
    DEFINE_FUNCTION(setSigma);
    DEFINE_FUNCTION(setMaxPower);
    DEFINE_FUNCTION(setConsumption);
}
