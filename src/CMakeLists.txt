
include(Client_Code/Task1/Task1_Paths.cmake)

qt_add_executable(QtQuick_Project
    # Common Section - begin ----------------------------
    lib/bico_quithread.cpp
    lib/QtLib_Project_Template/src/bico_qthread.cpp
    lib/QtLib_Project_Template/src/bico_qmessdata.cpp
    # Common Section - end ----------------------------

    main.cpp

    # Specific Section - begin ----------------------------
    ${TASK1_SOURCE_FILES}
    # Specific Section - end ----------------------------
)

target_include_directories(QtQuick_Project
    PRIVATE
        # Common Section - begin ----------------------------
        lib
        lib/QtLib_Project_Template/src
        # Common Section - end ----------------------------
        
        
        # Specific Section - begin ----------------------------
        ${TASK1_INCLUDE_DIRS}
        # Specific Section - end ----------------------------
)

qt_add_resources(QtQuick_Project "qml"
    PREFIX "/"
    FILES
        ${TASK1_QML_FILES}
)















target_link_libraries(QtQuick_Project
    PRIVATE 
        Qt6::Quick
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
# By enable below properties, the application
# will be created as a GUI application on Windows and macOS,
# without a console window.
# set_target_properties(QtQuick_Project PROPERTIES
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
#     MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
#     MACOSX_BUNDLE_SHORT_VERSION_STRING
#         "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
# )

include(GNUInstallDirs)
install(TARGETS QtQuick_Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
