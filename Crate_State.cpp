#include <zenilib.h>
#include <sstream>
#include "Crate_State.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace Crate {

  Crate_State::Crate_State()
    : m_crate("collide", "models/island.3ds",Point3f(70, -10.0f, 100.0f),
              Vector3f(40.0f, 40.0f, 40.0f)),
		crate2("collide", "models/island.3ds",Point3f(50.0f, -80.0f, 100.0f),
              Vector3f(40.0f, 40.0f, 40.0f)),
	  crate3("collide", "models/island.3ds",Point3f(250.0f, -150.0f, 100.0f),
              Vector3f(40.0f, 40.0f, 40.0f)),
	  crate4("collide", "models/island.3ds",Point3f(200.0f, 100.0f, 100.0f),
              Vector3f(40.0f, 40.0f, 40.0f)),
	  crate5("collide", "models/island.3ds",Point3f(20.0f,100.0f, 50.0f),
              Vector3f(40.0f, 40.0f, 40.0f)),
	  crate6("collide", "models/island.3ds",Point3f(70.0f,150.0f, 300.0f),
              Vector3f(140.0f, 140.0f, 140.0f)),
    m_player(Camera(Point3f(0.0f, 0.0f, 80.0f),
             Quaternion(),
             1.0f, 10000.0f),
             Vector3f(0.0f, 0.0f, -39.0f),
             11.0f),
	m_shot(false),
	port(false),
	disc(false),
    //initialize gold pieces
    gold("models/gold.3ds", Point3f(300.0f,300.0f,60.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    gold2("models/gold.3ds", Point3f(225.0f,-25.0f,60.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    gold3("models/gold.3ds", Point3f(150.0f,-115.0f,60.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    gold4("models/gold.3ds", Point3f(60.0f,-40.0f,60.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    gold5("models/gold.3ds", Point3f(70.0f,150.0f,50.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    //initialize enemies
    e1(50.0f,"models/creature.3ds", Point3f(100.0f,100.0f,50.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    e2(40.0f,"models/creature.3ds", Point3f(-100.0f,-100.0f,50.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
    e3(30.0f,"models/creature.3ds", Point3f(100.0f,-100.0f,50.0f),
             Vector3f(2.0f, 2.0f, 2.0f)),
             lvl(1)
  {
      gold_set.insert(&gold);
      gold_set.insert(&gold2);
      gold_set.insert(&gold3);
      gold_set.insert(&gold4);
      gold_set.insert(&gold5);
      crate_set.insert(&m_crate);
      crate_set.insert(&crate2);
      crate_set.insert(&crate3);
      crate_set.insert(&crate4);
      crate_set.insert(&crate5);
      crate_set.insert(&crate6);
      enemy_set.insert(&e1);
      enemy_set.insert(&e2);
      enemy_set.insert(&e3);
	  enemy_set_size = enemy_set.size();
    set_pausable(true);
    gold_count = 0;
    bounced = false;
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
    Vector3f x_vel;
    Vector3f y_vel;
    Vector3f z_vel;
    if(!bounced)
    {
        x_vel = velocity.get_i();
        y_vel = velocity.get_j();
        z_vel = m_player.get_velocity().get_k();
    }
    else
    {
        x_vel = new_v.get_i();
        y_vel = new_v.get_j();
        z_vel = new_v.get_j();
    }
   // Vector3f z_vel = m_player.get_velocity().get_k();


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
			m_disc = new port_disc::port_disc(m_player.shoot());
			disc = true;
		}
	}

	//Teleporting
	if (port){
		port = false;
		if (disc){
			if (m_disc->is_stuck())
			{
				enemy_set.erase(m_disc->stuck_to());
			}
			m_player.port(m_disc->location());
			delete m_disc;
			m_disc = NULL;
			disc = false;
			for (auto iter: crate_set)
			{
				if(iter->get_body().intersects(m_player.get_body())) 
				{
					if(m_player.get_camera().position.z < 50.0f + (iter->get_body().get_end_point_a().z))
					{
						if(m_player.get_camera().position.z > (iter->get_body().get_end_point_b().z))
						{
							m_player.set_position(Zeni::Point3f(m_player.get_camera().position.x,
								m_player.get_camera().position.y,
								50.0f + (iter->get_body().get_end_point_a().z)));
						}
					}
				}
			}
			for (auto iter: enemy_set)
			{
				if(iter->get_body().intersects(m_player.get_body()))
				{
					enemy_set.erase(iter);
				}
			}
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
	  if (!m_player.is_on_ground() || z_vel.z > 0.0f)
		z_vel -= Vector3f(0.0f, 0.0f, 210.0f * time_step);
	  if (disc)
		m_disc->accelerate(Zeni::Vector3f(0.0f, 0.0f, -210.0f) * time_step);

      /** Try to move on each axis **/
	  m_player.set_on_ground(false);
      partial_step(time_step, x_vel);
      partial_step(time_step, y_vel);
      partial_step(time_step, z_vel);
	  if (disc)
		m_disc->step(time_step);

      
      //enemy chasing player
      if(lvl == 5)
      {
          for(std::set<enemy*>::iterator it=enemy_set.begin();it!=enemy_set.end();++it)
          {
              (*it)->calc_dir();
              (*it)->cont_chase();//this checks if the enemy needs to rest
              (*it)->chase(m_player, time_step);
          }
      }
      /** Keep player above ground; Bookkeeping for jumping controls **/
      const Point3f &position = m_player.get_camera().position;
	  
      if(position.z < 50.0f) {
        m_player.set_position(Point3f(position.x, position.y, 50.0f));
		m_player.set_velocity(Zeni::Vector3f(m_player.get_velocity().x, m_player.get_velocity().y, 0.0f));
        m_player.set_on_ground(true);
      }
	  if (disc)
	  {
		  if (m_disc->location().z < 2.0f)
		  {
			  m_disc->stop();
		  }
	  }
	  /*else for(std::set<Crate*>::iterator it = crate_set.begin(); it!= crate_set.end(); ++it)
	  {
		  if (m_player.get_body().intersects((*it)->get_body()))
		  {
			  if (m_player.get_body().get_end_point_b().z > (*it)->get_body().get_end_point_a().z)
			  {
				  m_player.set_position(Point3f(position.x, position.y, 50.0f + (*it)->get_body().get_end_point_a().z));
			  }
		  }
	  }*/
      /*else
      {
        m_player.set_on_ground(false);
      }*/
    }
	
	//fly if holding space
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
    
    //bounce the player

    //stop the bouncing after 1 second
    if(bounce_timer.seconds() > 0.3f)
    {
        bounced = false;
    }

///////////////level difference starts from here//////////////////////////////////
    if(lvl < 2) //jetpack tutorial
    {
        crate2.disappear();
        crate3.disappear();
        crate4.disappear();
        crate5.disappear();
        crate6.disappear();
        for(std::set<game_object*>::iterator it= gold_set.begin(); it!=gold_set.end(); ++it)
        {
            if((*it) != &gold) (*it)->disappear();
        }
        for(std::set<enemy*>::iterator it= enemy_set.begin(); it!=enemy_set.end(); ++it)
        {
            (*it)->disappear();
        }
        if(gold.m_corner.z < -100.0f)
        {
            lvl++;
        }
    }
    if(lvl == 2) //use teleport gun
    {
        //e1.appear();
        crate6.appear();
        gold5.appear();
        if(gold5.m_corner.z < -100.0f)
        {
            lvl++;
        }
    }
    if (lvl == 3)
    {
        e1.appear();
        if(enemy_set.size() < enemy_set_size) //just make it to next level for now
        {
            lvl++;
            lvl_buf.set(0);
            lvl_buf.start();
        }
    }
    if (lvl == 4)
    {

        for(std::set<Crate*>::iterator it= crate_set.begin(); it!=crate_set.end(); ++it)
        {
            (*it)->appear();
        }
        for(std::set<enemy*>::iterator it= enemy_set.begin(); it!=enemy_set.end(); ++it)
        {
            (*it)->appear();
        }
        if(lvl_buf.seconds() > 5)
        {
            lvl++;
        }
    }

}

  //render the enemy set
  void Crate_State::render_set(std::set<enemy*> &input)  
    {
        std::set<enemy*>::iterator it;
		for(it = input.begin();it!= input.end();++it)
		{
			(*it)->render();
		}
    }

  void Crate_State::render_set(std::set<game_object*> &input)  
    {
        std::set<game_object*>::iterator it;
        for(it = input.begin();it!= input.end();++it)
        {
            (*it)->render();
        }
    }

  //render the Crate set
    void Crate_State::render_crate_set(std::set<Crate*> &input)  
    {
        std::set<Crate*>::iterator it;
        for(it = input.begin();it!= input.end();++it)
        {
            (*it)->render();
        }
    }

  void Crate_State::render() {
    Video &vr = get_Video();

    vr.set_3d(m_player.get_camera());
	//render 3 crates
    render_crate_set(crate_set);

	if (disc){
		m_disc->render();
	}
    //render the st of gold
    render_set(gold_set);
    render_set(enemy_set);
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
    get_Video().set_2d(std::make_pair(Point2f(0.0f, 0.0f), Point2f(1024.0f, 768.0f)), true);
	Zeni::Font &ft = get_Fonts()["title"];
	const Point3f &velocity = m_player.get_velocity();
	float xwid = ft.get_text_width(itoa((m_player.get_camera().position.z - 50.0f)/25.0f));
	//float ywid = ft.get_text_width(itoa(m_player.get_camera().position.z));
	//float zwid = ft.get_text_width(itoa(m_player.get_velocity().z));
	float spacewid = ft.get_text_width(",");
	ft.render_text(itoa((m_player.get_camera().position.z - 50.0f)/25.0f), Point2f(0.0f, 0.0f), get_Colors()["yellow"], ZENI_LEFT);
    //render lvl instructions//////////
    if(lvl = 1)ft.render_text("hold SPACE to use your jetpack\nand pick up the gold on the rock\nin front of you", Point2f(0.0f, 300.0f), get_Colors()["red"], ZENI_LEFT);
    //render the fuel left
     render_image("fuel_bar",Point2f(25.0f,70.0f),
         Point2f(25+m_player.get_time(),92),false);
    //render the health
     render_image("health_bar",Point2f(25.0f,102.0f),
         Point2f(25+m_player.get_health(),124),false);

     render_image("health_bar",Point2f(25.0f,102.0f),
         Point2f(25+m_player.get_health(),124),false);
  }

  void Crate_State::partial_step(const float &time_step, const Vector3f &velocity) {

    m_player.set_velocity(velocity);
    const Point3f backup_position = m_player.get_camera().position;

    m_player.step(time_step);

    /** If collision with the crate has occurred, roll things back **/
    for(std::set<Crate*>::iterator it = crate_set.begin(); it!=crate_set.end();++it)
    {
          if((*it)->get_body().intersects(m_player.get_body())) 
          {
			  if(m_player.get_camera().position.z < 50.0f + ((*it)->get_body().get_end_point_a().z))
			  {
				  if(m_player.get_camera().position.z > ((*it)->get_body().get_end_point_b().z))
				  {
					if(m_moved)
					{
					    /** Play a sound if possible **/
					    (*it)->collide();
					    m_moved = false;
					    //m_crate.~Crate();
					    //m_crate.disappear();
					  }
					  /*if (m_player.get_body().get_end_point_b().z - m_player.get_body().get_radius() > (*it)->get_body().get_end_point_b().z)
					  {
						  //m_player.set_position(Point3f(m_player.get_camera().position.x, m_player.get_camera().position.y, 50.0f + m_player.get_body().get_radius() + (*it)->get_body().get_end_point_a().z));
						  m_player.set_on_ground(true);
						  m_player.set_velocity(Zeni::Vector3f(m_player.get_velocity().x, m_player.get_velocity().y, 0.0f));
					  }
					  else
					  {*/
						  m_player.set_position(backup_position);
					  //}
					  /** Bookkeeping for jumping controls **/
					  if(velocity.k < 0.0f)
					  m_player.set_on_ground(true);
				  }
			  }
          }
		  if (disc)
		  {
				if(m_disc->get_body().intersects((*it)->get_body()))
				{
					if(m_disc->location().z < ((*it)->get_body().get_end_point_a().z))
					{
						if(m_disc->location().z > ((*it)->get_body().get_end_point_b().z))
						{
							if(m_disc->location().z > ((*it)->get_body().get_end_point_a().z) - 8.0f)
								m_disc->set_location(Zeni::Point3f(m_disc->location().x, m_disc->location().y, (*it)->get_body().get_end_point_a().z));
							m_disc->stop();
						}
					}
				}
		  }
    }

    for(std::set<game_object*>::iterator it = gold_set.begin(); it!=gold_set.end();++it)
    {
      if((*it)->get_body().intersects(m_player.get_body())) 
      {
          if(m_moved)
          {
            /** Play a sound if possible **/
                play_sound("coin");
                m_moved = false;
                (*it)->disappear();
                gold_count++;
          }
       }
    }
    for(std::set<enemy*>::iterator it = enemy_set.begin(); it!=enemy_set.end();++it)
    {
      if((*it)->get_body().intersects(m_player.get_body()))
      {
          if(m_moved)
          {
            /** Play a sound if possible **/
                play_sound("bounce");
                m_moved = false;
                (*it)->rest();
                //get the direction that player should be bounced
                new_v= Vector3f(6*(m_player.get_camera().position.x - (*it)->m_corner.x), 6*(m_player.get_camera().position.y - (*it)->m_corner.y),10.0f);
                bounced =true;
                bounce_timer.set(0);
                bounce_timer.start();
                m_player.attacked();
          }
       }
	  if(disc)
	  {
		  if(m_disc->get_body().intersects((*it)->get_body()))
		  {
			  if(m_disc->location().z < ((*it)->get_body().get_center().z + 12.0f))
			  {
					if(m_disc->location().z > ((*it)->get_body().get_center().z - 1.5f))
					{
						if(!m_disc->is_stuck())
						{
							m_disc->enemy_stuck(*it);
							(*it)->stop(true);
						}
					}
			  }
		  }
	  }
    }
  }

}
