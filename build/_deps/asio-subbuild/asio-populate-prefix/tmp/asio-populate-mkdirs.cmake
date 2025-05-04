# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-src")
  file(MAKE_DIRECTORY "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-src")
endif()
file(MAKE_DIRECTORY
  "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-build"
  "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix"
  "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix/tmp"
  "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp"
  "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix/src"
  "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/affanfaisal/cs222FNF/team-109-project/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
