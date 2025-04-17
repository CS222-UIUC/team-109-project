cmake_minimum_required(VERSION 3.15)
project(BlackjackGameServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# nlohmann/json (header only)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Asio (header only)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-28-1
)
FetchContent_MakeAvailable(asio)

# Set ASIO_INCLUDE_DIR to satisfy Crow's Findasio.cmake
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE INTERNAL "")

# Crow
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

# Link Crow (brings in nlohmann/json too)
target_link_libraries(game_server PRIVATE
  crow
)

# Include manually what Crow doesn't (Asio + optional JSON)
target_include_directories(game_server PRIVATE
  ${ASIO_INCLUDE_DIR}
  ${json_SOURCE_DIR}/include
)

find_package(Boost REQUIRED)
target_include_directories(game_server PRIVATE ${Boost_INCLUDE_DIRS})
