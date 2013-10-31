#ifndef CRATE_STATE_H
#define CRATE_STATE_H

#include "game_object.h"
#include "Crate.h"
#include "Player.h"
#include "game_object.h"
#include "enemy.h"
#include "port_disc.h"
#include <Zeni/Gamestate.h>
#include <Zeni/Timer.h>
#include <set>


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
	void on_mouse_button(const SDL_MouseButtonEvent &event);

    void perform_logic();

    //render all the enemies in the set
    void render_set(std::set<enemy*> &input);

    //render all the game objects in the set
    void render_set(std::set<game_object*> &input);

    //render all the crates in the set
    void render_crate_set(std::set<Crate*> &input);

    void render();

    
  private:
    void partial_step(const float &time_step, const Zeni::Vector3f &velocity);

    Zeni::Time_HQ time_passed;

    Crate m_crate, crate2, crate3, crate4, crate5,crate6;
    Player m_player;
	port_disc::port_disc* m_disc;
	bool disc;

    bool m_moved;
	bool m_shot;
	bool port;
    bool bounced;
    int lvl;
    //game_object gold;
    game_object gold, gold2,gold3, gold4,gold5;
    enemy e1,e2,e3;
    std::set<game_object*> gold_set;
    std::set<Crate*> crate_set;
    std::set<enemy*> enemy_set;
    Zeni::Chronometer<Zeni::Time> bounce_timer;
    Zeni::Chronometer<Zeni::Time> lvl_buf;
    //keep track of gold count
    int gold_count;
    Zeni::Vector3f new_v;
  };

}

#endif
