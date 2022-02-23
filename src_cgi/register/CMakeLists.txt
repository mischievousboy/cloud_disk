add_subdirectory(client)

set(target register)
project(${target})

gen_server(${target} "")
