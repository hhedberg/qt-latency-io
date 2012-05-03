/*
 * LatencyIODevice - A QIODevice with pluggable latency model
 *
 * (c) 2012 Henrik Hedberg <henrik.hedberg@iki.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 *
 */

#include <string.h>

#include "EchoIODevice.h"

EchoIODevice::EchoIODevice(QObject *parent)
	: QIODevice(parent)
{
	open(QIODevice::ReadWrite);
}

qint64 EchoIODevice::bytesAvailable() const
{
	return byteArray.size();
}

qint64 EchoIODevice::readData(char *data, qint64 maxSize)
{
	if (maxSize > byteArray.size()) {
		maxSize = byteArray.size();
	}
	memcpy(data, byteArray.data(), maxSize);
	byteArray.remove(0, maxSize);
	
	return maxSize;
}

qint64 EchoIODevice::writeData(const char *data, qint64 maxSize)
{
	if (maxSize > 0) {
		byteArray.append(data, maxSize);
		emit readyRead();
	}
	return maxSize;
}
