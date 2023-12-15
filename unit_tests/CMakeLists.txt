set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set( cur_path ${CMAKE_CURRENT_SOURCE_DIR} )
cmake_path(GET cur_path PARENT_PATH parent_path)

function(add_ut_run scriptfilepath ut_exe_path)
  set(full_ut_exe_path "${ut_exe_path}\n")
  write_file(${scriptfilepath} ${full_ut_exe_path} APPEND)
endfunction()


function(add_ut project_name source_path)
  project(${project_name})
  add_executable(${project_name})
  target_compile_options(${project_name} PRIVATE -Wall -Wextra -Wfatal-errors -DLOG_FORMAT_VERIFICATION)
  target_link_libraries(${project_name} PUBLIC
      shmlib
  )

  target_sources(${project_name}
      PRIVATE
      ${source_path}
  )
endfunction()

function(add_ut_nongtest project_name source_path)
  project(${project_name})
  add_executable(${project_name})
  target_compile_options(${project_name} PRIVATE -Wall -Wextra -Wfatal-errors -DLOG_FORMAT_VERIFICATION)
  target_link_libraries(${project_name} PUBLIC
      shmlib
  )

  target_sources(${project_name}
      PRIVATE
      ${source_path}
  )
endfunction()

add_ut("SharedRingBufferTest" ${CMAKE_CURRENT_LIST_DIR}/shm/SharedRingBufferTest.cpp)
add_ut_run(${parent_path}/run_ut.sh ${parent_path}/runfiles/unit_tests/SharedRingBufferTest)

add_ut("TSharedRingBufferTest" ${CMAKE_CURRENT_LIST_DIR}/shm/TSharedRingBufferTest.cpp)
add_ut_run(${parent_path}/run_ut.sh ${parent_path}/runfiles/unit_tests/TSharedRingBufferTest)

add_ut("SharedBlockQueueTest" ${CMAKE_CURRENT_LIST_DIR}/shm/SharedBlockQueueTest.cpp)
add_ut_run(${parent_path}/run_ut.sh ${parent_path}/runfiles/unit_tests/SharedBlockQueueTest)

add_ut("SharedMemoryTest" ${CMAKE_CURRENT_LIST_DIR}/shm/SharedMemoryTest.cpp)
add_ut_run(${parent_path}/run_ut.sh ${parent_path}/runfiles/unit_tests/SharedMemoryTest)

add_ut("SharedSemaphoreTest" ${CMAKE_CURRENT_LIST_DIR}/shm/SharedSemaphoreTest.cpp)
add_ut_run(${parent_path}/run_ut.sh ${parent_path}/runfiles/unit_tests/SharedSemaphoreTest)