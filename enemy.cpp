#include <zenilib.h>
#include "enemy.h"

using namespace Zeni;
using namespace Zeni::Collision;

void enemy::chase(Crate::Player &player, float & step)
{
    if(!aim.is_running())aim.start();
    user_pos = player.get_camera().position;
    /*float x = user_pos.x-m_corner.x;
    float y = user_pos.y-m_corner.y;
    float z = user_pos.z-m_corner.z;
    if(user_pos.x>m_corner.x)x=speed*step;
    else x= -speed*step;
    if(user_pos.y>m_corner.y)y=speed*step;
    else y= -speed*step;
    if(user_pos.z>m_corner.z)z=speed*step;
    else z= -speed*step;*/
    //only go chase if the enemy is not resting
    if(go_chase)
    {
        m_corner += speed*step*direction/100.0f;
        create_body();
    }
}

void enemy::rest()
{
    chill.set(0);
    chill.start();
    go_chase = false;
}

void enemy::cont_chase()
{
    if(!chill.is_running() || !chill.seconds()<2.0f)
    {
        go_chase = true;
        chill.stop();
    }
}

void enemy::calc_dir()
{
    if(aim.seconds()>1.0f)
    {
        direction = user_pos-m_corner;
        aim.set(0);
        aim.start();
    }
}