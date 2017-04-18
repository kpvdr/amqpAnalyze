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
    amqpAnalyze/amqp10/FieldType.hpp
    amqpAnalyze/amqp10/FrameBase.hpp
    amqpAnalyze/amqp10/FrameBuffer.hpp
    amqpAnalyze/amqp10/Performative.hpp
    amqpAnalyze/amqp10/ProtocolHeader.hpp
    amqpAnalyze/amqp10/ProvidesRequires.hpp
    amqpAnalyze/amqp10/Section.hpp
    amqpAnalyze/amqp10/Type.hpp
    amqpAnalyze/AmqpDissector.hpp
    amqpAnalyze/Error.hpp
    amqpAnalyze/FileParser.hpp
    amqpAnalyze/IpConnectionMap.hpp
    amqpAnalyze/IpDissector.hpp
    amqpAnalyze/Ip4Dissector.hpp
    amqpAnalyze/Ip6Dissector.hpp
    amqpAnalyze/Packet.hpp
    amqpAnalyze/TcpDissector.hpp
    amqpAnalyze/WireDissector.hpp
    std/AnsiTermColors.hpp
)

set(amqpAnalyze_SOURCES
    ${amqpAnalyze_HEADERS}
    amqpAnalyze.cpp
    amqpAnalyze/amqp10/FieldType.cpp
    amqpAnalyze/amqp10/FrameBase.cpp
    amqpAnalyze/amqp10/FrameBuffer.cpp
    amqpAnalyze/amqp10/Performative.cpp
    amqpAnalyze/amqp10/ProtocolHeader.cpp
    amqpAnalyze/amqp10/ProvidesRequires.cpp
    amqpAnalyze/amqp10/Section.cpp
    amqpAnalyze/amqp10/Type.cpp
    amqpAnalyze/AmqpDissector.cpp
    amqpAnalyze/Error.cpp
    amqpAnalyze/FileParser.cpp
    amqpAnalyze/IpConnectionMap.cpp
    amqpAnalyze/IpDissector.cpp
    amqpAnalyze/Ip4Dissector.cpp
    amqpAnalyze/Ip6Dissector.cpp
    amqpAnalyze/Packet.cpp
    amqpAnalyze/TcpDissector.cpp
    amqpAnalyze/WireDissector.cpp
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