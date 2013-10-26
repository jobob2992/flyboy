#include <zenilib.h>

#include "Player.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace Crate {

  Player::Player(const Camera &camera_,
         const Vector3f &end_point_b_,
         const float radius_)
   : m_camera(camera_),
     m_end_point_b(end_point_b_),
     m_radius(radius_),
     m_is_on_ground(false),
     fuel(100.0f)
  {
    m_camera.fov_rad = Zeni::Global::pi / 3.0f;

    create_body();
  }

  // Level 2
  void Player::set_position(const Point3f &position) {
    m_camera.position = position;
    create_body();
  }

  void Player::adjust_pitch(const float &phi) {
    const Quaternion backup = m_camera.orientation;
    const Vector3f backup_up = m_camera.get_up();

    m_camera.adjust_pitch(phi);

    if(m_camera.get_up().k < 0.0f && backup_up.k >= 0.0f)
      m_camera.orientation = backup;
  }

  void Player::turn_left_xy(const float &theta) {
    m_camera.turn_left_xy(theta);
  }

  void Player::set_on_ground(const bool &is_on_ground_) {
    m_is_on_ground = is_on_ground_;
    if(m_is_on_ground)
      m_velocity.k = 0.0f;
  }

  void Player::jump() {
    if(m_is_on_ground) {
      m_velocity.k += 150.0f;
      m_is_on_ground = false;
    }
  }

  std::pair <Zeni::Point3f, Zeni::Vector3f> Player::shoot(){
	  return std::make_pair (Zeni::Point3f(m_camera.position), Zeni::Vector3f(m_camera.get_forward()));
  }
  //jetpack implementation
    void Player::fly()
    {        fuel -= 0.5f;
        m_velocity.k += 5.0f;
    }

    void Player::fall() 
    {
      //  while(m_velocity.k > 0.0f)
      //  {
        //    m_velocity.k -= 0.2f;
       // }
    }

    bool Player::can_fly()
    {
        if(fuel > 0.0f) return true;
        else return false;
    }
 
    void Player::fuel_up()
    {
        while(fuel < 100.0f)
        {
            fuel += 0.1f;
        }
    }

    float Player::get_time()
    {
        return fuel;
    }

  void Player::step(const float &time_step) {
    m_camera.position += time_step * m_velocity;
    create_body();
  }
  




  void Player::create_body() {
    Sound &sr = get_Sound();

    m_body = Capsule(m_camera.position,
                     m_camera.position + m_end_point_b,
                     m_radius);

    sr.set_listener_position(m_camera.position);
    sr.set_listener_forward_and_up(m_camera.get_forward(), m_camera.get_up());
    sr.set_listener_velocity(m_velocity);
  }

}
