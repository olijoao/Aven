

#include <aven/scene/Scene.h>
#include <aven/scene/Volume.h>


namespace aven {

	Scene::Scene(clamped<ivec3, 1, 256> size)
		: volume(std::make_shared<Volume const>(Volume(size.getValue(), {0,0,0}, 1, 1, 1)))
	{
		
	}

	void Scene::serialize(std::ofstream& out) {
		static const char SCENE_ELEMENT_TYPE_VOLUME = 0;

		//volume
		if (volume) {
			out.write(&SCENE_ELEMENT_TYPE_VOLUME, 4);
		}
	}

	
}