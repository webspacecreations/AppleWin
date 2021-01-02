cmake_policy(SET CMP0072 NEW)

set(IMGUI_PATH NONE CACHE PATH "path to imgui")

if (EXISTS ${IMGUI_PATH}/imgui.h)
  message("Using IMGUI_PATH=${IMGUI_PATH}")

  include(FindPkgConfig)
  include(FindOpenGL)

  find_package(SDL2 REQUIRED)
  pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)

  add_executable(ia2
    main.cpp
    image.cpp
    emulator.cpp
    ../sdl/utils.cpp
    ../sdl/sdirectsound.cpp
    ../sdl/bitmaps.cpp
    ../sdl/gamepad.cpp
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/backends/imgui_impl_sdl.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl2.cpp
    )

  target_include_directories(ia2 PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
    )

  target_link_libraries(ia2 PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    appleii
    common2
    OpenGL::OpenGL
    )

  install(TARGETS ia2
    DESTINATION bin)

else()
  message("Bad IMGUI_PATH=${IMGUI_PATH}, skipping imgui code")
endif()
