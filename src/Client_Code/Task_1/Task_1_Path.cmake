get_filename_component(PARENT_CMAKE_DIR ${CMAKE_PARENT_LIST_FILE} DIRECTORY)
file(RELATIVE_PATH PARENT_CMAKE_RELATIVE_PATH 
     ${PARENT_CMAKE_DIR}
     ${CMAKE_CURRENT_LIST_DIR})

set(TASK_1_SOURCE_FILES
    ${PARENT_CMAKE_RELATIVE_PATH}/Task_1.cpp
    ${PARENT_CMAKE_RELATIVE_PATH}/Data_Object/Task_1_Data.cpp
)

set(TASK_1_INCLUDE_DIRS
    ${PARENT_CMAKE_RELATIVE_PATH}
    ${PARENT_CMAKE_RELATIVE_PATH}/Data_Object
)

set(TASK_1_QML_FILES
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task_1Content/App.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task_1/MyComponents/ModernButton/ModernButton.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task_1/MyComponents/ModernButton/WrapperModernButton.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task_1/OtherComponents/ModernButton/ModernButton.qml
    ${PARENT_CMAKE_RELATIVE_PATH}/UI/Task_1/OtherComponents/ModernButton/OtherComponentsWrapperModernButton.qml
)

