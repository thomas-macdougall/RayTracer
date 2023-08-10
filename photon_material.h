#include "photon.h"
#include "material.h"
#include "environment.h"
#include <random>
#pragma once

class PhotonMaterial: public Material {
public:
    Environment *env;
    Colour specular;
    Colour diffuse;
    
    void compute_indirect_photon(Photon &photon, Hit &best_hit, int recurse);
    Colour compute_once(Ray &viewer, Hit &hit, int recurse);

    PhotonMaterial(Environment *env, Colour spec, Colour dif);
  
};
