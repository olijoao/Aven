

#include <aven/objects/Scene.h>
#include <aven/objects/Volume.h>
#include <fstream>
  
namespace aven {

	Scene::Scene(clamped<ivec3, 1, 256> size)
		: volume(std::make_shared<Volume const>(Volume(size.getValue(), {0,0,0}, 1, 1, 1, true, false)))
	{
		//..	
	}


	Scene::Scene(Volume&& vol):volume(std::make_shared<Volume>(vol)) {
		//..	
	}


	void Scene::serialize(std::ofstream& out, Scene const& scene) {
		Volume::serialize(out, *scene.volume);
	}


	Scene Scene::deserialize(std::ifstream& in ) {	
		Scene scene(Volume::deserialize(in));
		return scene;
	}
	
}




