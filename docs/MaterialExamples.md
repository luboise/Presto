## Overriding default materials

```cpp
[MeshAsset1, MeshAsset2] = AssetManager.loadMesh("assets/my_mesh.gltf");

/*
   No new definition is created, only a new instance for all unique instances that the meshes use.
   */


auto definition = AssetManager.getMaterialDefinition(PR_DEFAULT_PIPELINE_3D /*3D Pipeline*/);
auto instance = definition.createInstance("My EPIC material");

MeshAsset1.setDefaultMaterial(instance);
```


## Creating new materials at runtime

```cpp
MeshAsset& block_mesh{AssetManager::loadMesh("assets/models/block.gltf")};

const MaterialDefinition& def{AssetManager::getMaterialDefinition(PR_DEFAULT_PIPELINE_3D)};


MaterialInstance floor_material{def.instantiate("MY EPIC MATERIAL")};

ImageAsset& floor_image{AssetManager::loadImage("assets/images/floor.png")};

// Could also just use setImage("diffuseTexture")
floor_material.setImage(PBRProperty::DIFFUSE, floor_image); 

// Could also just use setProperty("metallic")
floor_material.setProperty(PBRProperty::METALLIC, 0.8);

// 2 options

// Option 1: Change the default material of the mesh (all references to the mesh will use this material by default)

ModelComponent model{};

block_mesh.setDefaultMaterial(floor_material);

// Model will use the default material if an override isn't specified'
model.addMesh(block_mesh);

entity.setComponent<ModelComponent>(model)


// Option 2: Use a material override

ModelComponent model{};

model.addMesh(block_mesh, floor_material);

entity.setComponent<ModelComponent>(model)



// Option 2.5: Setting an override later

mesh = model.findMesh("Block")
model.setOverride(mesh, floor_material)




Image image = AssetManager.loadImage("assets/images/THEFLOOR.png");

floorMaterial.setImage("diffuseTexture", );



```






auto defaultMaterial = MeshAsset1.getDefaultMaterial();
auto pipelineId = defaultMaterial.getPipelineId();




MeshAsset1 = {
	uint32_t mesh_id = 1;
	MaterialInstance default_material;

};

default_material = {
	type: PBR;
	MaterialProperty[] = [
		{
			type: TEXTURE,
			value: textureId,
		},
		{
			type: VEC3,
			value: {0, 1, 2}
		},

	];
	Buffer buffer;
	
	setProperty(); => buffer.write(property);
}
