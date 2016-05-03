#ifndef EVAROBOT_GAZEBO_WIDGET_HH
#define EVAROBOT_GAZEBO_WIDGET_HH

#include <gazebo/common/Plugin.hh>
#include <gazebo/gui/GuiPlugin.hh>
#ifndef Q_MOC_RUN
#include <gazebo/transport/transport.hh>
#include <gazebo/gui/gui.hh>
#include <gazebo/math/gzmath.hh>
#include <gazebo/math/Vector3.hh>
#include "gazebo/msgs/msgs.hh"


//#include <ignition/math/Pose3.hh>
//#include <ignition/math/Vector3.hh>


#endif

namespace gazebo
{
    class GAZEBO_VISIBLE EvarobotWidget : public GUIPlugin
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent widget
      public: EvarobotWidget();

      /// \brief Destructor
      public: virtual ~EvarobotWidget();
      
      /// \brief Callback trigged when the button is pressed.
      protected slots: void AddBoxButton();
      protected slots: void AddCylinderButton();
      protected slots: void AddSphereButton();

      /// \brief Counter used to create unique model names
      private: unsigned int counter;

      /// \brief Node used to establish communication with gzserver.
      private: transport::NodePtr node;

      /// \brief Publisher of factory messages.
      private: transport::PublisherPtr objectPub;


      // Box Widgets
      private: QLineEdit *textBoxPoseX;
      private: QLineEdit *textBoxPoseY;
      private: QLineEdit *textBoxPoseZ;
      
      private: QLineEdit *textSizeX;
      private: QLineEdit *textSizeY;
      private: QLineEdit *textSizeZ;
      private: QComboBox *combobox_color;
      
      
      // Cylinder Widgets
      private: QLineEdit *textCylinderPoseX;
      private: QLineEdit *textCylinderPoseY;
      private: QLineEdit *textCylinderPoseZ;
      
      private: QLineEdit *textSizeCylinderR;
      private: QLineEdit *textSizeCylinderL;
      private: QComboBox *combocylinder_color;
      
      
      // Sphere Widgets
      private: QLineEdit *textSpherePoseX;
      private: QLineEdit *textSpherePoseY;
      private: QLineEdit *textSpherePoseZ;
      
      private: QLineEdit *textSizeSphereR;
            
      private: QComboBox *combosphere_color;
      
      
    };
}
#endif

