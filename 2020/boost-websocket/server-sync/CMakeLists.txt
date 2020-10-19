add_executable(server-sync
    websocket_server_sync.cpp
    )

target_link_libraries(server-sync
    PRIVATE OpenSSL::Crypto
    #PRIVATE OpenSSL::SSL
    PRIVATE pthread
    )

set_property(TARGET server-sync PROPERTY CXX_STANDARD 17)
