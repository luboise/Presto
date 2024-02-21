target_sources(Presto
PUBLIC	EntityManager.h
		Entity/Entity.h
		Component/Component.h
		System/System.h
		_EntityHeader.h

PRIVATE	EntityManager.cpp
		Entity/Entity.cpp
)