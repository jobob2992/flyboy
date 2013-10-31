#include <zenilib.h>
#include <String>
#include "game_object.h"

using namespace Zeni;
using namespace Zeni::Collision;


game_object::game_object(const Zeni::String &modl,
        const Zeni::Point3f &corner,
          const Zeni::Vector3f &scale,
          const Zeni::Quaternion &rotation)
  {
      m_model = new Model(modl);
      m_corner = corner;
      m_scale = scale;
      m_rotation = rotation;
      original_pos = m_corner;
      create_body();
      make_appear = false;
  }

  game_object & game_object::operator=(const game_object &rhs) {
      return *this;
  }

  game_object::~game_object() {
      delete m_model;
  }
  
  void game_object::render()
  {
    const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();
    m_model->set_translate(m_corner);
    m_model->set_scale(m_scale);
    m_model->set_rotate(rotation.second, rotation.first);
    m_model->render();
  }


  void game_object::create_body() 
  {
    m_body = Parallelepiped(m_corner,
                            m_rotation * 3.0f * m_scale.get_i(),
                            m_rotation * 1.78f * m_scale.get_j(),
                            m_rotation * 0.833 * m_scale.get_k());
  }


  void game_object::disappear()
  {
      m_corner.z = -1000.0f;
      create_body();
  }

  void game_object::appear()
  {
      if(!make_appear)
      {
          make_appear = true;
          m_corner = original_pos;
          create_body();
      }
  }