#include "light.h"
#include <chrono>
#include <random>

#pragma once

class PointLight : public Light 
{
public:

    Vertex position;
	Colour intensity;

	PointLight();
	PointLight(Vertex pos, Colour col);

	void get_position(Vertex &position);
    void emit_photons(Photon *photons, int max_photons);

};
