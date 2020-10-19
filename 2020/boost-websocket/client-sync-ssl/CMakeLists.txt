add_executable(client-sync-ssl
    websocket_client_sync_ssl.cpp
    )

target_link_libraries(client-sync-ssl
    # PRIVATE ${CONAN_LIBS}
    # PRIVATE Boost::asio
    PRIVATE OpenSSL::Crypto
    PRIVATE OpenSSL::SSL
    PRIVATE pthread
    )

set_property(TARGET client-sync-ssl PROPERTY CXX_STANDARD 17)
