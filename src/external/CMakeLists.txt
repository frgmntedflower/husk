add_executable(husk_external
    main.cpp
    memory/mem.cpp
    dx9/windowing.cpp
)


target_include_directories(husk_external PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

target_link_libraries(husk_external PRIVATE
    imgui
    psapi   # GetModuleInformation
    dwmapi
)

set_target_properties(husk_external PROPERTIES
    OUTPUT_NAME "husk_external"
    WIN32_EXECUTABLE FALSE  # keep console window for now
)
