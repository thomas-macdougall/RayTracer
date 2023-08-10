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

 // A simple Phong based lighting model.

#include "phong_material.h"
#include "p_scene.h"
#include <math.h>

Phong::Phong(Environment *env, Colour p_ambient, Colour p_diffuse, Colour p_specular, float p_power)
{
//BEGIN_STAGE_ONE
	ambient = p_ambient;
	diffuse = p_diffuse;
	specular = p_specular;
	power = p_power;
	this->env = env;
//END_STAGE_ONE
}

// The compute_once() method supplies the ambient term.
Colour Phong::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;

    float radius1 = 0.01f, radius2 = 0.005f;

    Photon photon = Photon(viewer, Colour(1.0f, 1.0f, 1.0f), hit.position, Photon::PHOTON_INDIRECT);
    // check if the photon map contains any photons within 1 unit of the hit position
    std::vector<int> nearest_photons = env->photon_map->radiusSearch(photon, radius1);
    std::vector<int> nearest_caustic_photons = env->caustics_map->radiusSearch(photon, radius2);
    
    // calculate the radiance
    result = result + radiance(nearest_photons, false, radius1, hit);
    result = result + radiance(nearest_caustic_photons, true, radius2, hit);
    
	return result;
}

Colour Phong::radiance(std::vector<int> indexes, bool caustics, float radius, Hit& hit)
{
    Colour result;
    Photon p;

    if (indexes.size() > 0)
    {
        // calculate the area of the circle
        float area = 3.14159f * pow(radius, 2.0f);

        // calculate radiance 
        for (int i = 0; i < indexes.size(); i++)
        {

            if (caustics)
            {
                p = env->caustics_vec[indexes[i]];
                p.intensity = Colour(1.0f, 1.0f, 1.0f);
            }
            else
            {
                p = env->photons_vec[indexes[i]];
            }

            // negative dot product means the photon is behind the surface
            float dot = -(p.path.direction.dot(hit.normal));
            if (dot > 0)
            {
                p.intensity = caustics ? p.intensity * dot : p.intensity * dot * diffuse;

                // cone filtering
                float dist = (p.hit_position - hit.position).length();
                float weight = 1.0f - (dist / radius);

                result = result + (p.intensity * weight);
            }
        }

        result = result * (1 / area);
    }

    return result;
}

// The compute_per_light() method supplies the diffuse and specular terms.
Colour Phong::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result;
//BEGIN_STAGE_ONE
	float diff;

	Vector tolight;
	Vector toviewer;

	result.r=0.0f;
	result.g=0.0f;
	result.b=0.0f;

	tolight = ldir;
	tolight.negate();

	toviewer = viewer;
	toviewer.negate();

	diff = hit.normal.dot(tolight);
	
	// Scene.raytrace() does this test, but let's keep it here in case that changes or we get called from elsewhere. 
	if (diff < 0.0f) // light is behind surface
	{
		return result;
	}

	// diffuse

	result += diffuse * diff;

	// the specular component

	Vector r;
	
	hit.normal.reflection(tolight, r);
	r.normalise();

	float h  = r.dot(toviewer);

	if (h > 0.0f)
	{
		float p = (float)pow(h, power);

		result += specular * p;
	}
//END_STAGE_ONE
	return result;
}

//reflection
void Phong::compute_indirect_photon(Photon &photon, Hit &best_hit, int recurse)
{
    if (recurse > 0)
    {
        std::mt19937 mt(std::random_device{}());
        std::uniform_real_distribution<float> dist(0, 1);
        std::uniform_real_distribution<float> dist2(-1, 1);


        float r = dist(mt);

        // reflection, absorbtion and transmission probabilities
        float p_s = max(specular.r, max(specular.g, specular.b));
        float p_d = max(diffuse.r, max(diffuse.g, diffuse.b));
        float p_a = 1 - p_s - p_d;

		if (r < p_s)
		{
			// compute the reflection ray
            Ray reflect_ray;
            best_hit.normal.reflection(photon.path.direction, reflect_ray.direction);
            reflect_ray.direction.normalise();
            reflect_ray.position = best_hit.position + 0.001f * reflect_ray.direction;

            // create new photon
            Photon new_photon;
            new_photon.path = reflect_ray;
            new_photon.intensity = scale(specular);
            new_photon.mode = Photon::PHOTON_INDIRECT;

			// trace photon
            recurse--;

			env->photontrace(new_photon, recurse);

		}
        else if (r <= p_s + p_d)
        {

            env->photons_vec.push_back(photon);
            env->num_photons++;

            // create new photon
            Photon new_photon;
            new_photon.intensity = scale(diffuse);
            new_photon.mode = Photon::PHOTON_INDIRECT;
            float x = 1, y = 1, z = 1;

            while ( x*x + y*y + z*z > 1.0 )
            {
                // generate three random numbers between -1 and 1
                x = (dist2(mt));
                y = (dist2(mt));
                z = (dist2(mt));
            }

            new_photon.path.direction = Vector(x,y,z);
            new_photon.path.direction.normalise();

            if (new_photon.path.direction.dot(best_hit.normal) < 0)
            {
                new_photon.path.direction = -photon.path.direction;
            }

            new_photon.path.position = best_hit.position + 0.001f * new_photon.path.direction;

            recurse--;
			env->photontrace(new_photon, recurse);
        }
        else
        {

            env->photons_vec.push_back(photon);
            env->num_photons++;

            // photon is absorbed
        }
    }
}

bool Phong::isGlobal()
{
    return false;
}

// scale the photon intensity for each r,g and b channels
Colour Phong::scale(Colour c)
{
    float maximum = max(c.r, max(c.g, c.b));
    float scale = 1.0f / maximum;

    c.r *= scale;
    c.g *= scale;
    c.b *= scale;
    return c;
}

