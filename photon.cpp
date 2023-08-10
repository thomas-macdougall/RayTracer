#include "photon.h"

Photon::Photon()
{
    this->path = Ray();
    this->intensity = Colour();
    this->hit_position = Vertex();
    this->mode = Photon::PHOTON_DIRECT;
}

Photon::Photon(Ray path, Colour intensity, Vertex hit_position, Mode mode)
{
    this->path = path;
    this->intensity = intensity;
    this->hit_position = hit_position;
    this->mode = mode;
}

float Photon::operator[](int axis) const
{
    switch (axis)
    {
    case 0:
        return hit_position.x;
        break;
    case 1:
        return hit_position.y;
        break;
    case 2:
        return hit_position.z;
        break;
    default:
        return 0.0f;
    }
}
