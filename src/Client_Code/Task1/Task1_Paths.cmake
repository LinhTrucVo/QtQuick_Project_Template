get_filename_component(PARENT_CMAKE_DIR ${CMAKE_PARENT_LIST_FILE} DIRECTORY)
file(RELATIVE_PATH PARENT_CMAKE_RELATIVE_PATH 
     ${PARENT_CMAKE_DIR}
     ${CMAKE_CURRENT_LIST_DIR})

set(TASK1_SOURCE_FILES
    ${PARENT_CMAKE_RELATIVE_PATH}/Task1.cpp
    ${PARENT_CMAKE_RELATIVE_PATH}/Data_Object/Task1_Data.cpp
)

set(TASK1_INCLUDE_DIRS
    ${PARENT_CMAKE_RELATIVE_PATH}
    ${PARENT_CMAKE_RELATIVE_PATH}/Data_Object
)

set(TASK1_QML_FILES
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task1Content/App.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task1/MyComponents/ModernButton/ModernButton.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task1/MyComponents/ModernButton/WrapperModernButton.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task1/OtherComponents/ModernButton/ModernButton.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task1/OtherComponents/ModernButton/OtherComponentsWrapperModernButton.qml
)

