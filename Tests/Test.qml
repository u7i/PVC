import QtQuick 2.4
import QtQuick.Layouts 1.3

import org.u7i.PVC 0.1

RowLayout{
    Text {
        id: caption
        text: "Hello, world"
        width: 100
        wrapMode: Text.Wrap
        elide: Text.ElideLeft
        horizontalAlignment: Text.AlignHCenter
    }

   QmlPlaybackWidget{
   }
}