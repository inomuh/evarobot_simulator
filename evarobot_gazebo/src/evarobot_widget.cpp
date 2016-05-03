#include <sstream>
#include <plugins/evarobot_widget.h>


using namespace gazebo;

// Register this plugin with the simulator
GZ_REGISTER_GUI_PLUGIN(EvarobotWidget)

/////////////////////////////////////////////////
EvarobotWidget::EvarobotWidget()
  : GUIPlugin()	
{
  this->counter = 0;
  
  // Set the frame background and foreground colors
  this->setStyleSheet(
    "QFrame { background-color : rgba(100, 100, 100, 255); color : rgba(224, 224, 224, 224) };");

  // Create layout
  QVBoxLayout *mainLayout = new QVBoxLayout;
  QFrame *mainFrame = new QFrame();
  
  QGridLayout *gridlayout = new QGridLayout;

  
	QPushButton *box_button = new QPushButton(tr("Add BOX"));
  connect(box_button, SIGNAL(clicked()), this, SLOT(AddBoxButton()));
	box_button->setStyleSheet("border-radius: 10px; padding: 2px 4px;");
	
	QPushButton *cylinder_button = new QPushButton(tr("Add CYLINDER"));
  connect(cylinder_button, SIGNAL(clicked()), this, SLOT(AddCylinderButton()));
	cylinder_button->setStyleSheet("border-radius: 10px; padding: 2px 4px;");
	
	QPushButton *sphere_button = new QPushButton(tr("Add SPHERE"));
  connect(sphere_button, SIGNAL(clicked()), this, SLOT(AddSphereButton()));
	sphere_button->setStyleSheet("border-radius: 10px; padding: 2px 4px;");
	
	
	// ComboBox Color
	combobox_color = new QComboBox;
	combobox_color->addItem(tr("Red"));
	combobox_color->addItem(tr("Green"));
	combobox_color->addItem(tr("Blue"));
	
	combocylinder_color = new QComboBox;
	combocylinder_color->addItem(tr("Red"));
	combocylinder_color->addItem(tr("Green"));
	combocylinder_color->addItem(tr("Blue"));
	
	combosphere_color = new QComboBox;
	combosphere_color->addItem(tr("Red"));
	combosphere_color->addItem(tr("Green"));
	combosphere_color->addItem(tr("Blue"));
	
	
	// Pose Text BOX
	textBoxPoseX = new QLineEdit;
  textBoxPoseX->setContentsMargins(1, 1, 1, 1);
  textBoxPoseX->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textBoxPoseX->setObjectName("objectPoseX");
  textBoxPoseX->setFixedHeight(20);
  
  textBoxPoseY = new QLineEdit;
  textBoxPoseY->setContentsMargins(1, 1, 1, 1);
  textBoxPoseY->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textBoxPoseY->setObjectName("objectPoseY");
  textBoxPoseY->setFixedHeight(20);
  
  textBoxPoseZ = new QLineEdit;
  textBoxPoseZ->setContentsMargins(1, 1, 1, 1);
  textBoxPoseZ->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textBoxPoseZ->setObjectName("objectPoseZ");
  textBoxPoseZ->setFixedHeight(20);

	// Pose Text CYLINDER
	textCylinderPoseX = new QLineEdit;
  textCylinderPoseX->setContentsMargins(1, 1, 1, 1);
  textCylinderPoseX->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textCylinderPoseX->setObjectName("objectPoseX");
  textCylinderPoseX->setFixedHeight(20);
  
  textCylinderPoseY = new QLineEdit;
  textCylinderPoseY->setContentsMargins(1, 1, 1, 1);
  textCylinderPoseY->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textCylinderPoseY->setObjectName("objectPoseY");
  textCylinderPoseY->setFixedHeight(20);
  
  textCylinderPoseZ = new QLineEdit;
  textCylinderPoseZ->setContentsMargins(1, 1, 1, 1);
  textCylinderPoseZ->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textCylinderPoseZ->setObjectName("objectPoseZ");
  textCylinderPoseZ->setFixedHeight(20);
  
  
  // Pose Text SPHERE
	textSpherePoseX = new QLineEdit;
  textSpherePoseX->setContentsMargins(1, 1, 1, 1);
  textSpherePoseX->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSpherePoseX->setObjectName("objectPoseX");
  textSpherePoseX->setFixedHeight(20);
  
  textSpherePoseY = new QLineEdit;
  textSpherePoseY->setContentsMargins(1, 1, 1, 1);
  textSpherePoseY->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSpherePoseY->setObjectName("objectPoseY");
  textSpherePoseY->setFixedHeight(20);
  
  textSpherePoseZ = new QLineEdit;
  textSpherePoseZ->setContentsMargins(1, 1, 1, 1);
  textSpherePoseZ->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSpherePoseZ->setObjectName("objectPoseZ");
  textSpherePoseZ->setFixedHeight(20);    
  
  // Box Size Text
  textSizeX = new QLineEdit;
  textSizeX->setContentsMargins(1, 1, 1, 1);
  textSizeX->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSizeX->setObjectName("objectSizeX");
  textSizeX->setFixedHeight(20);
  
  textSizeY = new QLineEdit;
  textSizeY->setContentsMargins(1, 1, 1, 1);
  textSizeY->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSizeY->setObjectName("objectSizeY");
  textSizeY->setFixedHeight(20);
  
  textSizeZ = new QLineEdit;
  textSizeZ->setContentsMargins(1, 1, 1, 1);
  textSizeZ->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSizeZ->setObjectName("objectSizeZ");
  textSizeZ->setFixedHeight(20);
  
  // Cylinder Size Text
  textSizeCylinderR = new QLineEdit;
  textSizeCylinderR->setContentsMargins(1, 1, 1, 1);
  textSizeCylinderR->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSizeCylinderR->setObjectName("objectSizeR");
  textSizeCylinderR->setFixedHeight(20);
  
  textSizeCylinderL = new QLineEdit;
  textSizeCylinderL->setContentsMargins(1, 1, 1, 1);
  textSizeCylinderL->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSizeCylinderL->setObjectName("objectSizeL");
  textSizeCylinderL->setFixedHeight(20);
  
	// Sphere Size Text
  textSizeSphereR = new QLineEdit;
  textSizeSphereR->setContentsMargins(1, 1, 1, 1);
  textSizeSphereR->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  textSizeSphereR->setObjectName("objectSizeSphere");
  textSizeSphereR->setFixedHeight(20);
  
	QLabel * headerlabel = new QLabel("Add Object");
	headerlabel->setAlignment(Qt::AlignCenter);
	
	
	// Add Widgets
	
	gridlayout->addWidget(headerlabel, 0, 0, 1, 4);
	
	QLabel *label_posx = new QLabel("X Position(m): ");
	QLabel *label_posy = new QLabel("Y Position(m): ");
	QLabel *label_posz = new QLabel("Z Position(m): ");
	
	QLabel *label_sizex = new QLabel("Size X / R / R (m): ");
	QLabel *label_sizey = new QLabel("Size Y / L (m): ");
	QLabel *label_sizez = new QLabel("Size Z (m): ");
	
	
  gridlayout->addWidget(label_posx,2,0);
  gridlayout->addWidget(label_posy,3,0);
  gridlayout->addWidget(label_posz,4,0);
  gridlayout->addWidget(label_sizex,5,0);
  gridlayout->addWidget(label_sizey,6,0);
  gridlayout->addWidget(label_sizez,7,0);
	
  gridlayout->addWidget(combobox_color,1,1);
  gridlayout->addWidget(textBoxPoseX,2,1);
  gridlayout->addWidget(textBoxPoseY,3,1);
  gridlayout->addWidget(textBoxPoseZ,4,1);
  gridlayout->addWidget(textSizeX,5,1);
  gridlayout->addWidget(textSizeY,6,1);
  gridlayout->addWidget(textSizeZ,7,1);
  gridlayout->addWidget(box_button,8,1);
  
  
  gridlayout->addWidget(combocylinder_color,1,2);
  gridlayout->addWidget(textCylinderPoseX,2,2);
  gridlayout->addWidget(textCylinderPoseY,3,2);
  gridlayout->addWidget(textCylinderPoseZ,4,2);
  gridlayout->addWidget(textSizeCylinderR,5,2);
  gridlayout->addWidget(textSizeCylinderL,6,2);
  gridlayout->addWidget(cylinder_button,8,2);
  
  gridlayout->addWidget(combosphere_color,1,3);
  gridlayout->addWidget(textSpherePoseX,2,3);
  gridlayout->addWidget(textSpherePoseY,3,3);
  gridlayout->addWidget(textSpherePoseZ,4,3);
  gridlayout->addWidget(textSizeSphereR,5,3);
  gridlayout->addWidget(sphere_button,8,3);
  
	
	mainFrame->setLayout(gridlayout);
  mainLayout->addWidget(mainFrame);
  
  gridlayout->setContentsMargins(2, 2, 2, 2);
  mainLayout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(mainLayout);

  this->move(10, 10);
  this->resize(500, 250);

  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();
  this->objectPub = this->node->Advertise<msgs::Geometry>("~/evarobot_widget/object");
}

/////////////////////////////////////////////////
EvarobotWidget::~EvarobotWidget()
{
}


/////////////////////////////////////////////////
void EvarobotWidget::AddBoxButton()
{
	msgs::Geometry msg;
	
	// Set Position
	msgs::Vector3d * pos = msg.add_points();
			
	pos->set_x(textBoxPoseX->text().toFloat());
	pos->set_y(textBoxPoseY->text().toFloat());
	pos->set_z(textBoxPoseZ->text().toFloat());	
	
	// Set Color
	if(combobox_color->currentIndex() == 0)
		msg.set_type(gazebo::msgs::Geometry::BOX);
	else if(combobox_color->currentIndex() == 1)
		msg.set_type(gazebo::msgs::Geometry::PLANE);
	else if(combobox_color->currentIndex() == 2)
		msg.set_type(gazebo::msgs::Geometry::MESH);
		
	// Set Geometry Size	
	msg.mutable_box()->mutable_size()->set_x(textSizeX->text().toFloat());
	msg.mutable_box()->mutable_size()->set_y(textSizeY->text().toFloat());
	msg.mutable_box()->mutable_size()->set_z(textSizeZ->text().toFloat());

	
	
	this->objectPub->Publish(msg);
}

void EvarobotWidget::AddCylinderButton()
{
	msgs::Geometry msg;
	
	// Set Position
	msgs::Vector3d * pos = msg.add_points();
			
	pos->set_x(textCylinderPoseX->text().toFloat());
	pos->set_y(textCylinderPoseY->text().toFloat());
	pos->set_z(textCylinderPoseZ->text().toFloat());
	
	
	// Set Color
	if(combocylinder_color->currentIndex() == 0)
		msg.set_type(gazebo::msgs::Geometry::CYLINDER);
	else if(combocylinder_color->currentIndex() == 1)
		msg.set_type(gazebo::msgs::Geometry::IMAGE);
	else if(combocylinder_color->currentIndex() == 2)
		msg.set_type(gazebo::msgs::Geometry::TRIANGLE_FAN);
		
	// Set Geometry Size
	msg.mutable_cylinder()->set_radius(textSizeCylinderR->text().toFloat());
	msg.mutable_cylinder()->set_length(textSizeCylinderL->text().toFloat());
	
	this->objectPub->Publish(msg);
	
}

void EvarobotWidget::AddSphereButton()
{
	
	msgs::Geometry msg;
	
	// Set Position
	msgs::Vector3d * pos = msg.add_points();
			
	pos->set_x(textSpherePoseX->text().toFloat());
	pos->set_y(textSpherePoseY->text().toFloat());
	pos->set_z(textSpherePoseZ->text().toFloat());
	
	// Set Type
	if(combosphere_color->currentIndex() == 0)
		msg.set_type(gazebo::msgs::Geometry::SPHERE);
	else if(combosphere_color->currentIndex() == 1)
		msg.set_type(gazebo::msgs::Geometry::HEIGHTMAP);
	else if(combosphere_color->currentIndex() == 2)
		msg.set_type(gazebo::msgs::Geometry::LINE_STRIP);
			
	// Set Geometry Size
	msg.mutable_sphere()->set_radius(textSizeSphereR->text().toFloat());
	
	this->objectPub->Publish(msg);
}


