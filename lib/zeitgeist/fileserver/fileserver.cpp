/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: fileserver.cpp,v 1.5 2004/02/12 14:07:23 fruit Exp $

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
#include "fileserver.h"
#include <salt/fileclasses.h>
#include <zeitgeist/core.h>

using namespace boost;
using namespace salt;
using namespace zeitgeist;

FileServer::FileServer() : Node()
{
}

FileServer::~FileServer()
{
}

salt::RFile* FileServer::Open(const char* inName)
{
  for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
      shared_ptr<FileSystem> fileSys = shared_static_cast<FileSystem>(*i);
      salt::RFile *file = fileSys->Open(inName);

      //first successful is returned
      if(file != NULL)
        {
          return file;
        }
    }

  // try to open it via the regular file system
  salt::RFile *file = new StdFile();
  if (file->Open(inName))
    {
      return file;
    }

  // too bad ... didn't work
  delete file;
  return NULL;
}

bool FileServer::Exist(const char *inName)
{
  salt::RFile *file = Open(inName);

  if(file == NULL)
    {
      return false;
    }

  delete file;
  return true;
}

// this routine registers a new file system instance with the server
bool FileServer::Mount(const char *inFileSysName, const char *inPath)
{
  shared_ptr<FileSystem> fileSys = shared_static_cast<FileSystem>(GetChild(inPath));

  if (fileSys)
    {
      // we already have a file system which is bound to the same name
      if (fileSys->GetClass()->GetName().compare(inFileSysName) == 0)
        {
          // as the file system has the same type, we can return true
          return true;
        }
      else
        {
          // already have a file system of a different type, so return false
          return false;
        }
    }

  // try to instantiate the file system
  fileSys = shared_static_cast<FileSystem>(GetCore()->New(inFileSysName));

  if(fileSys)
    {
      if(fileSys->SetPath(inPath))
        {
          // try to name the file system
          fileSys->SetName(inPath);

          // link it into our hierarchy
          if (AddChildReference(fileSys))
            {
              return true;
            }
        }
      // once we reach this point something hase gone wrong
    }

  return false;
}

bool FileServer::Unmount(const char* inPath)
{
  // try to remove a std file system first
  if (Unmount ("FileSystemSTD", inPath))
    {
      return true;
    }

  shared_ptr<Leaf> leaf = GetChild(inPath);

  if(leaf)
    {
      leaf->Unlink();
      return true;
    }

  return false;
}

bool FileServer::Unmount(const char* inFileSysName, const char* inPath)
{
  shared_ptr<FileSystem> fileSystem = shared_static_cast<FileSystem>(GetChild(inPath));

  if(fileSystem)
    {
      if (fileSystem->GetClass()->GetName().compare(inFileSysName) == 0)
        {
          fileSystem->Unlink();
          return true;
        }
    }

  return false;
}

int FileServer::ForEachFile(const char* /*directory*/, const char* /*name*/, const char* /*extension*/, FileSystem::TCallback /*callback*/, void* /*param*/)
{
  int count = 0;
  /*for (TDescriptionList::iterator iter = mFileSystems.begin (); iter != mFileSystems.end (); ++iter)
    {
    count += iter->fileSystem->ForEachFile(directory,name,extension,callback,param);
    }*/

  return count;
}

