/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: telnetdaemon.h,v 1.3 2003/09/05 12:22:51 fruit Exp $

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

   TelnetDaemon

        HISTORY:
                19.06.2002 MK
                        - initial version
*/

#ifndef TELNETDAEMON_H__
#define TELNETDAEMON_H__

#ifdef _WIN32
#include <winsock.h>
#endif
#include <list>

namespace zeitgeist
{

class TelnetSession;
class TelnetServer;

/** the TelnetDaemon is responsible to listen for incoming
    connections. For each connection it creates a new thread with a
    TelnetSession object, managing the session.
*/
class TelnetDaemon
{
        //
        // types
        //
public:
protected:
private:
        typedef std::list<TelnetSession*>       TSessionList;

        //
        // functions
        //
public:
        /** constructs the TelnetDaemon
            \param server is a reference to the server constructing this daemon
        */
        TelnetDaemon(TelnetServer &server);
        virtual ~TelnetDaemon();

        /** contains the code run inside the thread for this Daemon,
            called from the boost thread library */
        void operator()();

        /** called from the server to shutdown the daemon. This method
            destroys all session objects */
        void Terminate();

        /** prints the status of the daemon to stdout */
        void Status();

        /** adds a session a object to the list of managed sessions */
        void Attach(TelnetSession *session);

        /** removes a session object from the list of managed sessions */
        void Detach(TelnetSession *session);

        /** return a reference to the TelnetServer object, called by
            the TelnetSession objects */
        const TelnetServer&     GetServer() const       {       return mServer; }

protected:
        /** create the network socket and start the daemon */
        bool Init(int portNr);


        /** accepts a pending connection request, creates a new socket
            and associates the TelnetSession cc with it*/
        bool AcceptConnection(TelnetSession& cc);

        //
        // members
        //
public:
protected:
private:
        /** the listen socket of the server */
        SOCKET                  mDaemonSocket;

        /** the local adress the daemon is bound to */
        sockaddr_in             mDaemonAddr;

        /** the list of current active sessions */
        TSessionList    mSessions;

        /** a reference to the TelnetServer */
        TelnetServer    &mServer;
};

}

#endif //TELNETDAEMON_H__
