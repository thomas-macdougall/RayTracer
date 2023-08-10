#include "photon_material.h"
#include "p_scene.h"
#include <math.h>
#include <random>

PhotonMaterial::PhotonMaterial(Environment *env, Colour spec, Colour dif)
{
    this->env = env;
    this->specular = spec;
    this->diffuse = dif;
}

Colour PhotonMaterial::compute_once(Ray &viewer, Hit &hit, int recurse)
{

    Colour colour;
    float radius = 0.1f;

    Photon photon = Photon(viewer, Colour(1.0f, 1.0f, 1.0f), hit.position, Photon::PHOTON_INDIRECT);
    // check if the photon map contains any photons within 1 unit of the hit position
    std::vector<int> indexes = env->photon_map->radiusSearch(photon, radius);
    
    if (indexes.size() > 0)
    {
        // calculate the area of the circle
        float area = 3.14159f * pow(radius, 2.0f);
        // calculate radiance 
        for (int i = 0; i < indexes.size(); i++)
        {
            Photon p = env->photons_vec[indexes[i]];

            // if (p.mode == Photon::PHOTON_INDIRECT)
            //     colour = colour + (p.intensity * (1/area));

            colour = colour + (p.intensity * (1/area));
        }

        //colour = colour; * specular;
    }

    return colour;
}

//reflection
void PhotonMaterial::compute_indirect_photon(Photon &photon, Hit& hit, int recurse)
{

    if (recurse > 0)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(0, 1);
        std::uniform_real_distribution<float> dist2(0, 2);


        float r = dist(mt);

        // reflection, absorbtion and transmission probabilities
        float p_r = max(specular.r, max(specular.g, specular.b));
        float p_t = max(diffuse.r, max(diffuse.g, diffuse.b));
        float p_a = 1 - p_r - p_t;

		if (r <= p_r)
		{
			// compute the reflection ray
            Ray reflect_ray;
            hit.normal.reflection(photon.path.direction, reflect_ray.direction);
            reflect_ray.direction.normalise();
            reflect_ray.position = hit.position + 0.001f * reflect_ray.direction;

            // create new photon
            Photon new_photon;
            new_photon.path = reflect_ray;
            new_photon.intensity = photon.intensity * specular;
            new_photon.mode = Photon::PHOTON_INDIRECT;

			// trace photon
            recurse--;
            PhotonScene *scene = dynamic_cast<PhotonScene*>(env);
			scene->photontrace(new_photon, recurse);
		}
        else if (r > p_r && r <= p_r + p_t)
        {
            // if (photon.mode == Photon::PHOTON_INDIRECT)
            //     cout << "indirect photon position: " << photon.path.direction.x << ", " << photon.path.direction.y << ", " << photon.path.direction.z << endl;
            
            env->photons_vec.push_back(photon);
            env->num_photons++;

            // create new photon
            Photon new_photon;
            new_photon.intensity = photon.intensity * diffuse;
            new_photon.mode = Photon::PHOTON_INDIRECT;
            float x, y, z;

            while ( x*x + y*y + z*z > 1.0 )
            {
                // generate three random numbers between -1 and 1
                x = (dist2(mt) - 1.0);
                y = (dist2(mt) - 1.0);
                z = (dist2(mt) - 1.0);
            }

            new_photon.path.direction = Vector(x,y,z);
            new_photon.path.direction.normalise();

            if (new_photon.path.direction.dot(hit.normal) < 0)
            {
                new_photon.path.direction = -photon.path.direction;
            }

            new_photon.path.position = hit.position + 0.01f * new_photon.path.direction;
            // trace photon
            if (recurse < 5)
                cout << "Recurse: " << recurse << endl;
            recurse--;

            PhotonScene *scene = dynamic_cast<PhotonScene*>(env);
			scene->photontrace(new_photon, recurse);
        }
        else
        {
            env->photons_vec.push_back(photon);
            env->num_photons++;
            // photon is absorbed
        }
    
    }
}