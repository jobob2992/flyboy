#include "port_disc.h"

namespace port_disc {
	class port_disc{
		void port_disc(const Zeni::Point3f &position,
		  const Zeni::Vector3f &velocity):
			  m_end_point_a(position),
			  m_end_point_b(position + Vector3f(0.0f, 0.0f, -2.0f)),
			  m_radius(11.0f),
			  m_position(position),
			  m_velocity(velocity)
	{

	}
	private:
		void port_disc::create_body(){

		}
	};
}