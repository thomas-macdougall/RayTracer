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

/* CSG is an object that is built by Constructive Solid Geometry from two sub-objects.*/

#include "csg_object.h"

CSG::CSG(CSG::Mode p_mode, Object* p_left, Object* p_right)
{
	mode = p_mode;
	left = p_left;
	right = p_right;
	next = (Object*)0;
}

Hit* CSG::intersection(Ray ray)
{
	Hit* result = 0;

	// implement csg intersection
	// you can use the following code to get the intersection of the two sub-objects
	// and then apply the csg operation

	Hit* left_hit = left->intersection(ray);
	Hit* right_hit = right->intersection(ray);

	Hit* temp = 0;
	
	// if mode is union
	if (mode == CSG_UNION)
	{
		temp = result;
		// insert all left hits into result
		while (left_hit != 0 || right_hit != 0)
		{
			if (left_hit != 0 && right_hit != 0)
			{
				if (left_hit->t < right_hit->t)
				{
					temp = left_hit;
					left_hit = left_hit->next;
				}
				else
				{
					temp = right_hit;
					right_hit = right_hit->next;
				}
			}
			else if (left_hit == 0 && right_hit != 0)
			{
				temp = right_hit;
				right_hit = right_hit->next;
			}
			else
			{
				temp = left_hit;
				left_hit = left_hit->next;
			}

			// Insert first hit to the head
			if (result == 0) 
			{
				result = temp;
			}
			temp = temp->next;
		}
	}

	// if mode is intersection
	if (mode == CSG_INTER)
	{
		// find the first hit
		if (right_hit != 0 && left_hit != 0)
		{
			if (right_hit->t < left_hit->t)
			{
				result = left_hit;

				// find the first hit in the right list that has a greater t value than the first hit in the left list
				while (right_hit != 0 && right_hit->t < left_hit->t)
				{
					right_hit = right_hit->next;
				}
			}
			else
			{
				result = right_hit;

				// find the first hit in the left list that has a greater t value than the first hit in the right list
				while (left_hit != 0 && left_hit->t < right_hit->t)
				{
					left_hit = left_hit->next;
				}
			}

			temp = result;

			// find all the t values before the end of one of the lists
			while (left_hit != 0 && right_hit != 0)
			{
				if (left_hit->t < right_hit->t)
				{
					temp = left_hit;
					left_hit = left_hit->next;
				}
				else
				{
					temp = right_hit;
					right_hit = right_hit->next;
				}
				temp = temp->next;
			}
		}
		else 
		{
			return result;
		}

	}

	// if mode is difference
	if (mode == CSG_DIFF)
	{
		int inRight = 0;
		temp = result;

		// find the left hits that don't have a corresponding right hit
		// and return them
		while (left_hit != 0)
		{
			if (right_hit == 0)
			{
				temp = left_hit;
				// Insert first hit to the head
				if (result == 0) 
				{
					result = temp;
				}
				left_hit = left_hit->next;
				temp = temp->next;
			}
			else if (left_hit->t < right_hit->t)
			{
				if (inRight == 0)
				{
					temp = left_hit;
					// Insert first hit to the head
					if (result == 0) 
					{
						result = temp;
					}
					left_hit = left_hit->next;
					temp = temp->next;
				}
				else
				{
					left_hit = left_hit->next;
				}
			}
			else 
			{
				right_hit = right_hit->next;
				inRight = 1;
			}
		}
	}

	return result;
}

void CSG::apply_transform(Transform& transform)
{
	left->apply_transform(transform);
	right->apply_transform(transform);
}
