import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

// By importing module by path, the component appears
// in the Component tab in design mode.
// This makes the component available for preview and
// be able to dropped in design mode.
import "../Task1/MyComponents/ModernButton"
import "../Task1/OtherComponents/ModernButton"

Window {
    id: window
    objectName: "window"
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


// =====================================================================================================================================================================

    // Terminate the thread when the window is close - begin ------------------------------------------------------------------
    Connections
    {
        target: window
        function onClosing (){ toThread("terminate", "") } // new syntax
    }
    // Terminate the thread when the window is close - begin ------------------------------------------------------------------


// =====================================================================================================================================================================


// Signal handling - begin ------------------------------------------------------------------------------------------------------------------------------------

    // Signal transfer send data - begin ------------------------------------------------------------------
    signal toThread(string rev_mess, var rev_data)
    signal fromThread(string rev_mess, var rev_data)
    // Signal transfer send data - end ------------------------------------------------------------------

    // Handle data from Thread - begin ------------------------------------------------------------------
    onFromThread: function(rev_mess, rev_data)
    {
        // This block of code is allowed to be changed - begin -------------------
        if (rev_mess === "size")
        {
            button.height = rev_data.height
            button.width = rev_data.width
        }
        else
        {
            console.log(rev_mess + " " + rev_data)
        }

        // This block of code is allowed to be changed - end -------------------
    }
    // Handle data from Thread - end ------------------------------------------------------------------

// Signal handling - end ---------------------------------------------------------------------------------------------------




    // this ModernButton is from OtherComponents
    // not from the MyComponents.
    // Reason: if there are multiple module have
    // the same component name (ModernButton in this case)
    // the component in the last imported module is used
    ModernButton {
        objectName: "button"
        id: button
        x: 383
        y: 285
        text: qsTr("main")
        width: 140
        height: 45

        onClicked:
        {
            var data_to_thread
            if (qmess.text === "size")
            {
                // data_to_thread = Qt.size(int(qdata.text), int(qdata.text))
                data_to_thread = Qt.size(Number(qdata.text), Number(qdata.text))
            }
            else
            {
                data_to_thread = qdata.text
            }

            console.log(qmess.text + " " + data_to_thread)
            toThread(qmess.text, data_to_thread)
        }
    }

    TextField {
        id: qmess
        x: 205
        y: 149
        width: 318
        height: 40
        placeholderText: qsTr("mess")
    }

    TextField {
        id: qdata
        x: 205
        y: 214
        width: 318
        height: 40
        placeholderText: qsTr("data")
    }

    OtherComponentsWrapperModernButton {
        id: otherComponentsWrapperModernButton
        x: 383
        y: 374
    }

}
