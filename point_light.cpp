#include "point_light.h"

PointLight::PointLight()
{
}

PointLight::PointLight(Vertex pos, Colour col)
{
    position = pos;
    intensity = col;
}

void PointLight::get_position(Vertex &pos)
{
    pos = position;
}

void PointLight::emit_photons(Photon *photons, int max_photons)
{
    std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1, 1);

    for (int i = 0; i < max_photons; i++)
    {
        float x = 1,y = 1,z = 1;
        while ( x*x + y*y + z*z > 1.0 )
        {
            // generate three random numbers between -1 and 1
            x = (dist(mt));
            y = (dist(mt));
            z = (dist(mt));
        }

        photons[i].path.direction = Vector(x,y,z);
        photons[i].path.direction.normalise();
        photons[i].path.position = position;
        photons[i].intensity = intensity;
    }
}



