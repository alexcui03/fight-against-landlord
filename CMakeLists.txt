cmake_minimum_required(VERSION 3.15)

project(fal LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)

# set SHOW_ALL_CARDS to show other players' cards
#add_definitions(-DSHOW_ALL_CARDS)

set(PROJECT_SRC
    src/card.cpp
    src/cardset.cpp
    src/fal.cpp
    src/player/player_base.cpp
    src/player/player_normal.cpp
    #src/player/player_computer.cpp
    src/player/player_computer_2.cpp
)

set(PROJECT_INC
    src/card.h
    src/cardset.h
    src/fal.h
    src/ui.h
    src/player/player_base.h
    src/player/player_normal.h
    #src/player/player_computer.h
    src/player/player_computer_2.h
)

#add_compile_options(/utf-8)

add_executable(fal ${PROJECT_SRC})
target_include_directories(fal PRIVATE ${PROJECT_INC})

#add_executable(test-cardset
#    ${CMAKE_CURRENT_SOURCE_DIR}/test/cardset.test.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/src/cardset.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/src/card.cpp
#)
