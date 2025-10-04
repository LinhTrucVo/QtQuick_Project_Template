set(Task_1_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/Task_1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data_Object/Task_1_Data.cpp
    PARENT_SCOPE
)

set(Task_1_include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Data_Object
    PARENT_SCOPE
)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(App
    URI UI.Task_1Content.App
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Task_1Content/App
    RESOURCE_PREFIX /qt/qml/UI/Task_1Content/
    QML_FILES
        UI/Task_1Content/App.qml
)

qt_add_qml_module(MyComponents
    URI UI.Task_1.MyComponents
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Task_1Content/MyComponents
    RESOURCE_PREFIX /qt/qml/UI/Task_1Content/MyComponents
    QML_FILES
        UI/Task_1/MyComponents/ModernButton/ModernButton.qml
        UI/Task_1/MyComponents/ModernButton/WrapperModernButton.qml
)

qt_add_qml_module(OtherComponents
    URI UI.Task_1.OtherComponents.ModernButton
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Task_1Content/OtherComponents
    RESOURCE_PREFIX /qt/qml/UI/Task_1Content/OtherComponents
    QML_FILES
        UI/Task_1/OtherComponents/ModernButton/ModernButton.qml
        UI/Task_1/OtherComponents/ModernButton/WrapperModernButton.qml
)

add_library(Task_1_UI INTERFACE)
target_link_libraries(Task_1_UI
    INTERFACE
        App
        MyComponents
        OtherComponents
)

