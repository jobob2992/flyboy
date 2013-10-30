#ifndef PORT_DISC_H
#define PORT_DISC_H

#include <Zeni/Collision.h>
#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Sound.h>
#include <Zeni/Vector3f.h>
#include "game_object.h"

namespace port_disc{

	class port_disc {
	public:
		port_disc(std::pair <const Zeni::Point3f, const Zeni::Vector3f> in);
		void update(std::pair <const Zeni::Point3f, const Zeni::Vector3f> in);
		Zeni::Point3f location();
		void step(const float &time_step);
		const Zeni::Collision::Infinite_Cylinder & get_body() const {return m_body;}
		void accelerate(Zeni::Vector3f vec);
		void render();
		void stop();
		bool is_stopped(){return stopped;};

	private:
		void create_body();
		Zeni::Model *m_model;
		Zeni::Point3f m_end_point_a;
		Zeni::Point3f m_end_point_b;
		float m_radius;
		Zeni::Collision::Infinite_Cylinder m_body;
		Zeni::Point3f m_position;
		Zeni::Vector3f m_velocity;
		Zeni::Quaternion m_rotation;
		bool stopped;
	};
}
#endif