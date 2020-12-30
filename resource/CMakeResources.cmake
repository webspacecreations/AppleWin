function(add_resources out_var id)
  set(result)

  string(CONCAT content_h
    "#include <map>\n"
    "#include <string>\n"
    "\n"
    "namespace ${id}\n"
    "{\n"
    "  extern const std::map<std::string, std::pair<const char *, const char *>> resources\;\n"
    "}\n")

  string(CONCAT content_cpp_top
    "#include \"${id}_resources.h\"\n")

  string(CONCAT content_cpp_private
    "extern \"C\"\n"
    "{\n"
    "\n")

  string(CONCAT content_cpp_public
    "namespace ${id}\n"
    "{\n"
    "\n"
    "  const std::map<std::string, std::pair<const char *, const char *>> resources = {\n")

  foreach(in_f ${ARGN})
    set(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    add_custom_command(OUTPUT ${out_f}
      COMMAND ld -r -b binary -o ${out_f} ${in_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Adding resource: ${in_f} -> ${out_f}"
      VERBATIM
      )

    string(REGEX REPLACE "[ ./]" "_" safe_in_f ${in_f})
    set(symbol "_binary_${safe_in_f}")

    string(APPEND content_cpp_private
      "  // ${in_f}\n"
      "  extern const char ${symbol}_start\;\n"
      "  extern const char ${symbol}_end\;\n"
      "\n")
    string(APPEND content_cpp_public
      "    {\"${in_f}\", {&${symbol}_start, &${symbol}_end}},\n")

    list(APPEND result ${out_f})
  endforeach()

  string(APPEND content_cpp_private
    "}\n")

  string(APPEND content_cpp_public
    "  }\;\n"
    "\n"
    "}\n")

  set(out_h "${CMAKE_CURRENT_BINARY_DIR}/${id}_resources.h")
  file(WRITE ${out_h} ${content_h})
  message("Generating header for '${id}': ${out_h}")
  list(APPEND result ${out_h})

  set(out_cpp "${CMAKE_CURRENT_BINARY_DIR}/${id}_resources.cpp")
  file(WRITE ${out_cpp}
    ${content_cpp_top}
    "\n"
    ${content_cpp_private}
    "\n"
    ${content_cpp_public})
  message("Generating cpp for '${id}': ${out_cpp}")
  list(APPEND result ${out_cpp})

  set(${out_var} "${result}" PARENT_SCOPE)
endfunction()
