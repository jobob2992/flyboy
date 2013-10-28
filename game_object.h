#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <Zeni/Collision.h>
#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Sound.h>
#include <Zeni/Vector3f.h>
#include <String>

class game_object {
  public:
    game_object(const Zeni::String &modl,
        const Zeni::Point3f &corner,
          const Zeni::Vector3f &scale,
          const Zeni::Quaternion &rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f));
    game_object & operator=(const game_object &rhs);
    ~game_object();

    void render();

    const Zeni::Collision::Parallelepiped & get_body() const {return m_body;}

    void disappear();

private:
    void create_body();

    //get the 3D model
    Zeni::Model *m_model;

    //location, size, and rotation
    Zeni::Point3f m_corner;
    Zeni::Vector3f m_scale;
    Zeni::Quaternion m_rotation;
    //for collision
    Zeni::Collision::Parallelepiped m_body;
  };
#endif
