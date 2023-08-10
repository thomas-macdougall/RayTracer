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
// Environment is the base class for raytracing. We use this in material to do recursion as that allows
// Scene which is derived from this to depend (indirectly) on Material.

#pragma once

#include "ray.h"
#include "colour.h"
#include "photon.h"
#include "src/tree.h"
#include "kdtree.h"

// struct CORE
// {
//   typedef Photon* Item;  // The type we store in the tree.
//   typedef Vertex  Point; // The type we read coordinates from.
//   typedef float  Coord; // The type of individual coordinates.

//   static const int DIMENSIONS =  3; // We're in a three-dimensional space.
//   static const int MAX_DEPTH  = 20; // The tree will reach at most ten levels.
//   static const int STORAGE    =  8; // Leaves can hold eight items before splitting.

//   // Get the distance of a point along the given axis.
//   static Coord coordinate(const Point& point, int axis)
//   {
// 	switch (axis)
// 	{
// 	case 0:
// 		return point.x;
// 		break;
// 	case 1:
// 		return point.y;
// 		break;
// 	case 2:
// 		return point.z;
// 		break;
// 	default:
// 		return 0.0f;
// 	}
//   }

//   // Get the location of an item
//   static const Point& point(const Item& item)
//   {
//     return item->hit_position;
//   }
// };

class Environment {
public:

	// Begin Stage 3
    kdt::KDTree<Photon> *photon_map;
    kdt::KDTree<Photon> *caustics_map;
    std::vector<Photon> photons_vec;
    std::vector<Photon> caustics_vec;
	
    // Exit stage 

	int num_photons;

	// shoot a ray into the environment and get the colour and depth.
	// recurse indicates the level of recursion permitted.
	virtual void raytrace(Ray ray, int recurse, Colour& colour, float& depth)
	{
		colour.r = 0.0f;
		colour.g = 0.0f;
		colour.b = 0.0f;
		depth = 100000000.0f;
	}

	// raytrace a shadow ray. returns true if intersection found between 0 and limit along ray.
	virtual bool shadowtrace(Ray, float limit)
	{
		return false;
	}

	//BEGIN_STAGE_THREE

	// trace a photon through the environment
	virtual void photontrace(Photon photon, int recurse)
	{

	}

	// trace the caustic photons through the environment
	virtual void caustictrace(Photon photon, int recurse)
	{

	}

	virtual void raytrace_photon(int recurse)
	{

	}

	virtual bool projection(Photon photon, int recurse)
	{
		return false;
	}

	//BEGIN_STAGE_THREE
};
