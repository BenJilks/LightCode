CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(lightcode)

set(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(RapidJSON REQUIRED)
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-3.0)

INCLUDE_DIRECTORIES(${RapidJSON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTKMM_CFLAGS_OTHER})

INCLUDE_DIRECTORIES(include/)
file(GLOB SRC
    "source/*.cpp"
    "source/content/*.cpp"
    "source/settings/*.cpp"
    "source/filemanager/*.cpp"
    "source/textedit/*.cpp"
    "source/textedit/language/*.cpp"
)

ADD_EXECUTABLE(lightcode ${SRC})
TARGET_LINK_LIBRARIES(lightcode ${GTKMM_LIBRARIES})
TARGET_LINK_LIBRARIES(lightcode ${RapidJSON_LIBRARIES})
