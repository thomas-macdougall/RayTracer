#include "p_scene.h"
#include "photon_material.h"

PhotonScene::PhotonScene()
{
}

void PhotonScene::raytrace(Ray ray, int recurse, Colour &colour, float &depth)
{
    // first step, find the closest primitive
    Hit *best_hit = this->trace(ray);
	Light *lights = light_list;

    //if we found a primitive then compute the colour we should see
    if (best_hit != 0)
    {
        depth = best_hit->t;
		//Colour best = best_hit->what->material->compute_once(ray, *best_hit, recurse); 
		// print the colour

		// if (best.r != 0.0f || best.g != 0.0f || best.b != 0.0f)
		// {
		// 	cout << "Colour: " << best.r << " " << best.g << " " << best.b << endl;
		// }

		colour = colour + best_hit->what->material->compute_once(ray, *best_hit, recurse); 

		while (lights != (Light*)0)
		{
			Vector lightDir;

			lights->get_direction(best_hit->position, lightDir);
			lightDir.negate();
		 	colour = colour + best_hit->what->material->compute_per_light(ray.direction, *best_hit, lightDir);
			lights = lights->next;
		}

        delete best_hit;
    } 
    else
	{
		colour.r = 0.0f;
		colour.g = 0.0f;
		colour.b = 0.0f;
		colour.a = 1.0f;
	}

	
}


void PhotonScene::raytrace_photon(int recurse)
{
  	Light *lights = light_list;

	int max_photons = 1000;
	int caustics_max_photons = 1000;

	Photon *photons = (Photon*)malloc(sizeof(Photon) * max_photons);
	Photon *caustics_photons = (Photon*)malloc(sizeof(Photon) * caustics_max_photons);
	photons_vec = std::vector<Photon>();
	caustics_vec = std::vector<Photon>();

	Vertex pos;
	std::mt19937 mt(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1, 1);

	// for each light in the scene
	while (lights != 0)
	{
		lights->get_position(pos);

		// generate photons
		lights->emit_photons(photons, max_photons);
		lights->emit_photons(caustics_photons, caustics_max_photons);

		lights = lights->next;
	}

	// loop through photons and trace them through the scene recording hits in the photon map
	for (int i = 0; i < max_photons; i++)
	{
		photons[i].mode = (Photon::Mode)0;
		photontrace(photons[i], recurse);
	}

	for (int i = 0; i < caustics_max_photons; i++)
	{
		caustics_photons[i].mode = (Photon::Mode)0;

		// projection map onto the global objects
		while(!projection(caustics_photons[i], recurse))
		{
			float x = 1,y = 1,z = 1;
			while ( x*x + y*y + z*z > 1.0 )
			{
				// generate three random numbers between -1 and 1
				x = (dist(mt));
				y = (dist(mt));
				z = (dist(mt));
			}

			caustics_photons[i].path.direction = Vector(x,y,z);
        	caustics_photons[i].path.direction.normalise();
		}
	}

	photon_map = new kdt::KDTree<Photon>(photons_vec);

	// print the size of the vector
	std::cout << "caustics_vec size: " << caustics_vec.size() << std::endl;
	caustics_map = new kdt::KDTree<Photon>(caustics_vec);
}

// photon trace function
void PhotonScene::photontrace(Photon photon, int recurse)
{
	// first step, find the closest primitive
	Hit *best_hit = this->trace(photon.path);
	
	// if we found a primitive
	if (best_hit != 0)
	{
		Object *object = best_hit->what;

		// record the photon in the photon map
		photon.hit_position = best_hit->position;

		object->material->compute_indirect_photon(photon, *best_hit, recurse);

	}

	delete best_hit;
	
}

bool PhotonScene::projection(Photon photon, int recurse) 
{
	// first step, find the closest primitive
	Hit *best_hit = this->trace(photon.path);
	
	// if we found a primitive
	if (best_hit != 0)
	{
		Object *object = best_hit->what;

		// check if the object that has been hit is a global material
		if (object->material->isGlobal())
		{
			//std::cout << "hit global object" << std::endl;

			// if it is then we need to trace the photon again
			caustictrace(photon, recurse);
			return true;
		}

		// print hit object but not global
		//std::cout << "hit object but not global" << std::endl;
	}

	return false;
}

// caustic trace function
void PhotonScene::caustictrace(Photon photon, int recurse)
{
	// first step, find the closest primitive
	Hit *best_hit = this->trace(photon.path);
	
	// if we found a primitive
	if (best_hit != 0)
	{
		Object *object = best_hit->what;

		// check if the object that has been hit is a global material
		if (object->material->isGlobal())
		{
			// if it is then we need to trace the photon again
			object->material->compute_indirect_photon(photon, *best_hit, recurse);
		}
		else
		{
			caustics_vec.push_back(photon);
		}

	}

	delete best_hit;
	
}





