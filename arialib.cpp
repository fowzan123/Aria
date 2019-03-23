#include "Aria.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class ConnHandler
{
public:
  // Constructor
  ConnHandler(ArRobot *robot);
  // Destructor, its just empty
  ~ConnHandler(void) {}
  // to be called if the connection was made
  void connected(void);
  // to call if the connection failed
  void connFail(void);
  // to be called if the connection was lost
  void disconnected(void);
protected:
  // robot pointer
  ArRobot *myRobot;
  // the functor callbacks
  ArFunctorC<ConnHandler> myConnectedCB;
  ArFunctorC<ConnHandler> myConnFailCB;
  ArFunctorC<ConnHandler> myDisconnectedCB;
};

ConnHandler::ConnHandler(ArRobot *robot) :
  myConnectedCB(this, &ConnHandler::connected),
  myConnFailCB(this, &ConnHandler::connFail),
  myDisconnectedCB(this, &ConnHandler::disconnected)

{
  myRobot = robot;
  myRobot->addConnectCB(&myConnectedCB, ArListPos::FIRST);
  myRobot->addFailedConnectCB(&myConnFailCB, ArListPos::FIRST);
  myRobot->addDisconnectNormallyCB(&myDisconnectedCB, ArListPos::FIRST);
  myRobot->addDisconnectOnErrorCB(&myDisconnectedCB, ArListPos::FIRST);
}

// just exit if the connection failed
void ConnHandler::connFail(void)
{
  printf("directMotionDemo connection handler: Failed to connect.\n");
  myRobot->stopRunning();
  Aria::exit(1);
  return;
}

// turn on motors, and off sonar, and off amigobot sounds, when connected
void ConnHandler::connected(void)
{
  printf("directMotionDemo connection handler: Connected\n");
  myRobot->comInt(ArCommands::SONAR, 0);
  myRobot->comInt(ArCommands::ENABLE, 1);
  myRobot->comInt(ArCommands::SOUNDTOG, 0);
}

// lost connection, so just exit
void ConnHandler::disconnected(void)
{
  printf("directMotionDemo connection handler: Lost connection, exiting program.\n");
  Aria::exit(0);
}



int main(int argc, char **argv)
{
  Aria::init();

  ArArgumentParser argParser(&argc, argv);
  argParser.loadDefaultArguments();


  ArRobot robot;
  ArRobotConnector con(&argParser, &robot);

  // the connection handler from above
  ConnHandler ch(&robot);

  if(!Aria::parseArgs())
  {
    Aria::logOptions();
    Aria::exit(1);
    return 1;
  }

  if(!con.connectRobot())
  {
    ArLog::log(ArLog::Normal, "directMotionExample: Could not connect to the robot. Exiting.");

    if(argParser.checkHelpAndWarnUnparsed())
    {
      Aria::logOptions();
	}
    Aria::exit(1);
    return 1;
  }

  ArLog::log(ArLog::Normal, "directMotionExample: Connected.");

  if(!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    Aria::exit(1);
  }
