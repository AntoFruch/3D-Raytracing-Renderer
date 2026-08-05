add_executable(main
    main.cpp
)

target_link_libraries(main PUBLIC lib_core sfml-graphics)

add_custom_target(copy-res ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_CURRENT_BINARY_DIR}/res
        COMMENT "Mise à jour des ressources..."
)

add_dependencies(main copy-res)

