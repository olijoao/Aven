#ifndef CAMERA_GLSL
#define CAMERA_GLSL

#pragma include "shader/lib/geometry.glsl"
#pragma include "shader/lib/sample.glsl"


//camera
struct Camera {
    vec3 pos;
    vec2 invFilmSize;
    vec3 right;     // magnitude == width/2  of ImagePlane
    vec3 up;        // magnitude == height/2 of ImagePlane
    vec3 forward;   // normalized ! imagePlane is 1 unit away from camera.pos
    vec3 color_background_sky;
    vec3 color_background_ground;
    int nbrBounces;
    int nbrSamplesPerIteration;
};



//pos components are in [-1,1], top right is (1,1)
Ray camera_createRayAt(Camera cam, vec2 pos) {
    pos += cam.invFilmSize * (vec2(rand(), rand()) - 0.5f);

    vec3 posOnImagePlane =      cam.forward
                            +   cam.right * pos.x
                            +   cam.up * pos.y;

    return Ray(cam.pos, normalize(posOnImagePlane));
}


#endif	//CAMERA_GLSL