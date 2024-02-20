#version 460 core

#include "shader/lib/blend.glsl"
#include "shader/lib/camera.glsl"
#include "shader/lib/geometry.glsl"
#include "shader/lib/math.glsl"
#include "shader/lib/sample.glsl"
#include "shader/lib/shade.glsl"
#include "shader/lib/volume.glsl"


const int MAX_NUMBER_LIGHTS             = 5;     // don't forget to change in scene.h as well

const int VOLUME_TYPE_DEFAULT           = 0;
const int VOLUME_TYPE_BLENDED_NORMAL    = 1;
const int VOLUME_TYPE_BLENDED_ERASE     = 2;
const int VOLUME_TYPE_BLENDED_INSIDE    = 3;
const int VOLUME_TYPE_BLENDED_COLOR     = 4;

in vec2 pos_Pixel;
out vec4 FragColor;

//uniforms
uniform Camera      camera;
uniform vec2        rand_seed;
uniform sampler2D	textureTarget;
uniform uint        renderIteration;
uniform int         nbrSamplesPerIteration;

// volume
uniform int         volume_type;
uniform vec3        volume_pos;
uniform float       volume_density;
uniform float       volume_opacity;     //used with VOLUME_TYPE_BLENDED
uniform int         volune_isDisplayingBoundingBox;
uniform int         volume_renderModeHybrid;
uniform float       volume_stepSize;
uniform vec3        invVolumeSize;


// lights
const int LIGHT_TYPE_DIRECTION  = 0;
const int LIGHT_TYPE_POINT      = 1;
const int LIGHT_TYPE_SPOTLIGHT  = 2;

struct	Light{
    mat4 transformation;
    int type;
    vec3 color;
    float falloff_inCosRadians;
};

uniform int     nbrOfLights;
uniform Light   lights[MAX_NUMBER_LIGHTS];
uniform vec3    background_sky;
uniform vec3    background_ground;
uniform float   background_intensity;
uniform int     background_useAsLight;


vec4 getVoxelAt(vec3 pos) {
    ivec3 ipos = ivec3(pos * vec3(getVolumeSize()));

    switch (volume_type) {
        case VOLUME_TYPE_DEFAULT: {
            return getVolume_src(ipos);
        }
        case VOLUME_TYPE_BLENDED_NORMAL:{
            vec4 top    = getVolume_dst(ipos);
            vec4 bottom = getVolume_src(ipos);  
            top.a       *= volume_opacity;
            return blend_over(top, bottom);
        }
        case VOLUME_TYPE_BLENDED_ERASE: {
            vec4 top        = getVolume_dst(ipos);
            vec4 bottom     = getVolume_src(ipos);
            bottom.a        = bottom.a - volume_opacity * top.a;
            return bottom;
        }
        case VOLUME_TYPE_BLENDED_INSIDE: {
            vec4 bottom = getVolume_src(ipos);
            if (bottom.a == 0) 
                return vec4(0);
		    vec4 top = getVolume_dst(ipos);
			return blend_over(top, bottom);
        }
        case VOLUME_TYPE_BLENDED_COLOR: {
            vec4 bottom = getVolume_src(ipos);
            if (bottom.a == 0) 
                return vec4(0);
		    vec4 top = getVolume_dst(ipos);
            vec4 c = blend_over(top, bottom);
            return vec4(c.rgb, bottom.a);
        }
    }
    return vec4(1, 0, 1, 1);
}




vec3 getGradientAt(vec3 p) {
    vec3 delta = invVolumeSize;
    vec3 sample1, sample2;

    sample1.x = getVoxelAt(p - vec3(delta.x, 0, 0)).a;
    sample2.x = getVoxelAt(p + vec3(delta.x, 0, 0)).a;
    sample1.y = getVoxelAt(p - vec3(0, delta.y, 0)).a;
    sample2.y = getVoxelAt(p + vec3(0, delta.y, 0)).a;
    sample1.z = getVoxelAt(p - vec3(0, 0, delta.z)).a;
    sample2.z = getVoxelAt(p + vec3(0, 0, delta.z)).a;
    return sample1 - sample2;
    
}


vec3 worldToVolume(vec3 posWorld, vec3 posVolume, vec3 scale) {
    return (posWorld-posVolume) / (getVolumeSize() * scale) + vec3(0.5f, 0.5f, 0.5f);
}


vec3 sky_color(vec3 dir) {
    float t = 0.5 * (-dir.y + 1.0);
    return mix(background_sky, background_ground, t);
}

float filteredGrid( in vec2 p, in vec2 dpdx, in vec2 dpdy ) {
    const float N = 10.0;
    vec2 w = max(abs(dpdx), abs(dpdy));
    vec2 a = p + 0.5*w;                        
    vec2 b = p - 0.5*w;           
    vec2 i = (floor(a)+min(fract(a)*N,1.0)-
              floor(b)-min(fract(b)*N,1.0))/(N*w);
    return (1.0-i.x)*(1.0-i.y);

}


Hit intersectScene(Ray ray, int iteration) {
    vec3 skycolor = sky_color(ray.dir);
    Hit nearestHit = create_hit_sky(skycolor);

    // ground
    if (ray.dir != 0) {
        float t = -ray.origin.y / ray.dir.y;
        if (t > 0 && t < nearestHit.t) {
            const float grid_size = 8;
            const float grid_size2 = 8*8;
            vec3 intersection = ray.origin + ray.dir * t;
            float dist = distance(intersection, camera.pos);
            float falloff = 100/max(dist,100);
            
            vec2 grid_8     = abs(mod(intersection.xz+grid_size/2.0f, vec2(grid_size)) -  grid_size/2.0f);
            vec2 grid_64    = abs(mod(intersection.xz+grid_size2/2.0f, vec2(grid_size2)) -  grid_size2/2.0f);
            if( min(grid_64.x, grid_64.y) < 0.2f) 
				nearestHit = create_hit_GUI( falloff * vec3(1,1,1), t);
            else if( min(grid_8.x, grid_8.y) < 0.1f ) 
				nearestHit = create_hit_GUI( falloff * vec3(0.5f), t);
        }
    }

    //AABB
    vec3 scale = vec3(1.0f);
    vec3 aabbSize = vec3(getVolumeSize()) * scale;
    AABB aabb = AABB(volume_pos - aabbSize / 2, volume_pos + aabbSize / 2);
    vec2 t_aabb;
    if (intersect(ray, aabb, t_aabb) && t_aabb.x < nearestHit.t) {
        vec3 intersection = ray.origin + ray.dir * max(t_aabb.x, 0);
        vec3 normal = vec3(0,0,0);
            
        if (volune_isDisplayingBoundingBox != 0 && iteration == 0) {
            //intersect bounding box
            const float boundingBoxSize     = 1;
            const vec3  boundingBoxColor    = vec3(1,1,1);

            vec3 boundingBox = aabbSize/2 - boundingBoxSize;

            // bounding box edges front
            vec3 intersection_front_local = intersection - volume_pos;
            if(     t_aabb.x <= nearestHit.t
                &&  any(greaterThan(abs(intersection_front_local.xy), boundingBox.xy))
                &&  any(greaterThan(abs(intersection_front_local.xz), boundingBox.xz))
                &&  any(greaterThan(abs(intersection_front_local.yz), boundingBox.yz)))
            {
                nearestHit = create_hit_GUI(boundingBoxColor, t_aabb.x);
            }

            // bounding box edges back
            vec3 intersection_back_local = ray.origin + ray.dir * max(t_aabb.y, 0) - volume_pos;
            if (    t_aabb.y <= nearestHit.t
                &&  any(greaterThan(abs(intersection_back_local.xy), boundingBox.xy))
                &&  any(greaterThan(abs(intersection_back_local.xz), boundingBox.xz))
                &&  any(greaterThan(abs(intersection_back_local.yz), boundingBox.yz)))
            {
                nearestHit = create_hit_GUI(boundingBoxColor, t_aabb.y);
            }
        }

        float stepSize = volume_stepSize;
        if (iteration > 0)
            stepSize *= 2;

        float t = t_aabb.x + rand() * stepSize;;

      
        while (true) {
            if (t >= t_aabb.y || t >= nearestHit.t)
                break;

            vec3 samplePos = ray.origin + ray.dir * t;
            vec3 posLocal = worldToVolume(samplePos, volume_pos, scale);
            vec3 jitter = vec3(rand(), rand(), rand()) - 0.5f;

            posLocal += invVolumeSize * jitter;    //done here since used in getVoxelAt() and getGradientAt()

            vec4 voxel = getVoxelAt(posLocal);
            float density = voxel.a * volume_density;
            if (density > rand()) {
                nearestHit = create_hit_Volume(samplePos, getGradientAt(posLocal), t, voxel.rgb, volume_renderModeHybrid != 0);
            }

            t += stepSize;
        }

    }

    return nearestHit;
}



// t = amount of dir that light is away. important for shadow testing point and spotlights 
vec3 light_sample(vec3 pos_it, out vec3 dir, out float t) {
    int lightCount = nbrOfLights; 
    if (background_useAsLight != 0) lightCount++;
    if (lightCount == 0)            return vec3(0, 0, 0);
        
    int selectedLight = int(rand() * float(lightCount));
    if (selectedLight >= nbrOfLights) {
        dir = randomSpherePoint();
        t = FLOAT_MAX;
        return sky_color(dir) * background_intensity * lightCount;
    }

    int type = lights[selectedLight].type;

    switch (type) {
    case LIGHT_TYPE_DIRECTION: {
        dir = (lights[selectedLight].transformation * vec4(0, 1, 0, 0)).xyz;
        t = FLOAT_MAX;
        return lights[selectedLight].color * lightCount; 
    }

    case LIGHT_TYPE_POINT: {
        vec3 light_pos = (lights[selectedLight].transformation * vec4(0, 0, 0, 1)).xyz;
        dir = normalize(light_pos - pos_it);
        float distSquared = distanceSquared(light_pos, pos_it);
        t = sqrt(distSquared);
        return lights[selectedLight].color * lightCount / distSquared;
    }

    case LIGHT_TYPE_SPOTLIGHT: {
        vec3 light_pos = (lights[selectedLight].transformation * vec4(0, 0, 0, 1)).xyz;
        dir = normalize(light_pos - pos_it);
        float distSquared = distanceSquared(light_pos, pos_it);
        t = sqrt(distSquared);
        mat4 worldToLight = inverse(lights[selectedLight].transformation);
        vec3 lightdir_local = normalize((worldToLight * vec4(dir, 0)).xyz);
        float cosTheta = lightdir_local.y;
        float falloff;
        if (cosTheta < lights[selectedLight].falloff_inCosRadians)
            falloff = 0;
        else
            falloff = 1;
        return falloff * lights[selectedLight].color * lightCount / distSquared;
    }
    }
    
    // error
    return vec3(0, 1, 0);
    
}



vec3 getRayColor(Ray ray) {
	Hit hit = intersectScene(ray, 0);
    if (hit.material_type == MATERIAL_TYPE_SKY || hit.material_type == MATERIAL_TYPE_GUI)
        return hit.material_albedo;

    float t_light;
    vec3 dir_light;
    vec3 L = light_sample(hit.pos+hit.normal*0.0001, dir_light, t_light);

    vec3 material = material_f(hit, -ray.dir, dir_light);

    // shadow test
    if (L != vec3(0,0,0) && material != 0) {
        Ray ray_light = Ray(hit.pos, dir_light);
        Hit hit2 = intersectScene(ray_light, 1);
        if (t_light > hit2.t )
            L = vec3(0,0,0);
    }

    return material * L * hit.material_albedo;
        
}


void main() {
    srand(pos_Pixel.x  * rand_seed.x + pos_Pixel.y * rand_seed.y);

    vec3 finalColor;

    for (int i = 0; i < nbrSamplesPerIteration; i++) {
        Ray ray = camera_createRayAt(camera, pos_Pixel);
        finalColor += getRayColor(ray);
    }
    finalColor /= float(nbrSamplesPerIteration);


    vec3 old    = texture(textureTarget, vec2(1,-1)*(pos_Pixel.xy)/2+0.5).rgb;
    vec3 result = mix(finalColor, old, float(renderIteration) / (renderIteration + 1));
    FragColor   = vec4(result, 1);
}

