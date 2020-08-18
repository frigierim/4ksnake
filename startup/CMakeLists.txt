file(GLOB SRC_FILES *.cpp)

add_library(startup STATIC "")
target_sources(startup PUBLIC ${SRC_FILES})
target_include_directories(startup PUBLIC ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)
