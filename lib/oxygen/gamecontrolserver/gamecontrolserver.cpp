/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver.cpp,v 1.4 2004/01/01 18:24:33 fruit Exp $

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
#include <sstream>
#include "gamecontrolserver.h"
#include "baseparser.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/corecontext.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

GameControlServer::GameControlServer() : zeitgeist::Node()
{
}

GameControlServer::~GameControlServer()
{
}

bool
GameControlServer::InitParser(const std::string& parserName)
{
    mParser = shared_dynamic_cast<BaseParser>(GetCore()->New(parserName));

    if (mParser.get() == 0)
    {
        GetLog()->Error() << "ERROR: (GameControlServer::InitParser) Unable to create "
                          << parserName << std::endl;
        return false;
    }

    return true;
}

void
GameControlServer::InitEffector(const std::string& effectorName)
{
   mCreateEffector = effectorName;
}

shared_ptr<BaseParser>
GameControlServer::GetParser()
{
    return mParser;
}

shared_ptr<Scene>
GameControlServer::GetActiveScene()
{
    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) SceneServer not found.\n";
            return shared_ptr<Scene>();
        }

    shared_ptr<Scene> scene = sceneServer->GetActiveScene();

    if (scene.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) SceneServer reports no active scene\n";
        }

    return scene;
}

bool
GameControlServer::AgentConnect(int id)
{
    // for map::insert(Elem), the test here is not required. map::insert does not
    // overwrite existing elements. The test is required to report if the agent
    // was already connected or not.
    if (mAgentMap.find(id) != mAgentMap.end())
    {
        return false;
    }

    GetLog()->Debug()
        << "(GameControlServer) A new agent connected (id: " << id << ")\n";

    shared_ptr<Scene> scene = GetActiveScene();
    if (scene.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) Got no active scene from the SceneServer to"
                << " create the AgentAspect in.\n";
            return false;
        }

    // create a new AgentAspect for the ID in the scene and add it to
    // our map of AgentAspects
    shared_ptr<AgentAspect> aspect = shared_dynamic_cast<AgentAspect>
        (GetCore()->New("kerosin/AgentAspect"));

    if (aspect.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) cannot create new AgentAspect\n";
            return false;
        }

    stringstream name;
    name << "_AgentAspect" << id;
    aspect->SetName(name.str());

    scene->AddChildReference(aspect);
    mAgentMap[id] = aspect;

    return aspect->Init(mCreateEffector);
}

bool GameControlServer::AgentDisappear(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) AgentDisappear called for "
                << "unknown agent id " << id << "\n";
            return false;
        }

    // remove the AgentAspect from the Scene and our map. The
    // AgentAspect does all the necessary cleanup
    mAgentMap.erase(id);

    shared_ptr<Scene> scene = GetActiveScene();
    if (scene.get() != 0)
        {
            RemoveChildReference((*iter).second);
        }

    GetLog()->Debug() << "(GameControlServer) An agent disconnected (id: "
                      << id << ")\n";

    return true;
}

float GameControlServer::GetSenseInterval(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent.
    return 0.25;
}

float GameControlServer::GetSenseLatency(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent
    return 0.1;
}

float GameControlServer::GetActionLatency(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent.
    return 0.1;
}


shared_ptr<ActionObject::TList> GameControlServer::Parse(int id, string str) const
{
    TAgentMap::const_iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer::Parse) Parse called with unknown agent id "
                << id << "\n";
            return shared_ptr<ActionObject::TList>(new ActionObject::TList());
        }

    if (mParser.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer::Parse) No parser registered.\n";
            return shared_ptr<ActionObject::TList>(new ActionObject::TList());
        }

    // use the parser to create a TPredicateList
    shared_ptr<Predicate::TList> predicates(mParser->Parse(str));

    // construct an ActionList using the registered effectors
    shared_ptr<ActionObject::TList> actionList(new ActionObject::TList());

    // update the map of effectors below the agentaspect
    shared_ptr<AgentAspect> aspect = (*iter).second;
    aspect->UpdateEffectorMap();

    for
        (
         Predicate::TList::iterator iter = predicates->begin();
         iter != predicates->end();
         ++iter
        )
        {
            Predicate& predicate = (*iter);

            shared_ptr<Effector> effector = aspect->GetEffector(predicate.name);
            if (effector.get() == 0)
                {
                    GetLog()->Warning()
                        << "(GameControlServer::Parse) No effector registered for predicate "
                        << predicate.name << "\n";
                    continue;
                }

            shared_ptr<ActionObject> action(effector->GetActionObject(predicate));

            if (action.get() == 0)
                {
                    continue;
                }

            actionList->push_back(action);
        }

    return actionList;
}

shared_ptr<AgentAspect> GameControlServer::GetAgentAspect(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);
    if (iter == mAgentMap.end())
        {
            return shared_ptr<AgentAspect>();
        } else
            {
                return (*iter).second;
            }
}
