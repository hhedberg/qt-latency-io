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

#include <QLayout>
#include <QApplication>

#include "TestLatencyIODevice.h"
#include "VariableLatencyModel.h"

TestLatencyIODevice::TestLatencyIODevice()
{
	QLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
	label = new QLabel();
	layout->addWidget(label);
	lineEdit = new QLineEdit();
	layout->addWidget(lineEdit);
	
	connect(lineEdit, SIGNAL(returnPressed()), SLOT(handleReturnPressed()));

	echo = new EchoIODevice(this);
	AbstractLatencyModel *latencyModel = new VariableLatencyModel(0, 4000, 0, 4000, 600, this);
	latency = new LatencyIODevice(echo, latencyModel, this);

	connect(latency, SIGNAL(readyRead()), SLOT(handleReadyRead()));
}

void TestLatencyIODevice::handleReturnPressed()
{
	latency->write(lineEdit->text().toAscii());
	lineEdit->clear();
}

void TestLatencyIODevice::handleReadyRead()
{
	QByteArray bytes = latency->readAll();
	label->setText(bytes);
}

int main(int argc, char **argv)
{
	QApplication application(argc, argv);
	
	TestLatencyIODevice test;
	test.show();
	
	return application.exec();
}
