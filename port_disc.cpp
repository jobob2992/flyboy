#include <zenilib.h>

#include "port_disc.h"

namespace port_disc{
	port_disc::port_disc(std::pair <const Zeni::Point3f, const Zeni::Vector3f> in)
	{
		m_end_point_a = in.first;
		m_end_point_b = in.first + Zeni::Vector3f(0.0f, 0.0f, -2.0f);
		m_radius = 11.0f;
		m_position = in.first;
		m_velocity = in.second;
		create_body();
	}
	void port_disc::update(std::pair <const Zeni::Point3f, const Zeni::Vector3f> in)
	{
		m_end_point_a = in.first;
		m_end_point_b = in.first + Zeni::Vector3f(0.0f, 0.0f, -2.0f);
		m_radius = 11.0f;
		m_position = in.first;
		m_velocity = in.second;
		create_body();
	}
	void port_disc::create_body()
	{
		m_body = Zeni::Collision::Infinite_Cylinder(m_end_point_a, m_end_point_b, m_radius);
		//m_source->set_position(m_position + Zeni::Vector3f(0.0f, 0.0f, -1.0f));
	}
	Zeni::Point3f port_disc::location(){
		return m_position;
	}
};