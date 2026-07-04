add_library(husk_internal SHARED
    dllmain.cpp
)

target_include_directories(husk_internal PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

target_link_libraries(husk_internal PRIVATE imgui)

set_target_properties(husk_internal PROPERTIES
    OUTPUT_NAME "husk_internal"
    PREFIX ""   # no "lib" prefix on the .dll
)
