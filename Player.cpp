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
     fuel(0.0f)
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
	  return std::make_pair (Zeni::Point3f(m_camera.position), 300.0f*(Zeni::Vector3f(m_camera.get_forward())).normalized());
  }
  //jetpack implementation
    void Player::fly()
    {
        no_move.stop();
        if(!fly_timer.is_running())
        {
            fly_timer.set(0);
            fly_timer.start();
        }
        else
        {
            rest_timer.stop();
            fly_timer.start();
            in_air_time = fly_timer.seconds();
            if(fuel > 0.0f)
            {
                fuel -= 1.5f * in_air_time;
            }
            m_velocity.k += 5.0f;
        }
    }

	void Player::port(const Zeni::Point3f &location_)
	{
		m_camera.position = location_;
	}

    void Player::fall() 
    {
        if(fly_timer.is_running())
        {
            fly_timer.set(0);
            fly_timer.stop();
        }
        //no_move.stop();
    }

    bool Player::can_fly()
    {
        if(fuel > 0.0f) return true;
        else return false;
    }
 
    void Player::fuel_up()
    {
        if(!rest_timer.is_running())
        {
            rest_timer.set(0);
            rest_timer.start();
        }
        else
        {
            rest_timer.start();
            rest_time = rest_timer.seconds();
            if(fuel < 200)
            {
                fuel += rest_time * 0.5f;
            }
        }
    }

    float Player::get_time()
    {
        return fuel;
    }

    bool Player::resting()
    {
        bool result = false;
        if(!no_move.is_running())
        {
            no_move.set(0);
            no_move.start();
        }
        return no_move.seconds() > 5.0f;
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
