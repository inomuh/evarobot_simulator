#include <boost/bind.hpp>
#include <stdio.h>

#include <map>
#include <string>

#include <gazebo/common/Plugin.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"

#include <gazebo/physics/World.hh>
#include <gazebo/physics/HingeJoint.hh>
#include <gazebo/physics/Contact.hh>
#include <gazebo/sensors/Sensor.hh>
#include <gazebo/sensors/sensors.hh>
#include <gazebo/sensors/ContactSensor.hh>
#include <sdf/sdf.hh>
#include <sdf/Param.hh>
#include <gazebo/common/Exception.hh>
#include <gazebo/sensors/SensorTypes.hh>

#include <tf/tf.h>

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"
#include "sensor_msgs/LaserScan.h"

#include "gazebo/sensors/RaySensor.hh"
#include "gazebo/sensors/SensorManager.hh"

#include "im_msgs/BumperState.h"
#include "im_msgs/Bumper.h"

#include "gazebo_msgs/ContactsState.h"


namespace gazebo
{   
  class ROSBumperPlugin : public ModelPlugin
  {   
    public: ROSBumperPlugin()
    {
      // Start up ROS
      std::string name1 = "sensor"; // this is what appears in the rostopics
      int argc = 0;
      ros::init(argc, NULL, name1);      
    }
    public: ~ROSBumperPlugin()
    {
      delete this->nh;
    }

    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
     // Store the pointer to the model
      this->model = _parent;

      // ROS Nodehandle
      this->nh = new ros::NodeHandle("~");


      this->nh = new ros::NodeHandle("~");
      this->pubbumper = this->nh->advertise<im_msgs::Bumper>("sensor/Bumper",1000);

//***********************************************
	  sensors::SensorPtr bumpers[3];
		
      bumpers[0] = sensors::SensorManager::Instance()->GetSensor("bumper0");
      bumpers[1] = sensors::SensorManager::Instance()->GetSensor("bumper1");
      bumpers[2] = sensors::SensorManager::Instance()->GetSensor("bumper2");


	  for(int i = 0; i < 3; i++)
	  {
		  if(!bumpers[i])
			printf("bumper %d is NULL\n", i);
		  this->contactsensors[i] = boost::dynamic_pointer_cast<sensors::ContactSensor>(bumpers[i]);
          if(!this->contactsensors[i])
			printf("contactsensor %d is NULL\n", i);
	  }
 
     
//***********************************************

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      // for gazebo 1.4 or lower it should be ConnectWorldUpdateStart
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ROSBumperPlugin::OnUpdate, this));
          
    }

    // Called by the world update start event
    public: void OnUpdate()
    {
      
           ros::Time current_time, last_time;
            current_time = ros::Time::now();
            last_time = ros::Time::now();

          
			// Publish          
            im_msgs::Bumper totalbumper;
                        
            totalbumper.header.stamp=current_time; //ros::Time::now();
            totalbumper.header.frame_id="base_scan";
            
            totalbumper.state.clear();
            totalbumper.state.resize(3);
            
            
			

            
            for(int i = 0; i < 3; i++)
            {
				im_msgs::BumperState bumper_state;
				
				bumper_state.bumper_state = true;
				
				unsigned int collCount;// = this->contactsensors[i]->GetCollisionContactCount(this->contactsensors[i]->GetCollisionName(this->contactsensors[i]->GetCollisionCount()));
				
				contact_bilgisi = this->contactsensors[i]->GetContacts();
					
				collCount = contact_bilgisi.contact_size();
				
				if(collCount > 0)
					bumper_state.bumper_state = false;
				
				totalbumper.state[i] = bumper_state;	
				
			}
            
           
 
            this->pubbumper.publish(totalbumper);
			// Publish (END)

            ros::spinOnce();
    }


    // Pointer to the model
    private: physics::ModelPtr model;

    private: sensors::ContactSensorPtr contactsensors[3];
    
    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    // ROS Nodehandle
    private: ros::NodeHandle* nh;

	// ROS Publisher
	ros::Publisher pubbumper;
	
	private: gazebo::msgs::Contacts contact_bilgisi;


  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ROSBumperPlugin)
}
