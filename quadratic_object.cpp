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

#include "quadratic_object.h"
#include <math.h>

using namespace std;

Quadratic::Quadratic(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j)
{
  //set the coefficients
  A = a;
  B = b;
  C = c;
  D = d;
  E = e;
  F = f;
  G = g;
  H = h;
  I = i;
  J = j;

  next = (Object *)0;
}

Hit *Quadratic::intersection(Ray ray)
{
  //create a hit object
  Hit *hit = new Hit();
  Hit *hit1 = new Hit();

  Vector dir = ray.direction;
  Vertex pos = ray.position;

  //grouped common terms
  float a = A * pow(dir.x, 2) + (2*B*dir.x*dir.y) + (2*C*dir.x*dir.z) 
            + E*pow(dir.y, 2) + (2*F*dir.y*dir.z) + H*pow(dir.z, 2);

  float b = 2*(A*pos.x*dir.x + B*(pos.x*dir.y + pos.y*dir.x) + C*(pos.x*dir.z + pos.z*dir.x) + D*dir.x +
            + E*pos.y*dir.y + F*(pos.y*dir.z + pos.z*dir.y) + G*dir.y + H*dir.z*pos.z + I*dir.z);

  float c = A*pow(pos.x, 2) + (2*B*pos.x*pos.y) + (2*C*pos.x*pos.z) + (2*D*pos.x) + E*pow(pos.y, 2) 
            + (2*F*pos.y*pos.z) + (2*G*pos.y) + H*pow(pos.z,2) + 2*I*pos.z + J;

  //calculate the discriminant
  float discriminant = pow(b, 2) - 4 * a * c;

  if (discriminant < 0)
  {
    return 0;
  }

  //if the discriminant is less than 0, there is no intersection
  if (a == 0)
  {
    hit->t = -c/b;

    //calculate the hit point
    calcHit(hit, ray);
    hit->entering = true;

  
    delete hit1;
    return hit;
  }
  else
  {
    //calculate the two roots
    hit->t = (-b - sqrt(discriminant)) / (2 * a);
    hit1->t = (-b + sqrt(discriminant)) / (2 * a);

    // if hit1 is closer than hit, swap them
    if (hit1->t < hit->t)
    {
      float temp = hit->t;
      hit->t = hit1->t;
      hit1->t = temp;
    }

  }

  calcHit(hit, ray);
  hit->entering = true;
  calcHit(hit1, ray);
  hit1->entering = false;

  hit->next = hit1;

  return hit;
}

//calculate hit point and normal
void Quadratic::calcHit(Hit *hit, Ray ray)
{
  
  //calculate the intersection point
  hit->position = ray.position + hit->t * ray.direction;
  //calculate the normal
  float x_normal = A * hit->position.x + B * hit->position.y + C * hit->position.z + D;
  float y_normal = B * hit->position.x + E * hit->position.y + F * hit->position.z + G;
  float z_normal = C * hit->position.x + F * hit->position.y + H * hit->position.z + I;

  hit->normal = Vector(x_normal, y_normal, z_normal);
  hit->normal.normalise();

  //check that the normal is pointing in the correct direction
  if (hit->normal.dot(ray.direction) > 0)
  {
    hit->normal = -hit->normal;
  }

  hit->what = this;
}

void Quadratic::apply_transform(Transform& trans)
{
  //apply transform, trans, to this quadratic object
  Transform t_t = trans.transpose();
  Transform q = Transform(A, B, C, D, B, E, F, G, C, F, H, I, D, G, I, J);

  q = t_t * q * trans;

  //set the coefficients
  A = q.matrix[0][0];
  B = q.matrix[0][1];
  C = q.matrix[0][2];
  D = q.matrix[0][3];
  E = q.matrix[1][1];
  F = q.matrix[1][2];
  G = q.matrix[1][3];
  H = q.matrix[2][2];
  I = q.matrix[2][3];
  J = q.matrix[3][3];

}
