file (GLOB_RECURSE module_sources ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.c)
file (GLOB_RECURSE module_assembly ${CMAKE_CURRENT_LIST_DIR}/*.S)
set (kernel_sources ${kernel_sources} ${module_sources} ${module_assembly})

