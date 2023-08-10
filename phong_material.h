/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Phong is a child class of Material and implement the simple Phong
// surface illumination model

#pragma once

#include "material.h"
#include "environment.h"
#include <random>

class Phong : public Material {
public:
//BEGIN_STAGE_ONE
	Colour ambient;
	Colour diffuse;
	Colour specular;
	float  power;
	Environment *env;

	Phong(Environment *env, Colour p_ambient, Colour p_diffuse, Colour p_specular, float p_power);
//END_STAGE_ONE
	Phong();

	void compute_indirect_photon(Photon &photon, Hit &best_hit, int recurse);
    Colour compute_once(Ray &viewer, Hit &hit, int recurse);
	Colour compute_per_light(Vector& viewer, Hit& hit, Vector& ldir);
	bool isGlobal();
	Colour radiance(std::vector<int> indexes, bool caustics, float radius, Hit& hit);
	Colour scale(Colour c);
};
