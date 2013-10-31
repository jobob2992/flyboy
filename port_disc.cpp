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
		m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f);
		m_model = new Zeni::Model("models/ring.3ds");
		stopped = false;
		create_body();
	}
	void port_disc::update(std::pair <const Zeni::Point3f, const Zeni::Vector3f> in)
	{
		m_end_point_a = in.first;
		m_end_point_b = in.first + Zeni::Vector3f(0.0f, 0.0f, -2.0f);
		m_radius = 11.0f;
		m_position = in.first;
		m_velocity = in.second;
		stopped = false;
		if(stuck)
		{
			stuck->stop(false);
			stuck = NULL;
		}
		create_body();
	}
	void port_disc::step(const float &time_step)
	{
		if (!stopped)
			m_position += time_step * m_velocity;
		create_body();
	}
	void port_disc::accelerate(Zeni::Vector3f vec)
	{
		m_velocity += vec;
	}
	void port_disc::set_location(Zeni::Point3f loc)
	{
		m_position = loc;
		create_body();
	}
	void port_disc::enemy_stuck(enemy* it)
	{
		stuck = it;
		stop();
	}
	bool port_disc::is_stuck()
	{
		if (stuck)
			return true;
		else
			return false;
	}
	void port_disc::render()
	{
		const std::pair<Zeni::Vector3f, float> rotation = m_rotation.get_rotation();
		m_model->set_translate(m_position);
		m_model->set_scale(Zeni::Vector3f(9.0f, 9.0f, 2.0f));
		m_model->set_rotate(rotation.second, rotation.first);
		m_model->render();
	}
	void port_disc::create_body()
	{
		m_body = Zeni::Collision::Infinite_Cylinder(m_position, m_position + Zeni::Vector3f(0.0f, 0.0f, -2.0f), m_radius);
		//m_source->set_position(m_position + Zeni::Vector3f(0.0f, 0.0f, -1.0f));
	}
	Zeni::Point3f port_disc::location()
	{
		return m_position;
	}
	void port_disc::stop()
	{
		stopped = true;
	}
};