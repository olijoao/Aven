

#include <aven/objects/Scene.h>
#include <aven/objects/Volume.h>


namespace aven {

	Scene::Scene(clamped<ivec3, 1, 256> size)
		: volume(std::make_shared<Volume const>(Volume(size.getValue(), {0,0,0}, 1, 1, 1)))
	{
		
	}
	
}