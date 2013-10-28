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

	private:
		void create_body();
		Zeni::Vector3f m_end_point_a;
		Zeni::Vector3f m_end_point_b;
		float m_radius;
		Zeni::Collision::Infinite_Cylinder m_body;
		Zeni::Point3f m_position;
		Zeni::Vector3f m_velocity;
	};
}
#endif