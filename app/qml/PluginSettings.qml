/*
 * SPDX-FileCopyrightText: 2019 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import org.kde.kirigami 2.0 as Kirigami
import org.kde.kdeconnect 1.0

Kirigami.ScrollablePage
{
    id: root
    title: i18n("Plugin Settings")
    property string device

    ListView {
        id: sinkList

        anchors.fill: parent

        model: PluginModel {
            deviceId: device
        }

        delegate: Kirigami.SwipeListItem {
            width: parent.width
            enabled: true
            supportsMouseEvents: false

            CheckBox {
                checked: isChecked
                text: name
                onClicked: {
                    isChecked = checked
                }
            }

            actions: [
                Kirigami.Action {
                    icon.name: "settings-configure"
                    visible: configSource != ""
                    onTriggered: {
                        if (pageStack.lastItem.toString().startsWith("PluginInfoPage")) {
                            pageStack.lastItem.configFile = configSource
                            pageStack.lastItem.title = name
                        } else {
                            pageStack.push(Qt.resolvedUrl("PluginInfoPage.qml"), {
                                title: name,
                                configFile: configSource,
                                device: root.device
                            })
                        }
                    }
                }
            ]
        }
    }

}


