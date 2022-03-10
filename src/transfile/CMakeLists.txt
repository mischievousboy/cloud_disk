set(target transfile)

add_subdirectory(client)

project(${target})

gen_server(${target} "")

target_link_libraries(${target}
        ${HIREDIS_LIB}
        ${REDIS_PLUS_PLUS_LIB}
        )
