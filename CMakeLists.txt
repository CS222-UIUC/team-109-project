cmake_minimum_required(VERSION 3.15) 
project(BlackjackGameServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include(FetchContent)

# nlohmann/json (header only)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Crow C++ 
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-28-1 
  SOURCE_SUBDIR asio/include
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(crow)

add_executable(game_server 
    game_server.cc 
    BlackJack.cc 
    Deck.cc
)

# link dependencies
target_link_libraries(game_server PRIVATE 
    crow 
    # nlohmann_json::nlohmann_json #
)

target_include_directories(game_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${json_SOURCE_DIR}/include  
    ${asio_SOURCE_DIR}        
    ${crow_SOURCE_DIR}/include 
)