project(amqpAnalyze)

add_subdirectory(src)

cmake_minimum_required(VERSION 2.6)

set (ErrorFlag FALSE)


# Find libpcap_devel
#-------------------
find_library(HAVE_LIBPCAP
             NAMES pcap)
if (HAVE_LIBPCAP STREQUAL "HAVE_LIBPCAP-NOTFOUND")
    message("ERROR: Library libpcap not found.")
    set (ErrorFlag TRUE)
else ()
    message(" Library ${HAVE_LIBPCAP} found.")
endif ()

find_file(HAVE_PCAP_H
          pcapx.h)
if (HAVE_PCAP_H STREQUAL "HAVE_PCAP_H-NOTFOUND")
    message("ERROR: Header file pcap.h not found.")
    set (ErrorFlag TRUE)
else ()
    message(" Header ${HAVE_PCAP_H} found.")
endif ()

# Check for errors, terminate if error(s) exist
#----------------------------------------------
if (ErrorFlag)
    message(FATAL_ERROR "Required component(s) missing (see above), aborting configuration.")
endif ()

