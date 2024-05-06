#pragma once

#include <acore/math.h>
#include <optional>

namespace aven {

	class Ray {
	public:
		Ray(vec3 pos, vec3 dir) 
			:pos(pos), dir(dir) 
		{
			assert(dir.lengthSquared() > 0);
		}

		vec3 pos, dir;
	};


	template <typename T>
	class AABB {
	public:
		AABB(Vector3<T> min, Vector3<T> max)
			: min(min), max(max) 
		{
			assert(all(lessThanEqual(min, max)));
		}

		Vector3<T> getSize() const{
			return max - min;
		}

		bool isEmpty() const {
			return any(equal(min, max));
		}

		auto getMin() const {return min;}
		auto getMax() const {return max;}

	private:
		Vector3<T> min, max;
	};


	auto intersect(auto const& a, auto const& b) {
		return intersect(b,a);
	}


	inline std::optional<vec2> intersect(Ray const& r, AABB<float> const& aabb) {
		vec3 tMin = (aabb.getMin() - r.pos) / r.dir;
		vec3 tMax = (aabb.getMax() - r.pos) / r.dir;
		vec3 t1 = min(tMin, tMax);
		vec3 t2 = max(tMin, tMax);
		vec2 t = vec2(std::max(std::max(t1.x, t1.y), t1.z), std::min(std::min(t2.x, t2.y), t2.z));
		if ((t.x < t.y) && t.x > 0)
			return t;
		return {};		
	}


	template <typename T>
	inline std::optional<AABB<T>> intersect(AABB<T> const& aabb1, AABB<T> const& aabb2) {
	
		auto result_min = max(aabb1.getMin(), aabb2.getMin());
		auto result_max = min(aabb1.getMax(), aabb2.getMax());
		
		if (any(lessThanEqual(result_max, result_min)))
			return {};
		return AABB<T>(result_min, result_max);
	}
}