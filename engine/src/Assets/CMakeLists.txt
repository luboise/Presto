target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE Asset.cpp
          AssetPath.cpp
          # MeshAsset.cpp
          ModelAsset.cpp
          MaterialAsset.cpp
          MaterialInstanceAsset.cpp
          ImageAsset.cpp
          FontAsset.cpp
          # Asset Sources
          AssetSources/MeshSource.cpp
          AssetSources/FontSource.cpp)
