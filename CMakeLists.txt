cmake_minimum_required(VERSION 3.16)

project(QtQuick_Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(QtQuick_Project
    # Common Section - begin ----------------------------
    Template_Material/Common/bico_qmutexqueue.h
    Template_Material/Common/bico_qmessdata.h 
    Template_Material/Common/bico_qmessdata.cpp
    Template_Material/Common/bico_qthread.h
    Template_Material/Common/bico_qthread.cpp 
    Template_Material/bico_quithread.h 
    Template_Material/bico_quithread.cpp 
    # Common Section - end ----------------------------

    # Specific Section - begin ----------------------------
    main.cpp

    Client_Code/Bico_QUIThread_Sample/Bico_QUIThread_Sample.h
    Client_Code/Bico_QUIThread_Sample/Bico_QUIThread_Sample.cpp
    Client_Code/Bico_QUIThread_Sample/Data_Object/Bico_QUIThread_Sample_Data.h
    Client_Code/Bico_QUIThread_Sample/Data_Object/Bico_QUIThread_Sample_Data.cpp

    Client_Code/Task_1/Task_1.h
    Client_Code/Task_1/Task_1.cpp
    Client_Code/Task_1/Data_Object/Task_1_Data.h
    Client_Code/Task_1/Data_Object/Task_1_Data.cpp
    # Specific Section - end ----------------------------
)

qt_add_resources(QtQuick_Project "qml"
    PREFIX "/"
    FILES
        # Specific Section - begin ----------------------------
        Client_Code/Bico_QUIThread_Sample/Bico_QUIThread_Sample.qml
        Client_Code/Task_1/Task_1.qml
        # Specific Section - begin ----------------------------
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(QtQuick_Project PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QtQuick_Project
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(QtQuick_Project
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS QtQuick_Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
