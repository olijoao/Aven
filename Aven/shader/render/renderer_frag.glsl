#version 460 core

#pragma include "shader/lib/blend.glsl"
#pragma include "shader/lib/camera.glsl"
#pragma include "shader/lib/geometry.glsl"
#pragma include "shader/lib/math.glsl"
#pragma include "shader/lib/sample.glsl"
#pragma include "shader/lib/shade.glsl"


const int VOLUME_TYPE_DEFAULT           = 0;
const int VOLUME_TYPE_BLENDED_NORMAL    = 1;
const int VOLUME_TYPE_BLENDED_ERASE     = 2;


in vec2 pos_Pixel;
out vec4 FragColor;

//uniforms
uniform Camera      camera;
uniform vec2        rand_seed;
uniform sampler2D	textureTarget;
uniform uint        renderIteration;

// volume
uniform int         volume_type;
uniform vec3        volume_pos;
uniform float       volume_sigma_t;
uniform float       volume_density;
uniform float       volume_opacity;     //used with VOLUME_TYPE_BLENDED
uniform vec3        volume_color;       //used with VOLUME_TYPE_BLENDED
uniform int         volune_isDisplayingBoundingBox;
uniform int         volume_renderModeHybrid;
uniform float       volume_stepSize;
uniform ivec3       volumeSize;
uniform vec3        invVolumeSize;
layout(binding = 1) uniform sampler3D   volume;
layout(binding = 2) uniform sampler3D   volume_bottom;



vec4 getVoxelAt(vec3 pos) {
    switch (volume_type) {
    case VOLUME_TYPE_DEFAULT: {
        return texture(volume, pos);
    }
    case VOLUME_TYPE_BLENDED_NORMAL:{
        vec4 top    = vec4(volume_color, texture(volume, pos).r);
        vec4 bottom = texture(volume_bottom, pos);
        top.a       *= volume_opacity;
        return blend_over(top, bottom);
    }
    case VOLUME_TYPE_BLENDED_ERASE: {
        float top       = texture(volume, pos).r;
        vec4 bottom     = texture(volume_bottom, pos);
        bottom.a        = bottom.a - volume_opacity * top;
        return bottom;
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


vec3 worldToVolume(vec3 posWorld, vec3 posVolume, ivec3 volumeSize, vec3 scale) {
    return (posWorld-posVolume) / (volumeSize*scale) + vec3(0.5f, 0.5f, 0.5f);
}



Hit intersectWorld(Ray ray, int iteration) {
    //sky
    float t = 0.5 * (-ray.dir.y + 1.0);
    vec3 skycolor = mix(camera.color_background_sky, camera.color_background_ground, t);
    
    Hit nearestHit = create_hit_sky(skycolor);

    //AABB
    vec3 scale = vec3(1.0f);
    vec3 aabbSize = vec3(volumeSize) * scale;
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
            vec3 posLocal = worldToVolume(samplePos, volume_pos, volumeSize, scale);
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





vec3 getRayColor(Ray ray) {
    vec3 ray_color = vec3(1, 1, 1);

    const int maxIterations = 2+camera.nbrBounces;
    int iteration = 0;
    while (iteration < maxIterations) {
        
        Hit hit = intersectWorld(ray, iteration);

        Ray newRay;
        vec3 attenuation;
        bool scattered = material_scatter(hit, ray, attenuation, newRay);
        ray = newRay;
        ray_color *= attenuation;

        if (!scattered)
            return ray_color;
        
        iteration++;
    }
    return vec3(0, 0, 0);
}






void main() {
    srand(pos_Pixel.x  * rand_seed.x + pos_Pixel.y * rand_seed.y);

    vec3 finalColor;

    for (int i = 0; i < camera.nbrSamplesPerIteration; i++) {
        Ray ray         = camera_createRayAt(camera, pos_Pixel);
        finalColor += getRayColor(ray);
    }
    finalColor /= float(camera.nbrSamplesPerIteration);


    vec3 old    = texture(textureTarget, vec2(1,-1)*(pos_Pixel.xy)/2+0.5).rgb;
    vec3 result = mix(finalColor, old, float(renderIteration) / (renderIteration + 1));
    FragColor   = vec4(result, 1);
}

