/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

#include "global_material.h"
#include <random>
#include <math.h>

GlobalMaterial::GlobalMaterial(Environment* p_env, Colour p_reflect_weight, Colour p_refract_weight, float p_ior)
{
	environment = p_env;
	reflect_weight = p_reflect_weight;
	refract_weight = p_refract_weight;
	ior = p_ior;
}

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;
	float depth;

	if (recurse > 0)
	{
		//Calculate the reflection ray
		Ray reflect_ray;
		hit.normal.reflection(viewer.direction, reflect_ray.direction);
		reflect_ray.direction.normalise();
		reflect_ray.position = hit.position + 0.001f * reflect_ray.direction;
		
		//Calculate the refract ray
		Ray refract_ray;
		if (!hit.entering)
		{
			hit.normal.negate();
		}
		bool calculate = viewer.direction.refraction(hit.normal, refract_ray.direction, ior);
		refract_ray.direction.normalise();
		refract_ray.position = hit.position + 0.001f * refract_ray.direction;

		//calculate fresnel equations
		float reflectance;
		fresnel(viewer.direction, hit.normal, 1.0f, ior, reflectance);

		Colour reflect_colour;
		environment->raytrace(reflect_ray, recurse - 1, reflect_colour, depth);
		reflect_colour *= reflectance;
		result += reflect_colour;

		Colour refract_colour;
		if (calculate)
		{
			environment->raytrace(refract_ray, recurse - 1, refract_colour, depth);
			refract_colour *= (1-reflectance);
			result += refract_colour;
		}
	}
	
	return result;
}

Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result;

	result.r=0.0f;
	result.g=0.0f;
	result.b=0.0f;

	return result;
}

void GlobalMaterial::compute_indirect_photon(Photon &photon, Hit &best_hit, int recurse)
{
	if (recurse > 0)
    {
		float p_s, p_d;
		fresnel(photon.path.direction, best_hit.normal, 1.0f, ior, p_s);


		p_d = 1 - p_s;

		std::mt19937 mt(std::random_device{}());
		std::uniform_real_distribution<float> dist(0, 1);

		float r = dist(mt);

		if (r < p_s)
		{
			//reflection
			photon.path.direction.reflection(best_hit.normal, photon.path.direction);
			photon.path.direction.normalise();
			photon.path.position = best_hit.position + 0.001f * photon.path.direction;
			//photon.intensity = reflect_weight;
		}
		else
		{
			// refraction
			if (!best_hit.entering)
			{
				best_hit.normal.negate();
			}
			bool calculate = photon.path.direction.refraction(best_hit.normal, photon.path.direction, ior);
			photon.path.direction.normalise();
			photon.path.position = best_hit.position + 0.001f * photon.path.direction;
		}

		recurse--;
		environment->caustictrace(photon, recurse);
	}
}

bool GlobalMaterial::isGlobal()
{
	return true;
}

void GlobalMaterial::fresnel(Vector& view, Vector& normal, float etai, float etat, float& kr)
{
	float cos_theta = normal.dot(view);
	float r_parallel = (etat * cos_theta - cos_theta) / (etat * cos_theta + cos_theta);
	float r_perpendicular = (cos_theta - etat * cos_theta) / (cos_theta + etat * cos_theta);
	kr = (pow(r_parallel, 2) + pow(r_perpendicular, 2)) / 2;
}
