
set(target login)
project(${target})

add_subdirectory(client)

gen_server(${target} "")

target_link_libraries(${target}
        ${HIREDIS_LIB}
        ${REDIS_PLUS_PLUS_LIB}
        )