#pragma once

#include <aven/GL/Program.h>
#include <aven/gui/ImGui_Util.h>
#include <memory>
#include <string>



namespace aven {
	class Property {
	public:
		template <typename T>
		Property(std::string displayName, std::string internalName, T x)
			:self(std::make_unique<model_t<T>>(displayName, internalName, std::move(x))) 
		{
			//..
		}

		void updateUniform(gl::Program const& program) const{
			self->updateUniform(program);
		}

		friend bool display_imgui(Property& p) {
			return p.self->display_imgui();
		}

	private:
	
		struct concept_t {
			concept_t(std::string displayName, std::string internalName):
				displayName(displayName), internalName(internalName)
			{
				/*..*/ 
			}

			virtual void updateUniform(gl::Program const&) const = 0;
			virtual bool display_imgui() = 0;
			std::string const internalName;
			std::string const displayName;
		};


		template<typename T>
		struct model_t : concept_t {
			//ctor
			model_t(std::string displayName, std::string internalName, T x) final
				:concept_t(displayName, internalName), data(std::move(x)) 
			{
				//...
			}

			void updateUniform(gl::Program const& program) const final {
				program.setUniform(internalName, data);
			}

			bool display_imgui() final{
				return ::aven::display_imgui(displayName, data);
			}

			T data;

		};

		std::unique_ptr<concept_t> self;

	};
}