#ifndef CRATE_STATE_H
#define CRATE_STATE_H

#include "game_object.h"
#include "Crate.h"
#include "Player.h"
#include <Zeni/Gamestate.h>
#include <Zeni/Timer.h>


namespace Crate {

  class Crate_State : public Zeni::Gamestate_Base {
    struct Controls {
      Controls() : forward(false), left(false), back(false), right(false) , flying (false){}

      bool forward;
      bool left;
      bool back;
      bool right;
	  bool flying;
    } m_controls;

  public:
    Crate_State();

    void on_push();

    void on_key(const SDL_KeyboardEvent &event);
    void on_mouse_motion(const SDL_MouseMotionEvent &event);

    void perform_logic();

    void render();

  private:
    void partial_step(const float &time_step, const Zeni::Vector3f &velocity);

    Zeni::Time_HQ time_passed;

    Crate m_crate, crate2, crate3, crate4, crate5;
    Player m_player;
    bool m_moved;
    game_object gold, gold2,gold3, gold4;

    //keep track of gold count
    int gold_count;
  };

}

#endif
