set(target transfile)

add_subdirectory(client)

project(${target})

gen_server(${target} "")