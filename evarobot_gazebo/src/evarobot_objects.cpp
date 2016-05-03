#include <plugins/evarobot_objects.h>

#include <ignition/math/Pose3.hh>

#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>

namespace gazebo
{

EvarobotObjects::EvarobotObjects() {objectNo = 0;}

void EvarobotObjects::Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf)
{
  this->parent = _parent;
  node = transport::NodePtr(new transport::Node());
  node->Init(parent->GetName());
  object_sub = node->Subscribe("~/evarobot_widget/object", &EvarobotObjects::AddObject, this);
}

void EvarobotObjects::AddObject(ConstGeometryPtr &msg)
{
	
	sdf::ElementPtr model = LoadModel();
  sdf::ElementPtr base_link = model->GetElement("link");
	float posX, posY, posZ;
	
	for(int i = 0; i < msg->points_size(); i++)
	{
		posX = msg->points(i).x();
		posY = msg->points(i).y();
		posZ = msg->points(i).z();
	}
	
	if(msg->type() == gazebo::msgs::Geometry::BOX 
	|| msg->type() == gazebo::msgs::Geometry::PLANE 
	|| msg->type() == gazebo::msgs::Geometry::MESH)
	{
		if(msg->type() == gazebo::msgs::Geometry::BOX)
			CreateBoxVisual(msg->box().size().x(), msg->box().size().y(), msg->box().size().z(), base_link, 0);
		else if(msg->type() == gazebo::msgs::Geometry::PLANE)
			CreateBoxVisual(msg->box().size().x(), msg->box().size().y(), msg->box().size().z(), base_link, 1);
		else if(msg->type() == gazebo::msgs::Geometry::MESH)
			CreateBoxVisual(msg->box().size().x(), msg->box().size().y(), msg->box().size().z(), base_link, 2);	
		CreateBoxCollision(msg->box().size().x(), msg->box().size().y(), msg->box().size().z(), base_link);
		
	}
	else if(msg->type() == gazebo::msgs::Geometry::CYLINDER 
	|| msg->type() == gazebo::msgs::Geometry::IMAGE 
	|| msg->type() == gazebo::msgs::Geometry::TRIANGLE_FAN)
	{
		if(msg->type() == gazebo::msgs::Geometry::CYLINDER)
			CreateCylinderVisual(msg->cylinder().radius(), msg->cylinder().length(), base_link, 0);
		else if(msg->type() == gazebo::msgs::Geometry::IMAGE)
			CreateCylinderVisual(msg->cylinder().radius(), msg->cylinder().length(), base_link, 1);
		else if(msg->type() == gazebo::msgs::Geometry::TRIANGLE_FAN)
			CreateCylinderVisual(msg->cylinder().radius(), msg->cylinder().length(), base_link, 2);
		
		CreateCylinderCollision(msg->cylinder().radius(), msg->cylinder().length(), base_link);
	
	}
	else if(msg->type() == gazebo::msgs::Geometry::SPHERE 
	|| msg->type() == gazebo::msgs::Geometry::HEIGHTMAP 
	|| msg->type() == gazebo::msgs::Geometry::LINE_STRIP)
	{
	
		if(msg->type() == gazebo::msgs::Geometry::SPHERE)
			CreateSphereVisual(msg->sphere().radius(), base_link, 0);
		else if(msg->type() == gazebo::msgs::Geometry::HEIGHTMAP)
			CreateSphereVisual(msg->sphere().radius(), base_link, 1);
		else if(msg->type() == gazebo::msgs::Geometry::LINE_STRIP)
			CreateSphereVisual(msg->sphere().radius(), base_link, 2);
		
		CreateSphereCollision(msg->sphere().radius(), base_link);		
	}
	else
	{
		gzmsg << "Evarobot Map: Undefined Object" << std::endl;
	}

	//load maze from file
	//gzmsg << "loading from file " << maze_filename << std::endl;
	  
	
	// -------------------------------------------------------------------------
	
	std::string element_name = "object" + std::to_string(objectNo);
  model->GetAttribute("name")->Set(element_name);
	model->GetElement("pose")->Set(
    math::Pose(math::Vector3(posX, posY, posZ), math::Quaternion(0, 0, 0)));
  parent->InsertModelSDF(*modelSDF);
  
  objectNo++;
	
}


void EvarobotObjects::CreateSphereVisual(float radius, sdf::ElementPtr element, int color)
{
	
	msgs::Pose * visual_pose = new msgs::Pose;
	visual_pose->mutable_position()->set_x(0.0);
	visual_pose->mutable_position()->set_y(0.0);
	visual_pose->mutable_position()->set_z(0.0);
	visual_pose->mutable_orientation()->set_z(sin(0.0/2));
	visual_pose->mutable_orientation()->set_w(cos(0.0/2));
	
  msgs::Pose * paint_visual_pose = new msgs::Pose;
	paint_visual_pose->mutable_position()->set_x(0.0);
	paint_visual_pose->mutable_position()->set_y(0.0);
	paint_visual_pose->mutable_position()->set_z(0.0);
	paint_visual_pose->mutable_orientation()->set_z(sin(0.0/2));
	paint_visual_pose->mutable_orientation()->set_w(cos(0.0/2));


  msgs::Geometry *visual_geo = EvarobotObjects::CreateSphereGeometry(radius);
  msgs::Geometry *paint_visual_geo = EvarobotObjects::CreateSphereGeometry(radius+0.01);

  msgs::Visual visual;
  std::string visual_name = "object_v" + std::to_string(objectNo);
  visual.set_name(visual_name);
  visual.set_allocated_geometry(visual_geo);
  visual.set_allocated_pose(visual_pose);

  msgs::Material_Script *paint_script = new msgs::Material_Script();
  std::string *uri = paint_script->add_uri();
  *uri = "file://media/materials/scripts/gazebo.material";
  if(color == 0)
		paint_script->set_name("Gazebo/Red");
	else if(color == 1)
		paint_script->set_name("Gazebo/Green");
	else if(color == 2)
		paint_script->set_name("Gazebo/Blue");	

  msgs::Material *paint_material = new msgs::Material();
  paint_material->set_allocated_script(paint_script);

  msgs::Visual paint_visual;
  std::string paint_visual_name = "object_p" + std::to_string(objectNo);
  paint_visual.set_name(paint_visual_name);
  paint_visual.set_allocated_geometry(paint_visual_geo);
  paint_visual.set_allocated_pose(paint_visual_pose);
  paint_visual.set_allocated_material(paint_material);

  sdf::ElementPtr visualElem = msgs::VisualToSDF(visual);
  sdf::ElementPtr visualPaintElem = msgs::VisualToSDF(paint_visual);
  
  element->InsertElement(visualPaintElem);
  element->InsertElement(visualElem);	
}

void EvarobotObjects::CreateCylinderVisual(float radius, float length, sdf::ElementPtr element, int color)
{
	
	msgs::Pose * visual_pose = new msgs::Pose;
	visual_pose->mutable_position()->set_x(0.0);
	visual_pose->mutable_position()->set_y(0.0);
	visual_pose->mutable_position()->set_z(0.0);
	visual_pose->mutable_orientation()->set_z(sin(0.0/2));
	visual_pose->mutable_orientation()->set_w(cos(0.0/2));
	
  msgs::Pose * paint_visual_pose = new msgs::Pose;
	paint_visual_pose->mutable_position()->set_x(0.0);
	paint_visual_pose->mutable_position()->set_y(0.0);
	paint_visual_pose->mutable_position()->set_z(0.0);
	paint_visual_pose->mutable_orientation()->set_z(sin(0.0/2));
	paint_visual_pose->mutable_orientation()->set_w(cos(0.0/2));


  msgs::Geometry *visual_geo = EvarobotObjects::CreateCylinderGeometry(radius, length);
  msgs::Geometry *paint_visual_geo = EvarobotObjects::CreateCylinderGeometry(radius+0.01, length+0.01);

  msgs::Visual visual;
  std::string visual_name = "object_v" + std::to_string(objectNo);
  visual.set_name(visual_name);
  visual.set_allocated_geometry(visual_geo);
  visual.set_allocated_pose(visual_pose);

  msgs::Material_Script *paint_script = new msgs::Material_Script();
  std::string *uri = paint_script->add_uri();
  *uri = "file://media/materials/scripts/gazebo.material";
  if(color == 0)
		paint_script->set_name("Gazebo/Red");
	else if(color == 1)
		paint_script->set_name("Gazebo/Green");
	else if(color == 2)
		paint_script->set_name("Gazebo/Blue");	

  msgs::Material *paint_material = new msgs::Material();
  paint_material->set_allocated_script(paint_script);

  msgs::Visual paint_visual;
  std::string paint_visual_name = "object_p" + std::to_string(objectNo);
  paint_visual.set_name(paint_visual_name);
  paint_visual.set_allocated_geometry(paint_visual_geo);
  paint_visual.set_allocated_pose(paint_visual_pose);
  paint_visual.set_allocated_material(paint_material);

  sdf::ElementPtr visualElem = msgs::VisualToSDF(visual);
  sdf::ElementPtr visualPaintElem = msgs::VisualToSDF(paint_visual);
  
  element->InsertElement(visualPaintElem);
  element->InsertElement(visualElem);
}


void EvarobotObjects::CreateBoxVisual(float x, float y, float z, sdf::ElementPtr element, int color)
{
	
	msgs::Pose * visual_pose = new msgs::Pose;
	visual_pose->mutable_position()->set_x(0.0);
	visual_pose->mutable_position()->set_y(0.0);
	visual_pose->mutable_position()->set_z(0.0);
	visual_pose->mutable_orientation()->set_z(sin(0.0/2));
	visual_pose->mutable_orientation()->set_w(cos(0.0/2));
	
  msgs::Pose * paint_visual_pose = new msgs::Pose;
	paint_visual_pose->mutable_position()->set_x(0.0);
	paint_visual_pose->mutable_position()->set_y(0.0);
	paint_visual_pose->mutable_position()->set_z(0.0);
	paint_visual_pose->mutable_orientation()->set_z(sin(0.0/2));
	paint_visual_pose->mutable_orientation()->set_w(cos(0.0/2));


  msgs::Geometry *visual_geo = EvarobotObjects::CreateBoxGeometry(x, y, z);
  msgs::Geometry *paint_visual_geo = EvarobotObjects::CreateBoxGeometry(x+0.01, y+0.01, z+0.01);

  msgs::Visual visual;
  std::string visual_name = "object_v" + std::to_string(objectNo);
  visual.set_name(visual_name);
  visual.set_allocated_geometry(visual_geo);
  visual.set_allocated_pose(visual_pose);

  msgs::Material_Script *paint_script = new msgs::Material_Script();
  std::string *uri = paint_script->add_uri();
  *uri = "file://media/materials/scripts/gazebo.material";
  if(color == 0)
		paint_script->set_name("Gazebo/Red");
	else if(color == 1)
		paint_script->set_name("Gazebo/Green");
	else if(color == 2)
		paint_script->set_name("Gazebo/Blue");	

  msgs::Material *paint_material = new msgs::Material();
  paint_material->set_allocated_script(paint_script);

  msgs::Visual paint_visual;
  std::string paint_visual_name = "object_p" + std::to_string(objectNo);
  paint_visual.set_name(paint_visual_name);
  paint_visual.set_allocated_geometry(paint_visual_geo);
  paint_visual.set_allocated_pose(paint_visual_pose);
  paint_visual.set_allocated_material(paint_material);

  sdf::ElementPtr visualElem = msgs::VisualToSDF(visual);
  sdf::ElementPtr visualPaintElem = msgs::VisualToSDF(paint_visual);
  
  element->InsertElement(visualPaintElem);
  element->InsertElement(visualElem);
}

void EvarobotObjects::CreateSphereCollision(float radius, sdf::ElementPtr  element)
{
	msgs::Pose * collision_pose = new msgs::Pose;
	collision_pose->mutable_position()->set_x(0);
	collision_pose->mutable_position()->set_y(0);
	collision_pose->mutable_position()->set_z(0);
	collision_pose->mutable_orientation()->set_z(sin(0.0/2));
	collision_pose->mutable_orientation()->set_w(cos(0.0/2));

  msgs::Geometry *collision_geo = EvarobotObjects::CreateSphereGeometry(radius);

	msgs::Collision collision;
	std::string collision_name = "object_c" + std::to_string(objectNo);

  collision.set_name(collision_name);
	collision.set_allocated_geometry(collision_geo);
	collision.set_allocated_pose(collision_pose);

	sdf::ElementPtr collisionElem = msgs::CollisionToSDF(collision);
	
	element->InsertElement(collisionElem);
}


void EvarobotObjects::CreateCylinderCollision(float radius, float length, sdf::ElementPtr  element)
{
	msgs::Pose * collision_pose = new msgs::Pose;
	collision_pose->mutable_position()->set_x(0);
	collision_pose->mutable_position()->set_y(0);
	collision_pose->mutable_position()->set_z(0);
	collision_pose->mutable_orientation()->set_z(sin(0.0/2));
	collision_pose->mutable_orientation()->set_w(cos(0.0/2));


  msgs::Geometry *collision_geo = EvarobotObjects::CreateCylinderGeometry(radius, length);
	
	msgs::Collision collision;
	std::string collision_name = "object_c" + std::to_string(objectNo);

  collision.set_name(collision_name);
	collision.set_allocated_geometry(collision_geo);
	collision.set_allocated_pose(collision_pose);

	sdf::ElementPtr collisionElem = msgs::CollisionToSDF(collision);
	
	element->InsertElement(collisionElem);
}

void EvarobotObjects::CreateBoxCollision(float x, float y, float z, sdf::ElementPtr  element)
{
	
	msgs::Pose * collision_pose = new msgs::Pose;
	collision_pose->mutable_position()->set_x(0);
	collision_pose->mutable_position()->set_y(0);
	collision_pose->mutable_position()->set_z(0);
	collision_pose->mutable_orientation()->set_z(sin(0.0/2));
	collision_pose->mutable_orientation()->set_w(cos(0.0/2));


  msgs::Geometry *collision_geo = EvarobotObjects::CreateBoxGeometry(x, y, z);
	
	msgs::Collision collision;
	std::string collision_name = "object_c" + std::to_string(objectNo);

  collision.set_name(collision_name);
	collision.set_allocated_geometry(collision_geo);
	collision.set_allocated_pose(collision_pose);

	sdf::ElementPtr collisionElem = msgs::CollisionToSDF(collision);
	
	element->InsertElement(collisionElem);

}

msgs::Geometry *EvarobotObjects::CreateCylinderGeometry(float r, float l)
{
  msgs::CylinderGeom *cylinder = new msgs::CylinderGeom();
  cylinder->set_radius(r);
  cylinder->set_length(l);

  msgs::Geometry *geo = new msgs::Geometry();
  geo->set_type(msgs::Geometry_Type_CYLINDER);
  geo->set_allocated_cylinder(cylinder);

  return geo;
}

msgs::Geometry *EvarobotObjects::CreateSphereGeometry(float r)
{
  msgs::SphereGeom *sphere = new msgs::SphereGeom();
  sphere->set_radius(r);

  msgs::Geometry *geo = new msgs::Geometry();
  geo->set_type(msgs::Geometry_Type_SPHERE);
  geo->set_allocated_sphere(sphere);

  return geo;
}


msgs::Geometry *EvarobotObjects::CreateBoxGeometry(float x, float y, float z)
{
  msgs::Vector3d *size = new msgs::Vector3d();
  size->set_x(x);
  size->set_y(y);
  size->set_z(z);

  msgs::BoxGeom *box = new msgs::BoxGeom();
  box->set_allocated_size(size);

  msgs::Geometry *geo = new msgs::Geometry();
  geo->set_type(msgs::Geometry_Type_BOX);
  geo->set_allocated_box(box);

  return geo;
}

sdf::ElementPtr EvarobotObjects::LoadModel()
{
  modelSDF.reset(new sdf::SDF);
  
  sdf::initFile("root.sdf", modelSDF);
  //sdf::readFile("/home/evarobot/gzmaze/maze_base/model.sdf", modelSDF);
    
  return modelSDF->root->GetElement("model");
}

GZ_REGISTER_WORLD_PLUGIN(EvarobotObjects)
}

