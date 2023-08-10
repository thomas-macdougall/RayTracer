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

/* This is the top level for the program you need to create for lab three and four.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// these are core raytracing classes
#include "framebuffer.h"
#include "scene.h"

// classes that contain our objects to be rendered, all derived from Object
#include "polymesh_object.h"
#include "sphere_object.h"
#include "plane_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "phong_material.h"
#include "falsecolour_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"
#include "global_material.h"
#include "quadratic_object.h"
#include "compound_material.h"
#include "csg_object.h"
#include "photon_material.h"
#include "p_scene.h"

using namespace std;

// you will find it useful during development/debugging to create multiple functions that fill out the scene.
void build_scene(PhotonScene& scene)
{
	// The following transform allows 4D homogeneous coordinates to be transformed.It moves the supplied teapot model to somewhere visible.
	Transform * transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, -2.7f,
			0.0f, 1.0f, 0.0f, 5.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

	// Read in the teapot model.
	//PolyMesh* pm = new PolyMesh("teapot_smaller.ply", false);

	// Quadratic * sphere = new Quadratic(1, 0, 0, 0, 1, 0, 0, 1, 0, -0.1);
	// Quadratic * cylinder = new Quadratic(0, 0, 0, 0, 1, 0, 0, 1, 0, -0.03);

	//pm->apply_transform(*transform);
	Sphere * sphere = new Sphere(Vertex(0.4f, 0.0f, 0.3f), 0.3f);
	Sphere* sphere2 = new Sphere(Vertex(-0.2f, -0.0f, 0.3f), 0.3f);
	// Sphere* sphere3 = new Sphere(Vertex(0.2f, -0.2f, 0.6f), 0.2f);
	// Sphere* spherex = new Sphere(Vertex(0.0f, 0.0f, 0.0f), 0.2f);
	// Sphere* sphere2 = new Sphere(Vertex(0.1f, 0.0f, 0.0f), 0.2f);

	// Sphere* sphere2 = new Sphere(Vertex(0.0f, 0.0f, 1.0f), 0.7f);
	// CSG * csg = new CSG(CSG::CSG_DIFF, sphere2, sphere);

	//Plane* pm = new Plane(-1.0f, 0.0f, 0.0f, 0.8f);

	//DirectionalLight* dl = new DirectionalLight(Vector(1.01f, -1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f));

	PointLight* dl = new PointLight(Vertex(0.0f, 0.5f, 0.0f), Colour(1.0f, 1.0f, 1.0f));
	scene.add_light(dl);

	FalseColour * fc = new FalseColour();
	// Phong* bp1 = new Phong(Colour(0.01f, 0.0f, 0.0f), Colour(0.6f, 0.0f, 0.0f), Colour(0.8f, 0.8f, 0.8f), 40.f); //red
	// Phong* blue = new Phong(Colour(0.0f, 0.0f, 0.2f), Colour(0.0f, 0.0f, 0.6f), Colour(0.8f, 0.8f, 0.8f), 40.f); //blue
	// Phong* bp2 = new Phong(Colour(0.0f, 0.01f, 0.0f), Colour(0.3f, 0.3f, 0.3f), Colour(0.4f, 0.4f, 0.4f), 40.f);
	GlobalMaterial * bp3 = new GlobalMaterial(&scene, Colour(0.1f, 0.1f, 0.1f), Colour(0.9f, 0.9f, 0.9f), 0.0f);
	GlobalMaterial * bp4 = new GlobalMaterial(&scene, Colour(0.3f, 0.1f, 0.1f), Colour(0.9f, 0.9f, 0.9f), 1.8f);


	Phong* white = new Phong(&scene, Colour(0.01f, 0.01f, 0.01f), Colour(1.0f, 1.0f, 1.0f), Colour(0.4f, 0.4f, 0.4f), 40.f); //white
	Phong* blue = new Phong(&scene, Colour(0.01f, 0.01f, 0.01f), Colour(0.0f, 0.0f, 1.0f), Colour(0.4f, 0.4f, 0.4f), 40.f); //blue
	Phong* bp2 = new Phong(&scene, Colour(0.01f, 0.01f, 0.01f), Colour(0.3f, 0.3f, 0.3f), Colour(0.4f, 0.4f, 0.4f), 40.f);
	Phong* green = new Phong(&scene, Colour(0.01f, 0.01f, 0.01f), Colour(0.0f, 1.0f, 0.0f), Colour(0.1f, 0.1f, 0.1f), 40.f); //green
	Phong* red = new Phong(&scene, Colour(0.001f, 0.001f, 0.001f), Colour(0.7f, 0.1f, 0.0f), Colour(0.1f, 0.1f, 0.1f), 40.f); //green

	
	// PhotonMaterial * bp3 = new PhotonMaterial(&scene, Colour(0.0f, 0.0f, 0.8f), Colour(0.5f, 0.5f, 0.5f));
	// PhotonMaterial * bp4 = new PhotonMaterial(&scene, Colour(0.8f, 0.0f, 0.0f), Colour(0.5f, 0.5f, 0.5f));
	// PhotonMaterial * sphere_col = new PhotonMaterial(&scene, Colour(1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f));
	// PhotonMaterial * bp5 = new PhotonMaterial(&scene, Colour(0.0f, 0.8f, 0.0f), Colour(0.5f, 0.5f, 0.5f));

	// CompoundMaterial* compound =  new CompoundMaterial(2);
	// compound->include_material(bp1);
	// compound->include_material(bp3);

	// plane underneath
	Plane* plane = new Plane(0.0f, 1.0f, 0.0f, 0.4f);
	plane->set_material(white);
	scene.add_object(plane);

	//plane on top
	Plane* plane1 = new Plane(0.0f, -1.0f, 0.0f, 1.0f);
	plane1->set_material(blue);
	scene.add_object(plane1);

	// plane on the left side
	Plane* plane2 = new Plane(1.0f, 0.0f, 0.0f, 1.0f);
	plane2->set_material(red);
	scene.add_object(plane2);

	//plane on the right side
	Plane* plane3 = new Plane(-1.0f, 0.0f, 0.0f, 1.0f);
	plane3->set_material(red);
	scene.add_object(plane3);

	//plane on the back
	Plane* plane4 = new Plane(0.0f, 0.0f, -1.0f, 1.0f);
	plane4->set_material(green);
	scene.add_object(plane4);

	sphere->set_material(bp4);
	scene.add_object(sphere);

	sphere2->set_material(bp3);
	scene.add_object(sphere2);

	// // sphere->set_material(bp1);
	// sphere3->set_material(bp4);
	// scene.add_object(sphere);
	// scene.add_object(sphere3);

	// add cylinder to the scene
	// spherex->set_material(white);
	// sphere2->set_material(green);
	// cylinder->set_material(blue);
	
	// CSG * csg = new CSG(CSG::CSG_INTER, spherex, sphere2);
	// scene.add_object(csg);


	// add polymesh to the scene
	// pm->set_material(blue);
	//scene.add_object(pm);
	
}


// This is the entry point function to the program.
int main(int argc, char *argv[])
{
	int width = 512;
	int height = 512;
	// Create a framebuffer
	FrameBuffer* fb = new FrameBuffer(width, height);
	
	// Create a scene
	PhotonScene scene;
	
	// Setup the scene
	build_scene(scene);
	
	// Declare a camera
	//Camera *camera = new SimpleCamera(0.1f);
	Camera* camera = new FullCamera(0.5f, Vertex(0.0f, 0.0f, -1.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f));
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"test.ppm");
	fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
