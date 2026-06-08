include_guard(GLOBAL)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# generate_di_context(<target>
#     [SRC_DIR <path>]              # directory to scan; default: ${CMAKE_CURRENT_SOURCE_DIR}/src
#     [OUT_DIR <path>]              # output directory;   default: ${CMAKE_CURRENT_BINARY_DIR}/generated
#     [GEN_GETTER_NAME   <class>]   # Getter template class, e.g. "di::Getter"
#     [GEN_GETTER_INC    <header>]  # Getter include path,   e.g. "di/Getter.h"
#     [GEN_CONTEXT_CLASS_NAME <class>]  # generated context class, e.g. "di::Context"
# )
#
# Scans all *.h files under SRC_DIR for annotations of the form:
#   /// BEAN <name> [dep_type1 dep_type2 ...]
# and generates a DI context class (header + source) under OUT_DIR.
# The generated files are added to TARGET's sources, and OUT_DIR is added
# to the target's include directories so the header is reachable as
# "<namespace/ClassName>.h".
function(generate_di_context TARGET)
    cmake_parse_arguments(ARG ""
        "GEN_GETTER_NAME;GEN_GETTER_INC;GEN_CONTEXT_CLASS_NAME;SRC_DIR;OUT_DIR"
        "" ${ARGN})

    if(NOT ARG_SRC_DIR)
        set(ARG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
    endif()
    if(NOT ARG_OUT_DIR)
        set(ARG_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
    endif()
    if(NOT ARG_GEN_GETTER_NAME)
        set(ARG_GEN_GETTER_NAME "di::Getter")
    endif()
    if(NOT ARG_GEN_GETTER_INC)
        set(ARG_GEN_GETTER_INC "di/Getter.h")
    endif()
    if(NOT ARG_GEN_CONTEXT_CLASS_NAME)
        set(ARG_GEN_CONTEXT_CLASS_NAME "di::Context")
    endif()

    # Derive output file paths from the context class name (e.g. "di::Context" → "di/Context")
    string(REPLACE "::" "/" _CTX_REL "${ARG_GEN_CONTEXT_CLASS_NAME}")
    set(_GEN_H   "${ARG_OUT_DIR}/${_CTX_REL}.h")
    set(_GEN_CPP "${ARG_OUT_DIR}/${_CTX_REL}.cpp")

    # Track all scanned headers so the command reruns when any of them change.
    # CONFIGURE_DEPENDS causes CMake to re-glob (and reconfigure) when files
    # are added or removed.
    file(GLOB_RECURSE _SCAN_HEADERS CONFIGURE_DEPENDS "${ARG_SRC_DIR}/*.h")

    set(_GENERATOR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/di_context_generator.py")

    add_custom_command(
        OUTPUT  "${_GEN_H}" "${_GEN_CPP}"
        COMMAND "${Python3_EXECUTABLE}"
                "${_GENERATOR}"
                --src-dir        "${ARG_SRC_DIR}"
                --out-dir        "${ARG_OUT_DIR}"
                --getter-class   "${ARG_GEN_GETTER_NAME}"
                --getter-include "${ARG_GEN_GETTER_INC}"
                --context-class  "${ARG_GEN_CONTEXT_CLASS_NAME}"
        DEPENDS ${_SCAN_HEADERS} "${_GENERATOR}"
        COMMENT "Generating DI context ${ARG_GEN_CONTEXT_CLASS_NAME}…"
        VERBATIM
    )

    target_sources(${TARGET} PRIVATE "${_GEN_H}" "${_GEN_CPP}")
    target_include_directories(${TARGET} PRIVATE "${ARG_OUT_DIR}")
endfunction()
