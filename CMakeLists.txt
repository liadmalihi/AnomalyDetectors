cmake_minimum_required(VERSION 3.0.0)
project(ex6 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(ex6 MainTrain.cpp CLI.cpp HybridAnomalyDetector.cpp minCircle.cpp SimpleAnomalyDetector.cpp timeseries.cpp anomaly_detection_util.cpp Server.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
