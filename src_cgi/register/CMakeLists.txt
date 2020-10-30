
set(TargetCGI register)



#
list(APPEND DIR_SRCS main.cpp)


#message(STATUS CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
#message(STATUS CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR})
#message(STATUS DIR_SRCS=${DIR_SRCS})


add_executable(${TargetCGI} 
${DIR_SRCS}
 "register.h" "register.cpp")

target_link_libraries(${TargetCGI}
fdfsclient	
fastcommon
hiredis	
fcgi
m      
${MYSQL_NAME}
${LOG4CPLUS_NAME}
)


install(TARGETS ${TargetCGI}
    	RUNTIME DESTINATION ${DST_DIR})