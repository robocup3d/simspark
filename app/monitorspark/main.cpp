/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.1 2004/04/29 15:27:38 rollmark Exp $

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

#include <spark/spark.h>

using namespace spark;
using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

class SimSpark : public Spark
{
public:
    SimSpark(const std::string& relPathPrefix) :
        Spark(relPathPrefix)
    {};

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    /** print command line options */
    void PrintHelp();

    /** print a greeting */
    void PrintGreeting();

    /** process command line options */
    bool ProcessCmdLine(int argc, char* argv[]);
};

void SimSpark::PrintGreeting()
{
    GetLog()->Normal()
        << "monitorspark, 0.1\n"
        << "Universitšt Koblenz.\n"
        << "Copyright (C) 2004, "
        << "The RoboCup Soccer Server Maintenance Group.\n"
        << "\nType '--help' for further information\n\n";
}

void SimSpark::PrintHelp()
{
    GetLog()->Normal()
        << "\nusage: monitorspark [options]\n"
         << "\noptions:\n"
         << " --help\t print this message.\n"
         << "\n";
}

bool SimSpark::ProcessCmdLine(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "--help" ) == 0 )
        {
          PrintHelp();
          return false;
        }
    }

  return true;
}

bool SimSpark::InitApp(int argc, char** argv)
{
    GetSimulationServer()->SetSimStep(0.02);
    PrintGreeting();

    // process command line
    if (! ProcessCmdLine(argc, argv))
        {
            return false;
        }

    // run initialization scripts
     GetScriptServer()->Run("monitorspark.rb");
     GetScriptServer()->Run("bindings.rb");

    // tell the inputControl node the loaction of our camera
    shared_ptr<InputControl> inputCtr = GetInputControl();
    if (inputCtr.get() != 0)
        {
            inputCtr->SetFPSController("/usr/scene/camera/physics/controller");
        }

    return true;
}

int main(int argc, char** argv)
{
    // the spark app framework instance
    SimSpark spark("../../");

    if (! spark.Init(argc, argv))
        {
            return 1;
        }

    spark.GetSimulationServer()->Run(argc,argv);

    shared_ptr<RenderControl> renderCtr = spark.GetRenderControl();
    if (renderCtr.get() != 0)
        {
            spark.GetLog()->Normal()
                << "Average FPS: "
                << renderCtr->GetFramesRendered() /
                   spark.GetSimulationServer()->GetTime()
                << "\n";
        }

    return 0;
}
