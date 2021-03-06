/**
    This is the main method for the sensor_board node,
    it will launch use a lot of different classes for the different
    components but feed them all the same node handle so
    that they all run in the same process. This makes it possible
    for them to all communicate with the sensor board directly, 
    as opposed to having to share a serial port between processes, 
    which makes our life a lot easier. 
**/

#include "sensor_board_tortuga.h"

#include "thruster_tortuga.h"
#include "depth_tortuga.h"
#include "power_sensor_tortuga.h"
#include "temp_tortuga.h"
#include "sonar_server.h"
#include "sonar_client.h"
//include your header here, no need to relocate it.

//Sean wrote most of this, direct questions to him



int main(int argc, char **argv) {

    //initialize the ros node we'll use for anything wanting to talk to the sensor board. 
    ros::init(argc, argv, "sensor_board_node");
    std::shared_ptr<ros::NodeHandle> n(new ros::NodeHandle);
    
    //open the sensor board
    std::string sensor_file = "/dev/sensor";
    int fd = openSensorBoard(sensor_file.c_str());
    
    //todo make a static reference to checkerror and call it here
    syncBoard(fd);

    //todo, turn these off at some point. NOTE the cameras do not actually talk to us right now.
    camConnect(fd);
    DVLOn(fd, 1);
    
    ROS_DEBUG("opened the sensor board, fd  =  %i" ,fd );
    
 
    //SG:when you're integrating a new node add a unique_ptr to your node here as well
    std::unique_ptr<SensorBoardTortugaNode> thrusters;
    std::unique_ptr<SensorBoardTortugaNode> depth_sensor;
    std::unique_ptr<SensorBoardTortugaNode> power_sensor;
    std::unique_ptr<SensorBoardTortugaNode> temp_sensor;
    //  std::unique_ptr<SensorBoardTortugaNode> sonar_client;
    //  std::unique_ptr<SensorBoardTortugaNode> sonar_server;
 
 
    
    //TODO:
    //SG: This part may have to be a little different for this node, we may want to read from a config file, or just pass in more arguments
    //to specify which sensors are real vs simlated, for now we'll just make sure it works in the case where everything is the real tortuga version

    //SG: honestly we can put this next bit into a ramnode function, also probably don't want to
    //be using C-strings for this, segfaults or no fun. I'll hack something today but let's clean this up soon ok?

    //SG: think of the best way to handlde this, do we really want to pass "tortuga" as an argument if the sensor board is
    //unique to this robot, we could also honestly abandon the whole passing in argument thing and just launch a different node in the 
    //launch file... 
    
    
    ROS_DEBUG("attempting to initialize nodes\n");
    thrusters.reset(new ThrusterTortugaNode(n, 10, fd, sensor_file));
    depth_sensor.reset(new DepthTortugaNode(n, 10, fd, sensor_file));
    power_sensor.reset(new PowerNodeTortuga(n,10,fd,sensor_file));
    temp_sensor.reset(new TempTortugaNode(n,10,fd,sensor_file));
//    sonar_client.reset(new SonarClientNode(n,10,fd,sensor_file));
//    sonar_server.reset(new SonarServerNode(n,10,fd,sensor_file));
    ROS_DEBUG("nodes initialized, nice!\n");
    //copy the above with your node, just make sure n, fd and sensor_file are the same, not sure if we need rate honestly and I'd like to remove it if possible
 
     
    //this is the main loop for the program, it will continously update the relavent nodes until ros::ok fails.
   while (ros::ok()) {
        thrusters->update();
        ROS_ERROR("thrusters UPDATED");
        depth_sensor->update();
        ROS_ERROR("depth updated");
        //power_sensor->update();
        //temp_sensor->update();
        ros::spinOnce();   //this might be spelled wrong
        //make sure you run your nodes update here.
    }
}

