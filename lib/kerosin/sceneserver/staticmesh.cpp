/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmesh.cpp,v 1.14 2007/02/18 13:05:28 rollmark Exp $

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
#include "staticmesh.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/materialserver/material.h>
#include <kerosin/materialserver/materialserver.h>

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;
using namespace oxygen;

StaticMesh::StaticMesh() : mScale(1.0f,1.0f,1.0f)
{
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::ComputeBoundingBox()
{
}

void StaticMesh::CalcBoundingBox()
{
    mLocalBoundingBox.Init();

    if (mMesh.get() == 0)
        {
            return;
        }

    const int n = mMesh->GetVertexCount();

    const float* arPos = mMesh->GetPos().get();
    if (arPos == 0)
        {
            return;
        }

    for (int i = 0; i<n; ++i)
        {
            const float* v = arPos + (i * 3);
            mLocalBoundingBox.Encapsulate
                (
                 v[0]*mScale[0],
                 v[1]*mScale[1],
                 v[2]*mScale[2]
                 );
        }
}

void StaticMesh::RenderInternal()
{
    if (
        (mMesh.get() == 0) ||
        (mMaterials.empty())
        )
        {
            return;
        }

    const float* pos = mMesh->GetPos().get();
    if (pos == 0)
        {
            return;
        }

    glVertexPointer(3, GL_FLOAT, 0, pos);
    glEnableClientState (GL_VERTEX_ARRAY);

    const float* tex = mMesh->GetTexCoords().get();
    if (tex != 0)
        {
            glTexCoordPointer(3, GL_FLOAT, 0, tex);
            glEnableClientState (GL_TEXTURE_COORD_ARRAY);
        }

    const float* normal = mMesh->GetNormals().get();
    if (normal != 0)
        {
            glNormalPointer(GL_FLOAT, 0, normal);
            glEnableClientState(GL_NORMAL_ARRAY);
        }

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glScalef(mScale[0],mScale[1],mScale[2]);

    TriMesh::TFaces::const_iterator iter = mMesh->GetFaces().begin();
    int i=0;
    while (iter != mMesh->GetFaces().end())
        {
            const shared_ptr<Material> material = mMaterials[i];

            if (material.get() != 0)
                {
                    material->Bind();

                    const TriMesh::Face& face = (*iter);
                    const shared_ptr<IndexBuffer>& idx = face.indeces;

                    glDrawElements(GL_TRIANGLES, idx->GetNumIndex(),
                                   GL_UNSIGNED_INT, idx->GetIndex().get());
                }

            ++i;
            ++iter;
        }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY );
    glDisableClientState(GL_NORMAL_ARRAY);
}

const Vector3f& StaticMesh::GetScale()
{
    return mScale;
}

void StaticMesh::SetScale(const salt::Vector3f& scale)
{
    mScale = scale;
}

bool StaticMesh::Load(const std::string& name)
{
    ParameterList parameter;
    return Load(name,parameter);
}

bool StaticMesh::Load(const std::string& name, const ParameterList& parameter)
{
    mMeshName = name;
    mMeshParameter = parameter;
    mMesh.reset();
    mMaterials.clear();
    CalcBoundingBox();

    shared_ptr<GeometryServer> geometryServer = shared_dynamic_cast<GeometryServer>
        (GetCore()->Get("/sys/server/geometry"));

    if (geometryServer.get() == 0)
        {
            GetLog()->Error()
                << "(StaticMesh) ERROR: cannot get GeometryServer\n";
            return false;
        }

    shared_ptr<MaterialServer> materialServer = shared_dynamic_cast<MaterialServer>
        (GetCore()->Get("/sys/server/material"));

    if (materialServer.get() == 0)
        {
            GetLog()->Error()
                << "(StaticMesh) ERROR: cannot get MaterialServer\n";
            return false;
        }

    mMesh = geometryServer->GetMesh(name,parameter);

    if (mMesh.get() == 0)
        {
            return false;
        }

    CalcBoundingBox();

    // load corresponding materials
    for (
         TriMesh::TFaces::const_iterator iter = mMesh->GetFaces().begin();
         iter != mMesh->GetFaces().end();
         ++iter
         )
        {
            const TriMesh::Face& face = (*iter);

            shared_ptr<Material> material =
                materialServer->GetMaterial(face.material);

            if (material.get() == 0)
                {
                    material = materialServer->GetMaterial("default");
                }

            mMaterials.push_back(material);
        }

    return true;
}

const shared_ptr<TriMesh> StaticMesh::GetMesh()
{
    return mMesh;
}

const std::string& StaticMesh::GetMeshName()
{
    return mMeshName;
}

const ParameterList& StaticMesh::GetMeshParameter()
{
    return mMeshParameter;
}

