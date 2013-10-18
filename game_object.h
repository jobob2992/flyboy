#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <Zeni/Collision.h>
#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Sound.h>
#include <Zeni/Vector3f.h>

namespace game_object {

  class game_object {
  public:
    game_object::game_object(const Zeni::Point3f &position = Zeni::Point3f(0.0f, 0.0f, 0.0f),
          const Zeni::Vector3f &scale = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
          const Zeni::Quaternion &rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f),
		  const Zeni::Vector3f &velocity = Zeni::Vector3f(0.0f, 0.0f, 0.0f));
    game_object(const game_object &rhs);
    game_object & operator=(const game_object &rhs);
    ~game_object();

    void render();

    void collide();

	void move(const float &time);

	void accelerate(const Zeni::Vector3f &diff);
	
    void set_on_ground(const bool &is_on_ground_);

	void set_position(const Zeni::Point3f &position);

    const Zeni::Collision::Parallelepiped & get_body() const {return m_body;}

  private:
    void create_body();

    // Level 1
    static Zeni::Model * m_model;
    static unsigned long m_instance_count;

    Zeni::Sound_Source * m_source;

    // Level 2
    Zeni::Point3f m_position;
    Zeni::Vector3f m_scale;
    Zeni::Quaternion m_rotation;
	Zeni::Vector3f m_velocity;
	bool m_is_on_ground;

    // Level 3
    Zeni::Collision::Parallelepiped m_body; // not motion so much as collision

    // Level 4
    // A stationary game_object has no controls
  };

}

#endif
