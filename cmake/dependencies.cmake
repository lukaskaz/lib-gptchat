cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

set(source_dir "${CMAKE_BINARY_DIR}/libnlohmann-src")
set(build_dir "${CMAKE_BINARY_DIR}/libnlohmann-build")

EXTERNALPROJECT_ADD(
  libnlohmann
  GIT_REPOSITORY    https://github.com/nlohmann/json.git
  GIT_TAG           v3.11.3
  PATCH_COMMAND     ""
  PREFIX            libnlohmann-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

include_directories(${source_dir}/include)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_BINARY_DIR}/liboai-src")
set(build_dir "${CMAKE_BINARY_DIR}/liboai-build")

file(GLOB patches "${CMAKE_SOURCE_DIR}/patches/*.patch")
foreach(patch ${patches})
  list(APPEND patching_cmd git am -s --keep-cr < ${patch} &&)
endforeach()
list(APPEND patching_cmd echo "End of patches")

EXTERNALPROJECT_ADD(
  liboai
  GIT_REPOSITORY    https://github.com/D7EAD/liboai.git
  GIT_TAG           v4.0.1
  PATCH_COMMAND     ${patching_cmd}
  PREFIX            liboai-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir}/liboai && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/liboai/include)
link_directories(${build_dir}/build)
