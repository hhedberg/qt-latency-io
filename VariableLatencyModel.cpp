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

#include "VariableLatencyModel.h"

VariableLatencyModel::VariableLatencyModel(int minReadLatency,
                                           int maxReadLatency,
					   int minWriteLatency,
					   int maxWriteLatency,
					   int latencyPeriod,
					   QObject *parent)
	: AbstractLatencyModel(parent),
	  minReadLatency(minReadLatency), maxReadLatency(maxReadLatency),
	  minWriteLatency(minWriteLatency), maxWriteLatency(maxWriteLatency),
	  latencyPeriod(latencyPeriod), latencyPeriodStarted(0)
{
}

int VariableLatencyModel::readLatency(qint64 /* bytes */)
{
	mayRecalculateLatency();
	return currentReadLatency;
}

int VariableLatencyModel::writeLatency(qint64 /* bytes */) {
	mayRecalculateLatency();
	return currentWriteLatency;
}

void VariableLatencyModel::mayRecalculateLatency()
{
	qint64 now = QDateTime::currentMSecsSinceEpoch();
	if (latencyPeriodStarted != 0 && latencyPeriodStarted + latencyPeriod > now)
		return;
		
	currentReadLatency = qrand() % ((maxReadLatency + 1) - minReadLatency) + minReadLatency;
	currentWriteLatency = qrand() % ((maxWriteLatency + 1) - minWriteLatency) + minWriteLatency;
	latencyPeriodStarted = now;
}
