cmake_minimum_required(VERSION 3.16)

project(rts_ntc_unit_tests C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(unity)

foreach(position IN ITEMS lower upper)
    if(position STREQUAL "upper")
        set(ntc_at_upper 1)
    else()
        set(ntc_at_upper 0)
    endif()

    add_executable(rts_ntc_${position}_test
        test.c
        ../rts_ntc.c
    )
    target_include_directories(rts_ntc_${position}_test PRIVATE ..)
    target_compile_definitions(rts_ntc_${position}_test PRIVATE
        CONFIG_RTS_NTC_ENABLE=1
        CONFIG_RTS_NTC_IS_NTC_AT_UPPER=${ntc_at_upper}
    )
    target_link_libraries(rts_ntc_${position}_test PRIVATE unity)

    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(rts_ntc_${position}_test PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()

    add_test(NAME rts_ntc_${position} COMMAND rts_ntc_${position}_test)
endforeach()
