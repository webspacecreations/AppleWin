cmake_policy(SET CMP0072 NEW)

set(IMGUI_PATH NONE CACHE PATH "path to imgui")

# libglew-dev

if (EXISTS ${IMGUI_PATH}/imgui.h)
  message("Using IMGUI_PATH=${IMGUI_PATH}")

  include(FindPkgConfig)

  find_package(SDL2 REQUIRED)
  pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
  pkg_search_module(GLES2 REQUIRED glesv2)

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
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
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
    ${GLES2_LIBRARIES}
    appleii
    common2
    )
  target_compile_definitions(ia2 PRIVATE IMGUI_IMPL_OPENGL_ES3)

  install(TARGETS ia2
    DESTINATION bin)

else()
  message("Bad IMGUI_PATH=${IMGUI_PATH}, skipping imgui code")
endif()
