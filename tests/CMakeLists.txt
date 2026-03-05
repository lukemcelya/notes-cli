add_executable(notes_tests
        test_main.cpp
        json_store_tests.cpp
)

target_include_directories(notes_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(notes_tests PRIVATE notes_core)

add_test(NAME notes_tests COMMAND notes_tests)
