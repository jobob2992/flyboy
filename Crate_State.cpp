#include <zenilib.h>
#include <sstream>
#include "Crate_State.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace Crate {

  Crate_State::Crate_State()
    : m_crate("collide", "models/island.3ds",Point3f(-300, 100.0f, 450.0f),
              Vector3f(30.0f, 30.0f, 30.0f)),
		crate2("collide", "models/ring.3ds",Point3f(50.0f, 100.0f, 400.0f),
              Vector3f(30.0f, 30.0f, 30.0f)),
	  crate3("collide", "models/gold.3ds",Point3f(100.0f, 100.0f, 250.0f),
              Vector3f(30.0f, 30.0f, 30.0f)),
	  crate4("collide", "models/gold.3ds",Point3f(150.0f, 100.0f, 150.0f),
              Vector3f(40.0f, 40.0f, 40.0f)),
	  crate5("coin", "models/gold.3ds",Point3f(200.0f,100.0f, 50.0f),
              Vector3f(15.0f, 15.0f, 15.0f)),
    m_player(Camera(Point3f(0.0f, 0.0f, 80.0f),
             Quaternion(),
             1.0f, 10000.0f),
             Vector3f(0.0f, 0.0f, -39.0f),
             11.0f),
	m_shot(false),
	port(false),
	disc(false)
//    gold("collide", "models/gold.3ds", Point3f(200.0f,200.0f,300.0f),
//              Vector3f(30.0f, 30.0f, 30.0f))
  {
    set_pausable(true);
  }

  void Crate_State::on_push() {
    get_Window().set_mouse_state(Window::MOUSE_RELATIVE);
  }

  void Crate_State::on_key(const SDL_KeyboardEvent &event) {
    switch(event.keysym.sym) {
      case SDLK_w:
        m_controls.forward = event.type == SDL_KEYDOWN;
        break;

      case SDLK_a:
        m_controls.left = event.type == SDL_KEYDOWN;
        break;

      case SDLK_s:
        m_controls.back = event.type == SDL_KEYDOWN;
        break;

      case SDLK_d:
        m_controls.right = event.type == SDL_KEYDOWN;
        break;

		//NOW JETPACK REPLACE JUMP
	  case SDLK_SPACE:
        m_controls.flying = event.type == SDL_KEYDOWN;
        break;

      /*case SDLK_SPACE:
        if(event.type == SDL_KEYDOWN) {
          m_player.jump();
          m_moved = true;
        }*/
        break;

      default:
        Gamestate_Base::on_key(event);
        break;
    }
  }

  void Crate_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
    m_player.adjust_pitch(event.yrel / 500.0f);
    m_player.turn_left_xy(-event.xrel / 500.0f);
  }

  void Crate_State::on_mouse_button(const SDL_MouseButtonEvent &event) {
	  switch (event.button)
	  {
	  case 1:
		  if (event.type == SDL_MOUSEBUTTONDOWN)
			  m_shot = true;
		  else
			  m_shot = false;
		  break;
	  case 3:
		  if (event.type == SDL_MOUSEBUTTONDOWN)
			  port = true;
		  else
			  port = false;
		  break;
	  default:
		  break;
	  }
	  /*if(event.button == 1 && event.type == SDL_MOUSEBUTTONDOWN && !m_shot) { //left click
		  if (disc){
			  m_disc->update(m_player.shoot());
		  }
		  else{
			  m_disc = new port_disc::port_disc(m_player.shoot());
		  }
		  m_shot = true;
	  }
	  else if(event.button == 1 && event.state == SDL_MOUSEBUTTONUP && m_shot) { //left unclick
		  m_shot = false;
	  }
	  else if(event.button == 3 && event.state == SDL_MOUSEBUTTONDOWN && disc && !port) { //right click
		  m_player.port(m_disc->location());
		  delete m_disc;
		  m_disc = NULL;
		  port = true;
	  }
	  else if (event.button == 3 && event.state == SDL_MOUSEBUTTONUP && port){ //right unclick
		  port = false;
	  }*/
  }

  void Crate_State::perform_logic() {
    const Time_HQ current_time = get_Timer_HQ().get_time();
    float processing_time = float(current_time.get_seconds_since(time_passed));
    time_passed = current_time;

    /** Get forward and left vectors in the XY-plane **/
    const Vector3f forward = m_player.get_camera().get_forward().get_ij().normalized();
    const Vector3f left = m_player.get_camera().get_left().get_ij().normalized();

    /** Get velocity vector split into a number of axes **/
    const Vector3f velocity = (m_controls.forward - m_controls.back) * 80.0f * forward
                            + (m_controls.left - m_controls.right) * 80.0f * left;
    const Vector3f x_vel = velocity.get_i();
    const Vector3f y_vel = velocity.get_j();
    Vector3f z_vel = m_player.get_velocity().get_k();

    /** Bookkeeping for sound effects **/
    if(velocity.magnitude() != 0.0f)
      m_moved = true;

	//Shooting
	if (m_shot){
		m_shot = false;
		if (disc){
			m_disc->update(m_player.shoot());
		}
		else {
			play_sound("coin");
			m_disc = new port_disc::port_disc(m_player.shoot());
			disc = true;
		}
	}

	//Teleporting
	if (port){
		port = false;
		if (disc){
			m_player.port(m_disc->location());
			delete m_disc;
			m_disc = NULL;
			disc = false;
		}
	}

    /** Keep delays under control (if the program hangs for some time, we don't want to lose responsiveness) **/
    if(processing_time > 0.1f)
      processing_time = 0.1f;

    /** Physics processing loop**/
    for(float time_step = 0.05f;
        processing_time > 0.0f;
        processing_time -= time_step)
    {
      if(time_step > processing_time)
        time_step = processing_time;

      /** Gravity has its effect **/
      z_vel -= Vector3f(0.0f, 0.0f, 210.0f * time_step);

      /** Try to move on each axis **/
      partial_step(time_step, x_vel);
      partial_step(time_step, y_vel);
      partial_step(time_step, z_vel);

      /** Keep player above ground; Bookkeeping for jumping controls **/
      const Point3f &position = m_player.get_camera().position;
      if(position.z < 50.0f) {
        m_player.set_position(Point3f(position.x, position.y, 50.0f));
        m_player.set_on_ground(true);
      }
      else
      {
        m_player.set_on_ground(false);
      }
    }
	//fly if holding f
	if(m_controls.flying && m_player.can_fly())
	{
		m_player.fly();
	}
	else
	{
		m_player.fall();
	}
    
    //player is charging on ground or other places
    if(m_player.is_on_ground() || m_player.resting())
	{
		m_player.fuel_up();
	}
  }

  void Crate_State::render() {
    Video &vr = get_Video();

    vr.set_3d(m_player.get_camera());
	//render 3 crates
	m_crate.render();
	crate2.render();
	crate3.render();
	crate4.render();
	crate5.render();
//    gold.render();

	/*if (disc){
		m_disc.render();
	}*/

	//render the floor
	Material floor("rock");
	Vertex3f_Texture p0(Point3f(5000.0f, 5000.0f, -1.0f),Point2f(0.0f,0.0f));
	Vertex3f_Texture p1(Point3f(-5000.0f, 5000.0f, -1.0f),Point2f(0.0f,1.0f));
	Vertex3f_Texture p2(Point3f(-5000.0f, -5000.0f, -1.0f),Point2f(1.0f,1.0f));
	Vertex3f_Texture p3(Point3f(5000.0f, -5000.0f, -1.0f),Point2f(1.0f,0.0f));
	Quadrilateral<Vertex3f_Texture> quad(p0,p1,p2,p3);
	quad.fax_Material(&floor);
	vr.render(quad);

	vr.set_2d();
	Zeni::Font &ft = get_Fonts()["title"];
	const Point3f &position = m_player.get_camera().position;
	float xwid = ft.get_text_width(itoa(position.x));
	float ywid = ft.get_text_width(itoa(position.y));
	//float zwid = ft.get_text_width(itoa(position.z));
	float zwid =ft.get_text_width(itoa(m_player.get_time()));
	float spacewid = ft.get_text_width(",");
	ft.render_text(itoa(position.x), Point2f(0.0f, 0.0f), get_Colors()["yellow"], ZENI_LEFT);
	ft.render_text(",", Point2f(0.0f + xwid, 0.0f), get_Colors()["yellow"], ZENI_LEFT);
	ft.render_text(itoa(position.y), Point2f(0.0f + spacewid + xwid, 0.0f), get_Colors()["yellow"], ZENI_LEFT);
	ft.render_text(",", Point2f(0.0f + ywid + spacewid + xwid, 0.0f), get_Colors()["yellow"], ZENI_LEFT);
	ft.render_text(itoa(m_player.get_time()), Point2f(0.0f + spacewid + ywid + spacewid + xwid, 0.0f), get_Colors()["yellow"], ZENI_LEFT);
  }

  void Crate_State::partial_step(const float &time_step, const Vector3f &velocity) {
    m_player.set_velocity(velocity);
    const Point3f backup_position = m_player.get_camera().position;

    m_player.step(time_step);

    /** If collision with the crate has occurred, roll things back **/
    if(m_crate.get_body().intersects(m_player.get_body())) {
      if(m_moved)
      {
        /** Play a sound if possible **/
        m_crate.collide();
        m_moved = false;
        //m_crate.~Crate();
        //m_crate.disappear();
      }
      m_player.set_position(backup_position);

      /** Bookkeeping for jumping controls **/
      if(velocity.k < 0.0f)
        m_player.set_on_ground(true);
    }
	if(crate2.get_body().intersects(m_player.get_body())) {
      if(m_moved)
      {
        /** Play a sound if possible **/
        crate2.collide();
        m_moved = false;
      }
      m_player.set_position(backup_position);

      /** Bookkeeping for jumping controls **/
      if(velocity.k < 0.0f)
        m_player.set_on_ground(true);
    }
	if(crate3.get_body().intersects(m_player.get_body())) {
      if(m_moved)
      {
        /** Play a sound if possible **/
        crate3.collide();
        m_moved = false;
      }
      m_player.set_position(backup_position);

      /** Bookkeeping for jumping controls **/
      if(velocity.k < 0.0f)
        m_player.set_on_ground(true);
    }
	if(crate4.get_body().intersects(m_player.get_body())) {
      if(m_moved)
      {
        /** Play a sound if possible **/
        crate4.collide();
        m_moved = false;
      }
      m_player.set_position(backup_position);

      /** Bookkeeping for jumping controls **/
      if(velocity.k < 0.0f)
        m_player.set_on_ground(true);
    }
	if(crate5.get_body().intersects(m_player.get_body())) {
      if(m_moved)
      {
        /** Play a sound if possible **/
        crate5.collide();
        m_moved = false;
      }
      m_player.set_position(backup_position);

      /** Bookkeeping for jumping controls **/
      if(velocity.k < 0.0f)
        m_player.set_on_ground(true);
    }
  }

}
