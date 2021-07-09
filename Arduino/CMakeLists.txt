
if(WIN32)
#find_library(fmt CONFIG REQUIRED)
endif()

add_library(Arduino STATIC 
	"SerialPort.hpp" "SerialPort.cpp"
	"SimpleRPC.hpp" "SimpleRPC.cpp"
	"Serialization.hpp" "Serialization.cpp"
	"ArduinoChessBoard.h" "ArduinoChessBoard.cpp"
	"ArduinoExported.h"
	"Matrix8x8.h"
 )

if(WIN32)
target_link_libraries(Arduino fmt::fmt)
endif()
