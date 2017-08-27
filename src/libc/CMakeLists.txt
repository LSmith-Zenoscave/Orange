file (GLOB_RECURSE module_sources ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.c)
file (GLOB_RECURSE module_assembly ${CMAKE_CURRENT_LIST_DIR}/*.S)
set (libc_sources ${libc_sources} ${module_sources} ${modules_assembly})
