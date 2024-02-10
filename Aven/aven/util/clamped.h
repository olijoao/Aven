#pragma once

#include <aven/util/math.h>

namespace aven {
	

	template <typename T, T MIN, T MAX>
	class clamped {
	public:
		constexpr T getMin() const { return MIN; }
		constexpr T getMax() const { return MAX; }


		clamped(T v = MIN){
			value = clamp(v, MIN, MAX);
		}

		// cast operator
		operator T const() const{ 
			return getValue(); 
		}

		T const& getValue() const {
			return value;
		}

		void setValue(T value) {
			value = clamp(value, MIN, MAX);
			this->value = value;
		}

		clamped <T, MIN, MAX>& operator=(T const& value) {
			setValue(value);	
			return *this;
		}


	private:
		T value;
	};


	template<int MIN, int MAX>	
	using c_int = clamped<int, MIN, MAX>;

	template<float MIN, float MAX>	
	using c_float = clamped<float, MIN, MAX>;

	template<ivec3 MIN, ivec3 MAX>	
	using c_ivec3 = clamped<ivec3, MIN, MAX>;

	template<vec3 MIN, vec3 MAX>	
	using c_vec3 = clamped<vec3, MIN, MAX>;
	
}
