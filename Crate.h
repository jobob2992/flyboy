#ifndef CRATE_H
#define CRATE_H

#include <Zeni/Collision.h>
#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Sound.h>
#include <Zeni/Vector3f.h>

namespace Crate {

  class Crate {
  public:
    Crate(
          const Zeni::String &sfx,
          const Zeni::String &modl,
          const Zeni::Point3f &top_center = Zeni::Point3f(0.0f, 0.0f, 0.0f),
          const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
          const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f));
    Crate(const Crate &rhs);
    Crate & operator=(const Crate &rhs);
    ~Crate();

    void render();

    void collide();

    //make a crate disappear
    void disappear();
    void appear();
	const Zeni::Collision::Infinite_Cylinder & get_body() const {return m_body;}
    Zeni::String model_name;
  private:
    void create_body();

    // Level 1
    static Zeni::Model * m_model;
    static unsigned long m_instance_count;

    Zeni::Sound_Source * m_source;

    // Level 2
    Zeni::Point3f m_top_center;
    Zeni::Vector3f m_scale;
    Zeni::Quaternion m_rotation;

    // Level 3
	Zeni::Collision::Infinite_Cylinder m_body; // not motion so much as collision
    bool make_appear;
    Zeni::Point3f origin;
    // Level 4
    // A stationary Crate has no controls
  };

}

#endif
