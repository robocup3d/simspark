/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Mon May 9 2005
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: pantiltaction.h,v 1.1 2006/03/10 13:46:47 fruit Exp $

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
#ifndef DRIVEACTION_H
#define DRIVEACTION_H

#include <oxygen/gamecontrolserver/actionobject.h>
#include <salt/vector.h>

class PanTiltAction : public oxygen::ActionObject
{
public:
    PanTiltAction(const std::string& predicate, float pan, float tilt)
        : ActionObject(predicate), mPan(pan), mTilt(tilt) {}

    virtual ~PanTiltAction() {}

    //! @return the pan angle
    float GetPanAngle() const { return mPan; }

    //! @return the tilt angle
    float GetTiltAngle() const { return mTilt; }

protected:
    //! the pan angle (in degrees)
    float mPan;
    //! the tilt angle (in degrees)
    float mTilt;
};

#endif // DRIVEACTION_H