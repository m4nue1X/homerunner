
add_library(hrbtlebluez SHARED
  BluezBTLEConnector.cpp
)

target_link_libraries(hrbtlebluez
  ${LIBHR_LIBS}
)
