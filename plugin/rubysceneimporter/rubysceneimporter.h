/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: rubysceneimporter.h,v 1.2 2004/04/10 09:22:00 rollmark Exp $

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
#ifndef RUBYSCENEIMPORTER_H
#define RUBYSCENEIMPORTER_H

#include <oxygen/sceneserver/sceneimporter.h>
#include <oxygen/sceneserver/basenode.h>
#include <sfsexp/sexp.h>

class RubySceneImporter : public oxygen::SceneImporter
{
public:
    RubySceneImporter();
    virtual ~RubySceneImporter();

    virtual bool ImportScene(const std::string& fileName,
                             boost::shared_ptr<oxygen::BaseNode> root);

protected:
    bool ReadHeader(sexp_t* sexp);
    bool ReadGraph(sexp_t* sexp, boost::shared_ptr<oxygen::BaseNode> root);
    boost::shared_ptr<zeitgeist::Object> CreateInstance(const std::string& className);
    bool ReadMethodCall(sexp_t** _sexp, boost::shared_ptr<oxygen::BaseNode> node);
    boost::shared_ptr<oxygen::BaseNode> CreateNode(sexp_t* sexp);
    bool InvokeMethods(sexp_t** sexp, boost::shared_ptr<oxygen::BaseNode> node);

protected:
    /** the major version of the scen graph file */
    float mVersionMajor;

    /** the minor version of the scen graph file */
    float mVersionMinor;

    /** the last supplied fileName */
    std::string mFileName;
};

DECLARE_CLASS(RubySceneImporter);

#endif // RUBYSCENEIMPORTER_H