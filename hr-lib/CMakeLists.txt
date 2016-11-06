
## Genertate code
# dump all vni files into one
#list(APPEND VNL_INTERFACE_DIRS "lib/virtual-network-layers-cpp/interface" "interface")
#foreach(VNL_INTERFACE_DIR ${VNL_INTERFACE_DIRS})
#  file(GLOB_RECURSE VNI_FILES "${VNL_INTERFACE_DIR}" "*.vni")
#  foreach(VNI_FILE ${VNI_FILES})
#    file(READ ${VNI_FILE} VNIDATA)
#    file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/vnifiles.dump" "${VNIDATA}")
#  endforeach(VNI_FILE ${VNI_FILES})
#endforeach(VNL_INTERFACE_DIR ${VNL_INTERFACE_DIRS})
## calculate SHA1 checksum for vni file dump
#file(SHA1 "${CMAKE_CURRENT_BINARY_DIR}/vnifiles.dump" NEWSHA1)
## read old SHA1 checksum
#if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/vnifiles.sha1")
#  file(READ "${CMAKE_CURRENT_BINARY_DIR}/vnifiles.sha1" OLDSHA1)
#endif(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/vnifiles.sha1")
## if chacksum changed generate code
#if(NOT "${NEWSHA1}" EQUAL "${OLDSHA1}")
#  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/vnifiles.sha1" "${NEWSHA1}")
#  execute_process(
#    COMMAND ${VNICPPCODEGEN} ${VNL_INTERFACE_DIRS}
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
#    RESULT_VARIABLE VNIGENRESULT
#  )
#  if(!"${VNIGENRESULT}" EQUAL "0")
#    message(FATAL_ERROR "  failed to generate code!")
#  endif(!"${VNIGENRESULT}" EQUAL "0")
#endif(NOT "${NEWSHA1}" EQUAL "${OLDSHA1}")

add_subdirectory(lib)
add_subdirectory(src)
