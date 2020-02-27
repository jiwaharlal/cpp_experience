set(CONANFILE_TEMPLATE_PATH "${CMAKE_CURRENT_LIST_DIR}/conanfile.py.in")

function(add_conan_recipe ARG_NAME)
    message(STATUS "current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
    message(STATUS "current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
    message(STATUS "creating conan recype for target: ${ARG_NAME}")

    set(MICROCORE_NAME ${ARG_NAME})
    string(REPLACE "." "" MICROCORE_CONAN_CLASS ${MICROCORE_NAME})
    string(APPEND MICROCORE_CONAN_CLASS "Conan")
    set(MICROCORE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    set(MICROCORE_BIN_PATH ${CMAKE_CURRENT_BINARY_DIR})

    message(STATUS "current list dir: ${CMAKE_CURRENT_LIST_DIR}")
    message(STATUS "conanfile template path: ${CONANFILE_TEMPLATE_PATH}")

    configure_file(
        ${CONANFILE_TEMPLATE_PATH}
        "${CMAKE_CURRENT_BINARY_DIR}/conanfile.py"
        )

    add_custom_target(${MICROCORE_NAME}.Conan
        conan create "${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS ${MICROCORE_NAME}
        COMMENT "Creating conan package for ${MICROCORE_NAME}"
        )


        #configure_file(
            #"${PROJECT_SOURCE_DIR}/cmake/_Link.h.in"
            #"${MATSDK_INSTALL_INCLUDE_DIR}/${NAME}/_Link.h")
endfunction()
