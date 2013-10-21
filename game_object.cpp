#include <zenilib.h>

#include "game_object.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace game_object {

  game_object::game_object(const Point3f &position,
        const Vector3f &scale,
        const Quaternion &rotation,
		const Vector3f &velocity)
    : /*m_source(new Sound_Source(get_Sounds()["collide"])),*/
    m_position(position),
    m_scale(scale),
    m_rotation(rotation),
	m_velocity(velocity)
  {
    if(!m_instance_count)
      m_model = new Model("models/crate.3ds");
    ++m_instance_count;

    create_body();
  }

  game_object::game_object(const game_object &rhs)
    : m_source(new Sound_Source(get_Sounds()["collide"])),
    m_position(rhs.m_position),
    m_scale(rhs.m_scale),
    m_rotation(rhs.m_rotation),
	m_velocity(rhs.m_velocity)
  {
    ++m_instance_count;

    create_body();
  }

  game_object & game_object::operator=(const game_object &rhs) {
    m_position = rhs.m_position;
    m_scale = rhs.m_scale;
    m_rotation = rhs.m_rotation;
	m_velocity = rhs.m_velocity;

    create_body();

    return *this;
  }

  game_object::~game_object() {
    delete m_source;

    if(!--m_instance_count) {
      delete m_model;
      m_model = 0lu;
    }
  }

  void game_object::render() {
    const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();

    m_model->set_translate(m_position);
    m_model->set_scale(m_scale);
    m_model->set_rotate(rotation.second, rotation.first);

    m_model->render();
  }

  void game_object::collide() {
    if(!m_source->is_playing())
      m_source->play();
  }

  void game_object::move(const float &time) {
	  m_position.x += time * m_velocity.i;
	  m_position.y += time * m_velocity.j;
	  m_position.z += time * m_velocity.k;
  }

  void game_object::accelerate(const Vector3f &diff) {
	  m_velocity.i += diff.i;
	  m_velocity.j += diff.j;
	  m_velocity.k += diff.k;
  }
  
  void game_object::set_on_ground(const bool &is_on_ground) {
    m_is_on_ground = is_on_ground;
    if(m_is_on_ground)
      m_velocity.k = 0.0f;
  }

  void game_object::set_position(const Zeni::Point3f &position) {
	  m_position.x = position.x;
	  m_position.y = position.y;
	  m_position.z = position.z;
  }

  void game_object::create_body() {
    m_body = Parallelepiped(m_position,
                            m_rotation * m_scale.get_i(),
                            m_rotation * m_scale.get_j(),
                            m_rotation * m_scale.get_k());

    m_source->set_position(m_position + m_rotation * m_scale / 2.0f);
  }

  Model * game_object::m_model = 0;
  unsigned long game_object::m_instance_count = 0lu;

}
