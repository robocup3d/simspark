/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialsolid.cpp,v 1.1 2004/03/20 12:54:07 rollmark Exp $

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
#include "materialsolid.h"
#include <kerosin/openglserver/openglserver.h>

using namespace kerosin;
using namespace zeitgeist;
using namespace salt;
using namespace boost;
using namespace std;

MaterialSolid::MaterialSolid()
    : Material(), mColor(1,1,1)
{
}

MaterialSolid::~MaterialSolid()
{
}

void MaterialSolid::Bind()
{
    glColor3fv(mColor.GetData());
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MaterialSolid::SetColor(const Vector3f& color)
{
    mColor = color;
}