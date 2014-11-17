#add_library(${CONFIGURE} tool.cpp SerialFiles.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/${GRAPH}/ DESTINATION ${PROJECT_BINARY_DIR}/${GRAPH}/)
