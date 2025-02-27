target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE # EntityManager.cpp
          EntityManagerImpl.cpp Entity.cpp SceneManager.cpp Scene.cpp
          Figure.cpp)
