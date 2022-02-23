set(target ${LIBCOMMON})
project(${target})

message("PROJECT_SOURCE_DIR:        ${PROJECT_SOURCE_DIR}")
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_FILES)
aux_source_directory(database SRC_FILES)
aux_source_directory(log SRC_FILES)

add_library(${target} ${SRC_FILES})