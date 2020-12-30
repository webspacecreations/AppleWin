include(CMakeResources.cmake)

add_resources(rom_sources roms
  Apple2e_Enhanced.rom
  Apple2e.rom
  )

add_library(apple2roms STATIC
  ${rom_sources}
  )
