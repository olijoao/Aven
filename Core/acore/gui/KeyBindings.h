#pragma once

 #include <functional>

namespace aven{
	namespace keybindings {
		struct KeycharAndModifier {
			const unsigned int CTRL_BIT = 1;
			const unsigned int SHIFT_BIT = 2;
			const unsigned int ALT_BIT = 4;
			KeycharAndModifier(unsigned int character, bool mod_ctrl = false, bool mod_shift = false, bool mod_alt = false)
				:character(character),
				mods( (mod_ctrl		? CTRL_BIT	: 0)
					| (mod_shift	? SHIFT_BIT : 0)
					| (mod_alt		? ALT_BIT	: 0))
			{

			}

			bool operator==(KeycharAndModifier const&) const = default;

			unsigned int character;
			unsigned int mods;
		};

		struct KeyHasher {
			std::size_t operator()(KeycharAndModifier const& character) const {
				return character.character ^ character.mods;
			}
		};

		void pushChar(unsigned int c);
		void addKeyStroke(KeycharAndModifier key, std::function<void()> f);
		void handleKeyEvents();

	}

}
