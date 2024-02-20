
#ifndef SHADE_GLSL
#define SHADE_GLSL

#include "shader/lib/math.glsl"

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
const uint MATERIAL_TYPE_VOLUME         = 1;
const uint MATERIAL_TYPE_GUI            = 2;



struct Hit {
    vec3    pos;
    vec3    normal;                 // always pointing out of geometry
    float   t;                      // t along ray to intersection
    uint    material_type;
    vec3    material_albedo;        //used in LAMBERTIAN, METAL
    bool    volume_renderModeHybrid;
};



// helper functions to create Hit objects

Hit create_hit_sky(vec3 skyColor) {
    return Hit(vec3(0, 0, 0), vec3(0, 0, 0), FLOAT_MAX, MATERIAL_TYPE_SKY, skyColor, false); 
}

Hit create_hit_GUI(vec3 color, float t) {
    return Hit(vec3(0, 0, 0), vec3(0, 0, 0), t, MATERIAL_TYPE_GUI, color, false);
}

Hit create_hit_Volume(vec3 pos, vec3 normal, float t, vec3 albedo, bool volume_renderModeHybrid){
    return Hit(pos, normal, t, MATERIAL_TYPE_VOLUME, albedo, volume_renderModeHybrid);
}



vec3 material_f(const Hit hit, const vec3 wo, const vec3 wi) {
	 if (!hit.volume_renderModeHybrid) 
		return 1.0f/PI4 * hit.material_albedo;
        
	float length = length(hit.normal);
	if(length < rand() || length < 0.05)
		return 1.0f/PI4 * hit.material_albedo;
	
    vec3 normal = normalize(hit.normal);
	vec3 reflectDir = reflect(-wi, normal);  

    float spec = pow(max(dot(wo, reflectDir), 0.0), 32);
    float diff = 1.0f/PI2 * dot(normal, wi);
	if (dot(wi, normal) <= 0)
		return vec3(0,0,0);

	return (spec+diff) * hit.material_albedo;
}




#endif //SHADE_GLSL