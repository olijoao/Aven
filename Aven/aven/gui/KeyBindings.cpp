
#include <3rdParty/imgui/imgui.h>:w
#include <aven/gui/KeyBindings.h>
#include <unordered_map>
#include <vector>

namespace aven {
	namespace keybindings {
			
		std::vector<KeycharAndModifier> inputqeue_char;
		std::unordered_map<KeycharAndModifier, std::function<void()>, KeyHasher> keyStrokes;	//<modifier, character>

		
		void pushChar(unsigned int c) {
			inputqeue_char.push_back({ c });
		}

		void addKeyStroke(KeycharAndModifier key, std::function<void()> f) {
			keyStrokes.insert({ key, f });
		}

		void handleKeyEvents() {
			if (ImGui::GetIO().WantCaptureKeyboard) {
				inputqeue_char.clear();
				return;
			}

			while (!inputqeue_char.empty()) {
				auto c = inputqeue_char.back();
				inputqeue_char.pop_back();

				auto action = keyStrokes.find(c);
				if (action != keyStrokes.end())
					action->second();
			}
		}

	}
}