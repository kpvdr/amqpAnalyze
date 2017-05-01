# Set the version number here
set(amqpAnalyze_VERSION_MAJOR 0)
set(amqpAnalyze_VERSION_MINOR 1)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amqpAnalyze_config.hpp.in
               ${CMAKE_CURRENT_BINARY_DIR}/amqpAnalyze_config.hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(amqpAnalyze_HEADERS
    amqpAnalyze.hpp
    amqpAnalyze/amqp10/AmqpBlock.hpp
    amqpAnalyze/amqp10/Connection.hpp
    amqpAnalyze/amqp10/ConnectionHandler.hpp
    amqpAnalyze/amqp10/ConnectionState.hpp
    amqpAnalyze/amqp10/Decoder.hpp
    amqpAnalyze/amqp10/Endpoint.hpp
    amqpAnalyze/amqp10/FieldType.hpp
    amqpAnalyze/amqp10/Frame.hpp
    amqpAnalyze/amqp10/FrameBuffer.hpp
    amqpAnalyze/amqp10/FrameError.hpp
    amqpAnalyze/amqp10/FwdDecls.hpp
    amqpAnalyze/amqp10/IllegalStateError.hpp
    amqpAnalyze/amqp10/Performative.hpp
    amqpAnalyze/amqp10/ProtocolHeader.hpp
    amqpAnalyze/amqp10/ProvidesRequires.hpp
    amqpAnalyze/amqp10/Section.hpp
    amqpAnalyze/amqp10/Type.hpp
    amqpAnalyze/AmqpDissector.hpp
    amqpAnalyze/Dissector.hpp
    amqpAnalyze/Error.hpp
    amqpAnalyze/FileParser.hpp
    amqpAnalyze/FwdDecls.hpp
    amqpAnalyze/IpDissector.hpp
    amqpAnalyze/Ip4Dissector.hpp
    amqpAnalyze/Ip6Dissector.hpp
    amqpAnalyze/Options.hpp
    amqpAnalyze/Packet.hpp
    amqpAnalyze/TcpAddressInfo.hpp
    amqpAnalyze/TcpConnectionMap.hpp
    amqpAnalyze/TcpDissector.hpp
    #amqpAnalyze/Utils.hpp
    std/AnsiTermColors.hpp
)

set(amqpAnalyze_SOURCES
    ${amqpAnalyze_HEADERS}
    amqpAnalyze.cpp
    amqpAnalyze/amqp10/AmqpBlock.cpp
    amqpAnalyze/amqp10/Connection.cpp
    amqpAnalyze/amqp10/ConnectionHandler.cpp
    amqpAnalyze/amqp10/ConnectionState.cpp
    amqpAnalyze/amqp10/Decoder.cpp
    amqpAnalyze/amqp10/Endpoint.cpp
    amqpAnalyze/amqp10/FieldType.cpp
    amqpAnalyze/amqp10/Frame.cpp
    amqpAnalyze/amqp10/FrameBuffer.cpp
    amqpAnalyze/amqp10/FrameError.cpp
    amqpAnalyze/amqp10/IllegalStateError.cpp
    amqpAnalyze/amqp10/Performative.cpp
    amqpAnalyze/amqp10/ProtocolHeader.cpp
    amqpAnalyze/amqp10/ProvidesRequires.cpp
    amqpAnalyze/amqp10/Section.cpp
    amqpAnalyze/amqp10/Type.cpp
    amqpAnalyze/AmqpDissector.cpp
    amqpAnalyze/Dissector.cpp
    amqpAnalyze/Error.cpp
    amqpAnalyze/FileParser.cpp
    amqpAnalyze/IpDissector.cpp
    amqpAnalyze/Ip4Dissector.cpp
    amqpAnalyze/Ip6Dissector.cpp
    amqpAnalyze/Options.cpp
    amqpAnalyze/Packet.cpp
    amqpAnalyze/TcpAddressInfo.cpp
    amqpAnalyze/TcpConnectionMap.cpp
    amqpAnalyze/TcpDissector.cpp
    #amqpAnalyze/Utils.cpp
)
 

add_executable(amqpAnalyze
               ${amqpAnalyze_SOURCES}
)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11"
)
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -l pcap"
)