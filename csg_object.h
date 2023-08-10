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

 /* CSG is an object that is built by Constructive Solid Geometry from two sub-objects.
  * It supports three operations Union, Intersection and Difference of the two sub-objects.
  * */
 
#pragma once

#include "object.h"

class CSG : public Object
{
public:
	enum Mode {
		CSG_UNION=0,
		CSG_INTER,
		CSG_DIFF
	};

	Mode mode;
	Object* left;
	Object* right;

	CSG(CSG::Mode p_mode, Object* p_left, Object* p_right);

	Hit* intersection(Ray ray);
	void append(Hit* hit, Hit* result);
	void apply_transform(Transform& transform);
};
