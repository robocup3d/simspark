/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceresistanceperceptor.h,v 1.2 2007/06/03 22:12:58 hedayat Exp $

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

#ifndef FORCERESISTANCEPERCEPTOR_H_
#define FORCERESISTANCEPERCEPTOR_H_

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/physicsserver/odewrapper.h>
#include <oxygen/sceneserver/transform.h>
#include <salt/vector.h>
#include <list>
#include <utility>

class ForceResistancePerceptor : public oxygen::Perceptor
{
protected:
    typedef std::list<std::pair<dContactGeom, dJointID> > ContactListType;

public:
    /** adds new touch information
     */
    void AddTouchInfo(dContact &contact, dJointID contactJointID);

    /** adds touch information to predList
        \return true if data is available
     */
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    virtual void OnLink();
    virtual void OnUnlink();

    /** removes old information left over from the previous cycles */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    ContactListType mContactList;
    //! reference to the parent Transform node
    boost::shared_ptr<oxygen::Transform> mBody;

    // These are required while the Percept function can be called more than
    // once in a time step!
    
    //! last calculated point
    salt::Vector3f mLastPoint;

    //! last calculated force
    salt::Vector3f mLastForce;
};

DECLARE_CLASS(ForceResistancePerceptor);

#endif /*FORCERESISTANCEPERCEPTOR_H_*/
