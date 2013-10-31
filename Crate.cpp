#include <zenilib.h>
#include <iostream>
#include "Crate.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace Crate {

  Crate::Crate(
          const Zeni::String &sfx,
          const Zeni::String &modl,
      const Point3f &top_center,
        const Vector3f &scale_,
        const Quaternion &rotation_)
    : m_source(new Sound_Source(get_Sounds()[sfx])),
    m_top_center(top_center),
    m_scale(scale_),
    m_rotation(rotation_),
    model_name(modl),
    origin(top_center)
  {
    //if(!m_instance_count)
      m_model = new Model(modl);
   // ++m_instance_count;
      
    create_body();
  }

  Crate::Crate(const Crate &rhs)
    : m_source(new Sound_Source(get_Sounds()["coin"])),
	m_top_center(rhs.m_top_center),
    m_scale(rhs.m_scale),
    m_rotation(rhs.m_rotation)
  {
    ++m_instance_count;

    create_body();
  }

  Crate & Crate::operator=(const Crate &rhs) {
	m_top_center = rhs.m_top_center;
    m_scale = rhs.m_scale;
    m_rotation = rhs.m_rotation;

    create_body();

    return *this;
  }

  Crate::~Crate() {
    delete m_source;

    if(!--m_instance_count) {
      delete m_model;
     m_model = 0lu;
    }
  }

  void Crate::render() {
    const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();

	m_model->set_translate(m_top_center);
    m_model->set_scale(m_scale);
    m_model->set_rotate(rotation.second, rotation.first);

    m_model->render();
  }

  void Crate::collide() {
    if(!m_source->is_playing())
      m_source->play();
  }

  void Crate::disappear()
  {
      m_top_center.z = -1000.0f;
      create_body();
      make_appear = false;
  }

  void Crate::create_body() {
	  m_body = Infinite_Cylinder(m_top_center,
								m_top_center - Zeni::Vector3f(0.0f, 0.0f, 1.055 * m_scale.z),
								0.977f * m_scale.x);

	  m_source->set_position(m_top_center - Zeni::Vector3f(0.0f, 0.0f, -0.5275 * m_scale.z));
  }

  void Crate::appear()
  {
      if(!make_appear)
      {
          make_appear = true;
          m_top_center = origin;
          create_body();
      }
  }
  Model * Crate::m_model = 0;
  unsigned long Crate::m_instance_count = 0lu;

}
