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

#ifndef INCLUDED_CONSTANT_LATENCY_MODEL_H
#define INCLUDED_CONSTANT_LATENCY_MODEL_H

#include <QtGlobal>

#include "AbstractLatencyModel.h"

class ConstantLatencyModel : public AbstractLatencyModel
{
public:
	ConstantLatencyModel(int readLatency = 0, int writeLatency = 0, QObject *parent = 0);
	virtual ~ConstantLatencyModel() { }

	int readLatency(qint64 bytes);
	int writeLatency(qint64 bytes);
	
private:
	int mReadLatency;
	int mWriteLatency;
};

#endif
