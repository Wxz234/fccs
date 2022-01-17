add_library(fccs STATIC 
"fccs.cpp"
"device.cpp"
"dx12_backend.cpp"
"commandlist.cpp"
"commandqueue.cpp"
"window.cpp"
"swapchain.cpp"
"format.cpp"
)

target_include_directories(fccs PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_compile_definitions(fccs PRIVATE UNICODE)