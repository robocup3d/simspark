/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerbase.cpp,v 1.6 2004/03/25 22:06:55 jboedeck Exp $

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
#include "soccerbase.h"
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/controlaspect/controlaspect.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/ball/ball.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace std;

bool
SoccerBase::GetSceneServer(const Leaf& base,
                           shared_ptr<SceneServer>& scene_server)
{
    scene_server = shared_static_cast<SceneServer>
        (base.GetCore()->Get("/sys/server/scene"));

    if (scene_server.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") scene server not found.\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetTransformParent(const Leaf& base,
                               shared_ptr<Transform>& transform_parent)
{
    transform_parent = shared_dynamic_cast<Transform>
        (make_shared(base.GetParentSupportingClass("Transform")));

    if (transform_parent.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") parent node is not derived from TransformNode\n";
        return false;
    }
    return true;
}

bool
SoccerBase::GetAgentState(const shared_ptr<Transform> transform,
                          shared_ptr<AgentState>& agent_state)
{
    agent_state =
        shared_dynamic_cast<AgentState>(transform->GetChild("AgentState"));

    if (agent_state.get() == 0)
    {
        transform->GetLog()->Error()
            << "Error: (SoccerBase: " << transform->GetName()
            << ") node has no AgentState child\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetAgentBody(const shared_ptr<Transform> transform,
                          shared_ptr<Body>& agent_body)

{
    agent_body = shared_dynamic_cast<Body>
        (transform->GetChildSupportingClass("Body", true));

    if (agent_body.get() == 0)
    {
        transform->GetLog()->Error()
            << "(SoccerBase) ERROR: " << transform->GetName()
            << ") node has no Body child\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetAgentBody(const Leaf& base, TTeamIndex idx,
                         int unum, shared_ptr<Body> & agent_body)
{      
    shared_ptr<AgentState> agentState;
    shared_ptr<Transform>  parent;

    // get matching AgentState
    GetAgentState(base, idx, unum, agentState);
    
    // get AgentAspect
    GetTransformParent(*agentState, parent);

    // call GetAgentBody with matching AgentAspect
    GetAgentBody(parent, agent_body);

    return true;
}

bool
SoccerBase::GetAgentState(const Leaf& base,
                          shared_ptr<AgentState>& agent_state)
{
    shared_ptr<Transform> parent;
    if (! GetTransformParent(base,parent))
    {
        return false;
    }

    return SoccerBase::GetAgentState(parent,agent_state);
}

bool
SoccerBase::GetAgentState(const Leaf& base, TTeamIndex idx,
                          int unum, shared_ptr<AgentState>& agentState)
{
    // get the active scene
    shared_ptr<Scene> activeScene;

    if (GetActiveScene(base, activeScene))
    {
        Leaf::TLeafList leafList;

        // get a list of all the agent aspects
        activeScene->GetChildrenOfClass("AgentAspect", leafList);

        if (leafList.size() == 0)
        {
            base.GetLog()->Error()
                << "ERROR: (SoccerBase) active scene doesn't have "
                << "children of type AgentAspect\n";

            return false;
        }        

        Leaf::TLeafList::iterator iter = leafList.begin();

        // search through the list to find an agent state
        // with matching team index and unum
        for (
              iter;
              iter != leafList.end();
              ++iter
             )
        {
            shared_ptr<Transform> agentAspect =
             shared_dynamic_cast<Transform>(*iter);

            GetAgentState(agentAspect, agentState);

            if (
                (agentState->GetTeamIndex() == idx) &&
                (agentState->GetUniformNumber() == unum)
               )
            {
                return true;
            }
        }

        return false;
    } 
}

bool
SoccerBase::GetGameState(const Leaf& base,
                         shared_ptr<GameStateAspect>& game_state)
{
    game_state = shared_dynamic_cast<GameStateAspect>
        (base.GetCore()->Get("/sys/server/gamecontrol/GameStateAspect"));

    if (game_state.get() == 0)
        {
            base.GetLog()->Error()
                << "Error: (SoccerBase: " << base.GetName()
                << ") found no GameStateAspect\n";
            return false;
        }

    return true;
}

bool
SoccerBase::GetActiveScene(const Leaf& base,
                           shared_ptr<Scene>& active_scene)
{
    shared_ptr<SceneServer> sceneServer;
    if (! GetSceneServer(base,sceneServer))
        {
            return false;
        }

    active_scene = sceneServer->GetActiveScene();
    if (active_scene.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") SceneServer reports no active scene\n";
        return false;
    }
    return true;
}

bool
SoccerBase::GetBody(const Leaf& base, shared_ptr<Body>& body)
{
    shared_ptr<Transform> parent;
    if (! GetTransformParent(base,parent))
    {
        return false;
    }

  body = shared_dynamic_cast<Body>(parent->GetChildOfClass("Body"));

  if (body.get() == 0)
    {
      base.GetLog()->Error()
          << "ERROR: (SoccerBase: " << base.GetName()
          << ") parent node has no Body child.";
      return false;
    }

  return true;
}

bool
SoccerBase::GetBall(const Leaf& base, shared_ptr<Ball>& ball)
{
    shared_ptr<Scene> scene;
    if (! GetActiveScene(base,scene))
        {
            return false;
        }

    ball = shared_dynamic_cast<Ball>
        (base.GetCore()->Get(scene->GetFullPath() + "Ball"));

    if (ball.get() == 0)
        {
            base.GetLog()->Error()
                << "ERROR: (SoccerBase: " << base.GetName()
                << " found no ball node\n";
            return false;
        }

    return true;
}

bool
SoccerBase::GetBallBody(const Leaf& base, shared_ptr<Body>& body)
{
    shared_ptr<Scene> scene;
    if (! GetActiveScene(base,scene))
        {
            return false;
        }

   body = shared_dynamic_cast<Body>
        (base.GetCore()->Get(scene->GetFullPath() + "Ball/physics"));

   if (body.get() == 0)
       {
            base.GetLog()->Error()
                << "ERROR: (SoccerBase: " << base.GetName()
                << " found no ball body node\n";
            return false;
       }

   return true;
}

bool
SoccerBase::GetBallCollider(const zeitgeist::Leaf& base,
                boost::shared_ptr<oxygen::SphereCollider>& sphere)
{
    shared_ptr<Scene> scene;
    if (! GetActiveScene(base,scene))
        {
            return false;
        }

   sphere = shared_dynamic_cast<SphereCollider>
        (base.GetCore()->Get(scene->GetFullPath() + "Ball/geometry"));

   if (sphere.get() == 0)
       {
            base.GetLog()->Error()
                << "ERROR: (SoccerBase: " << base.GetName()
                << ") Ball got no SphereCollider node\n";
            return false;
       }

   return true;
}

salt::Vector3f
SoccerBase::FlipView(const salt::Vector3f& pos, TTeamIndex ti)
{
    salt::Vector3f newPos;
    switch (ti) {
    case TI_NONE:
        newPos[0] = 0.0;
        newPos[1] = 0.0;
        newPos[2] = 0.0;
        break;
    case TI_LEFT:
        newPos[0] = -pos[0];
        newPos[1] = -pos[1];
        newPos[2] = pos[2];
        break;
    case TI_RIGHT:
        newPos = pos;
        break;
    }
    return newPos;
}

string
SoccerBase::PlayMode2Str(const TPlayMode mode)
{
    switch (mode)
        {
        case PM_BeforeKickOff:
            return STR_PM_BeforeKickOff;

        case PM_KickOff_Left:
            return STR_PM_KickOff_Left;

        case PM_KickOff_Right:
            return STR_PM_KickOff_Right;

        case PM_PlayOn:
            return STR_PM_PlayOn;

        case PM_KickIn_Left:
            return STR_PM_KickIn_Left;

        case PM_KickIn_Right:
            return STR_PM_KickIn_Right;

        case PM_GameOver:
            return STR_PM_GameOver;

        case PM_Goal_Left:
            return STR_PM_Goal_Left;

        case PM_Goal_Right:
            return STR_PM_Goal_Right;

        default:
            return STR_PM_Unknown;
        };
}

shared_ptr<ControlAspect>
SoccerBase::GetControlAspect(const zeitgeist::Leaf& base,const string& name)
{
  static const string gcsPath = "/sys/server/gamecontrol/";

  shared_ptr<ControlAspect> aspect = shared_dynamic_cast<ControlAspect>
    (base.GetCore()->Get(gcsPath + name));

  if (aspect.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") found no ControlAspect " << name << "\n";
    }

  return aspect;
}


