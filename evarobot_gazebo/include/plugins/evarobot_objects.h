#include <tuple>
#include <random>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include <gazebo/common/Plugin.hh>
#include "gazebo/gazebo.hh"
#include <gazebo/math/gzmath.hh>



namespace gazebo {

  class EvarobotObjects: public WorldPlugin {

    public:
      EvarobotObjects();
      void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf);

      void AddObject(ConstGeometryPtr &msg);

    private:

      /// \brief load from maze_base/model.sdf
      sdf::ElementPtr LoadModel();


      msgs::Geometry *CreateBoxGeometry(float x, float y, float z);
      msgs::Geometry *CreateCylinderGeometry(float r, float h);
      msgs::Geometry *CreateSphereGeometry(float r);

			void CreateBoxVisual(float x, float y, float z, sdf::ElementPtr  element, int color);
			void CreateBoxCollision(float x, float y, float z, sdf::ElementPtr  element);
			
			void CreateCylinderVisual(float radius, float length, sdf::ElementPtr element, int color);
			void CreateCylinderCollision(float radius, float length, sdf::ElementPtr  element);

			void CreateSphereVisual(float radius, sdf::ElementPtr element, int color);
			void CreateSphereCollision(float radius, sdf::ElementPtr  element);

      std::list<sdf::ElementPtr> all_wall_elements;

      transport::NodePtr node;
      transport::SubscriberPtr regen_sub;
      transport::SubscriberPtr object_sub;

      physics::WorldPtr parent;

      sdf::SDFPtr modelSDF;
            
      int objectNo;
     
        
  };
}


