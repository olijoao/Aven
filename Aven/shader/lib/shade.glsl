
#ifndef SHADE_GLSL
#define SHADE_GLSL

#pragma include "shader/lib/math.glsl"

// see pbr book page 514
// interface
//		vec3 X_f(in const vec3 wi, in const vec3 wo):
//			 given pair of directions returns the value of the distribution function
//		vec3 X_Sample_f(const vec3 &wo, vec3 *wi, out float pdf):
//			computes the direction of incident light ωi given an outgoing direction ωo and returns the value of the BxDF for the pair of directions.


// Schlick's approximation for reflectance.
float schlick(float cosine, float ref_idx) {
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}



//material type enum
const uint MATERIAL_TYPE_SKY            = 0;
const uint MATERIAL_TYPE_LAMBERT        = 1;
const uint MATERIAL_TYPE_METAL          = 2;    
const uint MATERIAL_TYPE_DIELECTRIC     = 3;
const uint MATERIAL_TYPE_VOLUME         = 4;
const uint MATERIAL_TYPE_GUI            = 5;



struct Hit {
    vec3    pos;
    vec3    normal;                 // always pointing out of geometry
    float   t;                      // t along ray to intersection
    bool    frontFace;              // false = hit backface
    uint    material_type;
    vec3    material_albedo;        //used in LAMBERTIAN, METAL
    float   material_ir;            //used by dielectric
    float   material_fuzz;          //used in METAL, in [0,1]
    bool    volume_renderModeHybrid;
};



// helper functions to create Hit objects

Hit create_hit_sky(vec3 skyColor) {
    return Hit(vec3(0, 0, 0), vec3(0, 0, 0), FLOAT_MAX, false, MATERIAL_TYPE_SKY, skyColor, 0, 0, false); 
}

Hit create_hit_GUI(vec3 color, float t) {
    return Hit(vec3(0, 0, 0), vec3(0, 0, 0), t, false, MATERIAL_TYPE_GUI, color, 0, 0, false);
}

Hit create_hit_Volume(vec3 pos, vec3 normal, float t, vec3 albedo, bool volume_renderModeHybrid){
    return Hit(pos, normal, t, true, MATERIAL_TYPE_VOLUME, albedo, 0, 0, volume_renderModeHybrid);
}

Hit create_hit_Lambert(vec3 pos, vec3 normal, float t, vec3 albedo) {
    return Hit(pos, normal, t, true, MATERIAL_TYPE_LAMBERT, albedo, 0, 0, false);
}

Hit create_hit_Metal(vec3 pos, vec3 normal, float t, vec3 albedo, float fuzz) {
    return Hit(pos, normal, t, true, MATERIAL_TYPE_METAL, albedo, 0, fuzz, false);
}

Hit create_hit_Dielectric(vec3 pos, vec3 normal, float t, bool frontFace, float ir) {
    return Hit(pos, normal, t, frontFace, MATERIAL_TYPE_DIELECTRIC, vec3(0,0,0), ir, 0, false);
}






//returns true if ray was sattered
bool material_scatter(const Hit hit, const Ray ray, inout vec3 attenuation, out Ray scattered) {
    switch (hit.material_type) {
        case MATERIAL_TYPE_SKY:
        case MATERIAL_TYPE_GUI:
            attenuation = hit.material_albedo;
            return false;

        case MATERIAL_TYPE_LAMBERT:
            vec3 scatter_direction = randomHemispherePoint(hit.normal);
            scattered = Ray(hit.pos + 0.001 * scatter_direction, scatter_direction);
            attenuation = hit.material_albedo;
            return true;

        case MATERIAL_TYPE_VOLUME:
            if (!hit.volume_renderModeHybrid) {
                attenuation = hit.material_albedo;
                scattered = Ray(hit.pos, randomSpherePoint());
                return true;
            }

            vec3 normal;
            float length = length(hit.normal);
            if(length > rand())
                normal = normalize(hit.normal);
            else{
                attenuation = hit.material_albedo;
                scattered = Ray(hit.pos, randomSpherePoint());
                return true;
            }
            



        //falls through :)
        case MATERIAL_TYPE_METAL:
            vec3 reflected = reflect(ray.dir, normal);
            reflected += hit.material_fuzz * randomHemispherePoint(reflected);
            reflected = normalize(reflected);
            scattered = Ray(hit.pos +0.001*reflected, reflected);
            attenuation = hit.material_albedo;
            bool valid = dot(scattered.dir, normal) > 0;
            if (!valid)
                attenuation = vec3(0,0,0);
            return valid;

        case MATERIAL_TYPE_DIELECTRIC:
            attenuation = vec3(0.75, 0.75, 0.75);
            float ratio_refraction = hit.frontFace ? (1.0f) / hit.material_ir : hit.material_ir;
            float cos_theta     = min(dot(-ray.dir, hit.normal), 1.0);
            double sin_theta    = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = ratio_refraction * sin_theta > 1.0;
            vec3 direction;
            if (cannot_refract || schlick(cos_theta, ratio_refraction) > rand())
                direction = reflect(ray.dir, hit.normal);
            else
                direction = refract(-ray.dir, hit.normal, ratio_refraction);
            direction = normalize(direction);
            scattered = Ray(hit.pos +0.001* direction, direction);
            return true;
    }
    return false; 
}


#endif //SHADE_GLSL