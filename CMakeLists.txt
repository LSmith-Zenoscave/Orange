cmake_minimum_required(VERSION 2.8)

project(ORANGE LANGUAGES C ASM)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_COMPILER i686-elf-gcc)
set (CMAKE_ASM_COMPILER i686-elf-gcc)

set (ORANGE_MAJOR_VERSION 0)
set (ORANGE_MINOR_VERSION 0)
set (ORANGE_PATCH_VERSION 1)
set (ORANGE_VERSION ${ORANGE_MAJOR_VERSION}.${ORANGE_MINOR_VERSION}.${ORANGE_PATCH_VERSION})

set (CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH} ${CMAKE_CURRENT_LIST_DIR}/include/kernel} ${CMAKE_CURRENT_LIST_DIR}/include/libc}")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_LIST_DIR}/include/kernel -I${CMAKE_CURRENT_LIST_DIR}/include/libc -ffreestanding -fpic -fno-common -nostdlib -lgcc -std=c11")
execute_process(COMMAND i686-elf-gcc "--print-file-name=crtbegin.o" OUTPUT_VARIABLE CRT_ROOT)
get_filename_component(CRT_ROOT "${CRT_ROOT}" DIRECTORY)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -B${CRT_ROOT}")
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set (libs "")
set (kernel_sources "")
set (libc_sources "")

include(${CMAKE_CURRENT_LIST_DIR}/src/libc/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/kernel/CMakeLists.txt)

list (REMOVE_ITEM kernel_sources "src/kernel/boot/crti.S")
list (REMOVE_ITEM kernel_sources "src/kernel/boot/crtn.S")

set (crti_source src/kernel/boot/crti.S)
set (crtn_source src/kernel/boot/crtn.S)

set (ORANGE_sources ${crti_source} ${kernel_sources} ${libc_sources} ${crtn_source})

add_executable (orange ${ORANGE_sources})
set_target_properties (orange PROPERTIES LINK_FLAGS "-Wl,--build-id=none -T ${CMAKE_CURRENT_LIST_DIR}/src/kernel/boot/linker.ld")
target_include_directories (orange SYSTEM PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include/kernel} ${CMAKE_CURRENT_LIST_DIR}/include/libc}")

file(GLOB_RECURSE lib_includes "include")
install(DIRECTORY include/libc/ DESTINATION sysroot/usr/include FILES_MATCHING PATTERN "*.h")


