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
      create_body();
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
                            m_rotation * m_scale.get_i(),
                            m_rotation * m_scale.get_j(),
                            m_rotation * m_scale.get_k());
  }


  void game_object::disappear()
  {
      m_corner.z = -1000.0f;
      create_body();
  }