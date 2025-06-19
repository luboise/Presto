target_sources(
  ${PRESTO_ENGINE_NAME}
          MaterialInstanceAsset.cpp
          # Asset Sources
          AssetSources/MeshSource.cpp)

internal_modules(image.cpp)
