cmake_minimum_required(VERSION 2.8.9)
project(pairly-db)

enable_testing()

find_package(Boost COMPONENTS unit_test_framework system python3 REQUIRED)

find_package(PythonLibs REQUIRED)
FIND_PACKAGE(PythonInterp)

#find_package(Boost 1.64.0 REQUIRED COMPONENTS system python)
message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

if (${BUILD_MONGO})
    file(GLOB SOURCES "src/*.cpp")
    add_library(pairly-db SHARED ${SOURCES})

    # Python lib
    add_library(pypairly SHARED python-lib/pairly-db.cpp)
    target_link_libraries(pypairly ${Boost_LIBRARIES} pairly-db)
    target_link_libraries(pypairly ${PYTHON_LIBRARIES})
    set_target_properties(pypairly PROPERTIES PREFIX "")

    find_package(libmongocxx REQUIRED)
    find_package(libbsoncxx REQUIRED)
    SET(CMAKE_EXE_LINKER_FLAGS 
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib")

    target_include_directories(pairly-db PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
                                                ${LIBBSONCXX_INCLUDE_DIRS})

    target_include_directories(pypairly PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
                                                ${LIBBSONCXX_INCLUDE_DIRS})

    target_link_libraries(pairly-db PRIVATE ${LIBMONGOCXX_LIBRARIES}
                                            ${LIBBSONCXX_LIBRARIES})

    target_compile_definitions(pairly-db PRIVATE ${LIBMONGOCXX_DEFINITIONS})


else()
    set(SOURCES "src/pairly.cpp"
                "src/data-factory-mock.cpp")
    add_library(pairly-db SHARED ${SOURCES})
endif()

# TODO: set Debug or release
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic -Wextra")

if (${SANITIZE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()
 
include_directories(include)



file(GLOB TEST_SRCS "tests/*.cpp")

foreach(testSrc ${TEST_SRCS})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)

        #Add compile target
        add_executable(${testName} ${testSrc})
        
        #link to Boost libraries AND your targets and dependencies
        target_link_libraries(${testName} ${Boost_LIBRARIES} pairly-db)

        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/testBin)

        #Finally add it to test execution - 
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName} 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin 
                 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName} )
endforeach(testSrc)

message("-- Compile with: ${CMAKE_CXX_FLAGS}")
#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
#install(TARGETS testStudent DESTINATION /usr/lib)
