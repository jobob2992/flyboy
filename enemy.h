#ifndef ENEMY_H
#define ENEMY_H

#include "Player.h"
#include "game_object.h"
#include "Crate.h"

class enemy : public game_object {
  public:
      explicit enemy(const float &chase_speed,const Zeni::String &modl,
        const Zeni::Point3f &corner,
          const Zeni::Vector3f &scale,
          const Zeni::Quaternion &rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f)): game_object(modl,corner,scale,rotation),
          go_chase(true),speed(chase_speed), direction(Zeni::Point3f(0,0,0)){}

      void chase(Crate::Player &player, float &step);

      //rest a bit after successfully attack
      void rest();
      //go chase after a certain time
      void cont_chase();
      void calc_dir();
private:
    Zeni::Point3f user_pos;
    Zeni::Chronometer<Zeni::Time> chill;
    Zeni::Chronometer<Zeni::Time> aim;
    Zeni::Point3f direction;
    bool go_chase;
    float speed;
  };
#endif
