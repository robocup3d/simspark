// This is a dummy agent created for testing with the SpadesTest. It
// reads length prefixed data (see spades manual 6.3) from fd 3 and
// writes to fd 4. A corresponding agent DB (agentdb.list, entry
// 'default') exists in the spadestest directory.

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

using namespace std;

// the receive buffer of the agent
static char buffer[4096];

// pointer to the payload received in the buffer
static char* msg_data = &buffer[sizeof(long)];

void
log(const char* out)
{
  printf(out);
  fflush(NULL);
}

int
getInput()
{
  ssize_t sz = read(3, buffer, sizeof(buffer));

  if ((unsigned int)sz < sizeof(unsigned int))
    {
      return 0;
    }

  unsigned int len;
  memcpy(&len, buffer, sizeof(unsigned int));
  len = ntohl(len);

  // zero terminate received data
  buffer[sizeof(unsigned int) + len] = 0;

  return len;
}

void
putOutput(const char* out)
{
  strcpy(msg_data, out);
  unsigned int len = strlen(out);
  unsigned int netlen = htonl(len);
  memcpy(buffer,&netlen,sizeof(netlen));
  write(4, buffer, len + sizeof(netlen));
}

void
ProcessSensation()
{
  // Stime time data

  // This is a sensation to be given to the agent. It begins a
  // thinking cycle. The first time is the simulation time the
  // sensation was generated (also known as the send time) and the
  // second is the time that the sensation is delivered to the agent
  // (also known as the arrive time). If the parameter send agent send
  // time is off, -1 is always sent as the send time, and if the
  // parameter send agent arrive time is off, -1 is always sent as the
  // arrive time. The data is an arbitrary data string generated by
  // the world model. The agent can reply with act messages, and must
  // finish with a done thinking message.

  // 'A'- action
  // (init (teamname RoboLog) (unum 4711))


  static int numSensation = 0;

  switch (numSensation)
    {
    case 0:
      // this is the first received sensation. use the create effector
      // to setup the Sensors and Effectors
      log ("received first sensation ");
      log(msg_data);
      log("\n");
      log("creating agent\n");
      putOutput("A(create)");
      break;

    case 1:
      log("received second sensation ");
      log(msg_data);
      log("\n");
      log("sending init command\n");
      putOutput("A(init (unum 8) (teamname RoboLog))");
      break;

    default:
      log("received another sensation ");
      log(msg_data);

      log("using force effector\n ");
      putOutput("A(force 50 0 0)");
    }

  log("writing done thinking message\n");
  putOutput("D");

   ++numSensation;
}

void
ProcessInitMessage()
{
  // Ddata

  // After startup, this initialization data message is sent to the
  // agent. On a normal startup, data will be empty. On an agent
  // migration (see Section 5.3), the data returned by the old agent
  // processes will be given to the new agent. An initialization done
  // message should be sent once the initialization data has been
  // processed and all other startup is complete.

  log("received init message 'D'\n");

  // init here

  log("writing init done message\n");
  putOutput("I");
}

void
ProcessThinkTimeMessage()
{
  // Ktime

  // This is a think time message that notifies the agent how much
  // thinking time was used for the last thinking cycle. These are
  // only sent if send agent think times is on.

  log ("received a think time message ");
  log (msg_data);
  log ("\n");

  log("writing done thinking message\n");
  putOutput("D");
}

int
main(int /*argc*/, const char *const */*argv*/)
{
  log("AgentTest started\n");

  //
  // perfom any agent init here
  //

  // 'initialization done' message
  while (true)
    {
      if (getInput())
        {
          switch(msg_data[0])
            {
            case 'D' :
              ProcessInitMessage();
              break;

            case 'S' :
              ProcessSensation();
              break;

            case 'K' :
              ProcessThinkTimeMessage();
              break;

            default:
              log("received unknown data: ");
              log(msg_data);
              log("\n");
              break;
            }
        }
    }
}
