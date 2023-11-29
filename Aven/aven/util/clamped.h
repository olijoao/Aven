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

		T getValue() const {
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

		clamped <T, MIN, MAX>& operator+=(T const& value) {
			if (this->value > 0 && value > INT_MAX - this->value)	//overflow
				setValue(MAX);
			else if (this->value < 0 && value < INT_MIN - this->value) //underflow
				setValue(MIN);
			else
				setValue(this->value + value);		
			return *this;
		}


	private:
		T value;
	};

}