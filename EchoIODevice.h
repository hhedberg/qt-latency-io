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

#ifndef INCLUDED_ECHO_IO_DEVICE_H
#define INCLUDED_ECHO_IO_DEVICE_H

#include <QIODevice>
#include <QByteArray>

class EchoIODevice : public QIODevice
{
	Q_OBJECT

public:
	EchoIODevice(QObject *parent = 0);
	virtual ~EchoIODevice() { }

	qint64 bytesAvailable() const;
	
protected:
	qint64 readData(char *data, qint64 maxSize);
	qint64 writeData(const char *data, qint64 maxSize);
	
private:
	QByteArray byteArray;
};

#endif
