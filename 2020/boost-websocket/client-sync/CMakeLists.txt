add_executable(client-sync
    websocket_client_sync.cpp
    )

target_link_libraries(client-sync
    PRIVATE OpenSSL::Crypto
    PRIVATE pthread
    )

set_property(TARGET client-sync PROPERTY CXX_STANDARD 17)
