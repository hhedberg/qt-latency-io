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

#include <QDateTime>
#include <string.h>

#include "LatencyIODevice.h"

LatencyIODevice::LatencyIODevice(QIODevice *device, AbstractLatencyModel *latencyModel, QObject *parent)
	: QIODevice(parent), mDevice(device), mLatencyModel(latencyModel), readableBytes(0), readQueueByteSize(0)
{
	open(QIODevice::ReadWrite);
	writeTimer.setSingleShot(true);
	readTimer.setSingleShot(true);
	
	connect(mDevice, SIGNAL(readyRead()), SLOT(handleReadyRead()));
	connect(&writeTimer, SIGNAL(timeout()), SLOT(handleWriteTimeout()));
	connect(&readTimer, SIGNAL(timeout()), SLOT(handleReadTimeout()));
}

qint64 LatencyIODevice::bytesAvailable() const
{
	return readableBytes;
}

QIODevice *LatencyIODevice::device()
{
	return mDevice;
}

AbstractLatencyModel *LatencyIODevice::latencyModel()
{
	return mLatencyModel;
}

void LatencyIODevice::setLatencyModel(AbstractLatencyModel *latencyModel)
{
	mLatencyModel = latencyModel;	
}

qint64 LatencyIODevice::readData(char *data, qint64 maxSize)
{
	qint64 size = maxSize;
	if (size > readableBytes) {
		size = readableBytes;
	}
	
	qint64 got = mDevice->read(data, size);
	if (got > 0) {
		readableBytes -= got;
	}

	return got;
}

qint64 LatencyIODevice::writeData(const char *data, qint64 maxSize)
{
	if (maxSize > 0) {
		int latency = mLatencyModel ? mLatencyModel->writeLatency(maxSize) : 0;
		qint64 time = QDateTime::currentMSecsSinceEpoch() + latency;
		if (!writeQueue.isEmpty() && writeQueue.last().time > time) {
			time = writeQueue.last().time;
		}

		writeByteArray.append(data, maxSize);
		writeQueue.enqueue(Queued(time, maxSize));
	
		if (!writeTimer.isActive()) {
			handleWriteTimeout();
		}
	}
	return maxSize;
}

void LatencyIODevice::handleReadyRead()
{
	qint64 got = mDevice->bytesAvailable() - readQueueByteSize;
	if (got > 0) {
		int latency = mLatencyModel ? mLatencyModel->readLatency(got) : 0;
		qint64 time = QDateTime::currentMSecsSinceEpoch() + latency;
		if (!readQueue.isEmpty() && readQueue.last().time > time) {
			time = readQueue.last().time;
		}

		readQueue.enqueue(Queued(time, got));
		readQueueByteSize += got;
		
		if (!readTimer.isActive()) {
			handleReadTimeout();
		}
	}
}

void LatencyIODevice::handleReadTimeout()
{
	while (!readQueue.isEmpty()) {
		qint64 until = readQueue.head().time - QDateTime::currentMSecsSinceEpoch();
		if (until > 0) {
			readTimer.start(until);
			break;
		}
	
		qint64 got = readQueue.dequeue().size;
		readQueueByteSize -= got;

		readableBytes += got;	
		emit readyRead();
	}
}

void LatencyIODevice::handleWriteTimeout()
{
	while (!writeQueue.isEmpty()) {
		qint64 until = writeQueue.head().time - QDateTime::currentMSecsSinceEpoch();
		if (until > 0) {
			writeTimer.start(until);
			break;
		}
		
		qint64 wrote = writeQueue.dequeue().size;
		QByteArray bytes = writeByteArray.left(wrote);
		writeByteArray.remove(0, wrote);
		mDevice->write(bytes);
	}
}
