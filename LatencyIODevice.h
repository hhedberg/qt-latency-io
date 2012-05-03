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

#ifndef INCLUDED_LATENCY_IO_DEVICE_H
#define INCLUDED_LATENCY_IO_DEVICE_H

#include <QIODevice>
#include <QByteArray>
#include <QQueue>
#include <QTimer>

#include "AbstractLatencyModel.h"

class LatencyIODevice : public QIODevice
{
	Q_OBJECT

public:
	LatencyIODevice(QIODevice *device, AbstractLatencyModel *latencyModel = 0, QObject *parent = 0);
	virtual ~LatencyIODevice() { }

	qint64 bytesAvailable() const;	
	QIODevice *device();
	AbstractLatencyModel *latencyModel();
	void setLatencyModel(AbstractLatencyModel *latencyModel);
	
protected:
	qint64 readData(char *data, qint64 maxSize);
	qint64 writeData(const char *data, qint64 maxSize);
	
private:
	struct Queued
	{
		Queued(qint64 time, qint64 size) : time(time), size(size) { }
		qint64 time;
		qint64 size;
	};

	QIODevice *mDevice;
	AbstractLatencyModel *mLatencyModel;
	
	QByteArray writeByteArray;
	QQueue<Queued> writeQueue;
	QTimer writeTimer;
	
	qint64 readableBytes;
	QQueue<Queued> readQueue;
	qint64 readQueueByteSize;
	QTimer readTimer;
	
private slots:
	void handleReadyRead();
	void handleWriteTimeout();
	void handleReadTimeout();
};

#endif
