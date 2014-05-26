/*
*  Copyright (C) 2014 Kyle Manna <kyle@kylemanna.com>
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 2 or (at your option)
*  version 3 of the License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QFile>
#include <QXmlStreamReader>

#include "core/Tools.h"
#include "crypto/CryptoHash.h"
#include "crypto/Random.h"

#include "keys/YkChallengeResponseKey.h"
#include "keys/drivers/Yubikey.h"

YkChallengeResponseKey::YkChallengeResponseKey(int slot,
                                               bool blocking)
    : m_slot(slot),
      m_blocking(blocking)
{
}

QByteArray YkChallengeResponseKey::rawKey() const
{
    return m_key;
}

YkChallengeResponseKey* YkChallengeResponseKey::clone() const
{
    return new YkChallengeResponseKey(*this);
}


/** Assumes yubikey()->init() was called */
bool YkChallengeResponseKey::challenge(const QByteArray& chal)
{
    if (Yubikey::instance()->challenge(m_slot, true, chal, m_key) != Yubikey::ERROR) {
        return true;
    }

    return false;
}

QString YkChallengeResponseKey::getName() const
{
    unsigned int serial;
    QString fmt("Yubikey[%1] Challenge Response - Slot %2 - %3");

    Yubikey::instance()->getSerial(serial);

    return fmt.arg(QString::number(serial),
                   QString::number(m_slot),
                   (m_blocking) ? "Press" : "Passive");
}

bool YkChallengeResponseKey::isBlocking() const
{
    return m_blocking;
}
