/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: tcpsocket.cpp,v 1.1 2004/03/09 17:13:33 fruit Exp $

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
#include "tcpsocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <rcssbase/net/exception.hpp>
#include <iostream>

namespace rcss
{
    namespace net
    {
        TCPSocket::TCPSocket()
        {
            open();
        }

        TCPSocket::TCPSocket( const Addr& addr )
        {
            open();
            bind( addr );
        }

        TCPSocket::TCPSocket( const Addr& addr, const Addr& dest )
        {
            open();
            bind( addr );
            connect( dest );
        }

        void
        TCPSocket::doOpen( int& fd )
        {
            close();
            fd = ::socket(AF_INET, SOCK_STREAM, 0 );
            if( fd < 0 )
                throw OpenErr( errno );
        }
    }
}
