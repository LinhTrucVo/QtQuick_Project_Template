cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(QtQuick_Project
    # Common Section - begin ----------------------------
    lib/bico_quithread.cpp
    lib/QtLib_Project_Template/src/bico_qthread.cpp
    lib/QtLib_Project_Template/src/bico_qmessdata.cpp
    # Common Section - end ----------------------------


    # Specific Section - begin ----------------------------

    main.cpp

    # Task 1 ------------- - begin
    Client_Code/Task_1/Task_1.cpp
    Client_Code/Task_1/Data_Object/Task_1_Data.cpp
    # Task 1 ------------- - end
    
    # Specific Section - end ----------------------------
)

target_include_directories(QtQuick_Project
    PRIVATE
        # Common Section - begin ----------------------------
        lib
        lib/QtLib_Project_Template/src
        # Common Section - end ----------------------------
        
        
        # Specific Section - begin ----------------------------
        
        # Task 1 ------------- - begin
        Client_Code/Task_1
        # Task 1 ------------- - end

        # Specific Section - end ----------------------------
)

qt_add_resources(QtQuick_Project "qml"
    PREFIX "/"
    FILES
        # Common Section - begin ----------------------------
        # ...
        # Common Section - end ----------------------------
        
        
        # Specific Section - begin ----------------------------

        # Task 1 ------------- - begin
        Client_Code/Task_1/UI/Task_1Content/App.qml
        Client_Code/Task_1/UI/Task_1/qmldir
        Client_Code/Task_1/UI/Task_1/Constants.qml
        Client_Code/Task_1/UI/Task_1/MyComponents/ModernButton/qmldir
        Client_Code/Task_1/UI/Task_1/MyComponents/ModernButton/ModernButton.qml
        Client_Code/Task_1/UI/Task_1/MyComponents/ModernButton/WrapperModernButton.qml
        Client_Code/Task_1/UI/Task_1/OtherComponents/ModernButton/ModernButton.qml
        Client_Code/Task_1/UI/Task_1/OtherComponents/ModernButton/WrapperModernButton.qml
        # Task 1 ------------- - end

        # Specific Section - begin ----------------------------
)

# set(QML_IMPORT_PATH
#     ${CMAKE_CURRENT_SOURCE_DIR}/Client_Code/Task_1/UI
#     CACHE STRING "QML import path" FORCE
# )

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
