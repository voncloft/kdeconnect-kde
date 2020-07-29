/**
 * Copyright 2014 Pramod Dematagoda <pmdematagoda@mykolab.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "screensaverinhibitplugin.h"

#include <KLocalizedString>
#include <KPluginFactory>
#include <QDBusConnection>
#include "screensaverinterface.h"
#include "kdeconnect_screensaverinhibit_debug.h"

K_PLUGIN_CLASS_WITH_JSON(ScreensaverInhibitPlugin, "kdeconnect_screensaver_inhibit.json")

#define INHIBIT_SERVICE QStringLiteral("org.freedesktop.ScreenSaver")
#define INHIBIT_PATH QStringLiteral("/ScreenSaver")

ScreensaverInhibitPlugin::ScreensaverInhibitPlugin(QObject* parent, const QVariantList& args)
    : KdeConnectPlugin(parent, args)
{
    OrgFreedesktopScreenSaverInterface inhibitInterface(INHIBIT_SERVICE, INHIBIT_PATH, QDBusConnection::sessionBus(), this);

    inhibitCookie = inhibitInterface.Inhibit(QStringLiteral("org.kde.kdeconnect.daemon"), i18n("Phone is connected"));
}

ScreensaverInhibitPlugin::~ScreensaverInhibitPlugin()
{
    if (inhibitCookie == 0) return;

    OrgFreedesktopScreenSaverInterface inhibitInterface(INHIBIT_SERVICE, INHIBIT_PATH, QDBusConnection::sessionBus(), this);
    inhibitInterface.UnInhibit(inhibitCookie);

    /*
     * Simulate user activity because what ever manages the screensaver does not seem to start the timer
     * automatically when all inhibitions are lifted and the user does nothing which results in an
     * unlocked desktop which would be dangerous. Ideally we should not be doing this and the screen should
     * be locked anyway.
     */
    inhibitInterface.SimulateUserActivity();
}

void ScreensaverInhibitPlugin::connected()
{
}

bool ScreensaverInhibitPlugin::receivePacket(const NetworkPacket& np)
{
    Q_UNUSED(np);
    return false;
}

#include "screensaverinhibitplugin.moc"
