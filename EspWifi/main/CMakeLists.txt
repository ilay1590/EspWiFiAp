file(GLOB_RECURSE SOURCES "softap_example_main.cpp" "Source/*.cpp" "Source/*.c")
file(GLOB_RECURSE HEADERS "Include/*.h" "Include/*.hpp")

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "Include")