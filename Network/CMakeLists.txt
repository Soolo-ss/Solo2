aux_source_directory(. DIR_NETWORK)

set(SOURCE_FILES ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/Connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Network.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionManager.cpp
        PARENT_SCOPE)

