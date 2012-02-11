set(pluginTester_SRCS
    main.cpp
    pluginTester.cpp
    fakekipiaction.cpp
    fakeinfoshared.cpp
    fakekipiinterface.cpp
    fakeimagecollectionshared.cpp
)

kde4_add_executable(kamosoPluginTester ${pluginTester_SRCS})

target_link_libraries(kamosoPluginTester ${KDE4_KDECORE_LIBRARY} ${KDE4_KIO_LIBRARY} ${KIPI_LIBRARIES})

# Not sure if we want to install this :/
install(TARGETS kamosoPluginTester ${INSTALL_TARGETS_DEFAULT_ARGS})

