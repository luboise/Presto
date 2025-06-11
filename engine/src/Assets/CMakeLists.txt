target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE Asset.cpp
          AssetPath.cpp
          AssetSources.cpp
          # MeshAsset.cpp
          ModelAsset.cpp
          MaterialAsset.cpp
          MaterialInstanceAsset.cpp
          ImageAsset.cpp
          FontAsset.cpp)
