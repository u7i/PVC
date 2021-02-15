import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import org.u7i.PVC 0.1

RowLayout{
    ControllerUtils {
        id: utils
    }
    ControllerLogic {
        id: logic
            
        onCurrentPlayerUpdated: {
            songTitle.text = logic.getCurrentPlayerSongTitle()
            playerName.text = logic.getCurrentPlayerName()
                
            // Update buttons
            switch(logic.getActionButtonState()){
                case ActionButtonState.DoPlay:
                    actionButtonIcon.pixmap = utils.getStandardIconAsPixmap(Style.SP_MediaPlay, 16, 16)
                    
                    actionButtonIcon.enabled = true
                    actionButton.enabled = true
                    break;
                    
                case ActionButtonState.DoPause:
                    actionButtonIcon.pixmap = utils.getStandardIconAsPixmap(Style.SP_MediaPause, 16, 16)
                    
                    actionButtonIcon.enabled = true
                    actionButton.enabled = true
                    break;
                    
                case ActionButtonState.None:
                    actionButtonIcon.pixmap = utils.getStandardIconAsPixmap(Style.SP_MediaPlay, 16, 16)
                    
                    actionButtonIcon.enabled = false
                    actionButton.enabled = false
                    break;
                    
            }
            
            // Set next button enable state
            nextButtonIcon.enabled = logic.isNextButtonEnabled()
            nextButton.enabled = logic.isNextButtonEnabled()
        }
    }
    
    // Constructor
    Component.onCompleted: {
        logic.onCurrentPlayerUpdated()
    }
    
    // GUI setup
    Column{
        id: labelLayout
        spacing: 0
        
        width: utils.getLabelWidth(logic.getLongestStringFromAvailable(), songTitle.font, 30)
        
        Label {
            id: songTitle
            width: parent.width
                
            text: "It's work"
            wrapMode: Text.Wrap
        
            horizontalAlignment: Text.AlignRight
        }
        Label {
            id: playerName
            width: parent.width
                
            text: "It's work"
            wrapMode: Text.Wrap
            font.weight: Font.Bold
        
            horizontalAlignment: Text.AlignRight
        }
    }


    RowLayout{
        id: buttonLayout
        spacing: 0

        Button {
            id: actionButton
            font.pixelSize: 32

            Layout.preferredWidth: 32
            Layout.preferredHeight: 32

            onClicked: {
                logic.actionButtonClicked()
            }
            
            PixmapViewer{
                id: actionButtonIcon
                
                pixmap: utils.getStandardIconAsPixmap(Style.SP_MediaPlay, 16, 16)
                anchors.centerIn: parent

                width: 16
                height: 16
            }
        }
        Button {
            id: nextButton
            
            font.pixelSize: 32

            Layout.preferredWidth: 16
            Layout.preferredHeight: 32

            onClicked: {
                logic.nextButtonClicked()
            }
            
            PixmapViewer{
                id: nextButtonIcon
                
                pixmap: utils.getStandardIconAsPixmap(Style.SP_ArrowRight, 16, 32)
                anchors.centerIn: parent

                width: 16
                height: 32
            }
        }
    }
}
