#include "scene.h"

class PhotonScene: public Scene
{
public:
    int num_photons;

    PhotonScene();

    void raytrace_photon(int recurse);
    void photontrace(Photon photon, int recurse);
    void caustictrace(Photon photon, int recurse);
    void raytrace(Ray ray, int recurse, Colour& colour, float& depth);
    bool projection(Photon photon, int recurse);
};
