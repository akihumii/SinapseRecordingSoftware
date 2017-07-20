//  ------------------------------------------------------------------------
//
//  This file is part of the Intan Technologies RHS2000 Interface
//  Version 1.01
//  Copyright (C) 2013-2017 Intan Technologies
//
//  ------------------------------------------------------------------------
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "datahandlerthread.h"

using namespace std;

DataHandlerThread::DataHandlerThread(DataStreamFifo* dataFifo_, QSerialPort* serialData_, QObject *parent):
    QThread(parent),
    serialData(serialData_),
    dataFifo(dataFifo_)
{
    keepGoing = false;
    running = false;
    stopThread = false;
    numUsbBlocksToRead = 1;
    unsigned int bufferSize = 32 * 2 * 31744;
    cout << "DataHandlerThread: Allocating " << bufferSize / 1.0e6 << " MBytes for USB buffer." << endl;
    dataBuffer = new unsigned char [bufferSize];
}

DataHandlerThread::~DataHandlerThread()
{
    delete [] dataBuffer;
}

void DataHandlerThread::run()
{
    while (!stopThread) {
        if (keepGoing) {
            running = true;
            while (keepGoing && !stopThread) {
                qDebug() << "Reading in serial data" << currentThreadId();
                dataFifo->writeToBuffer((unsigned char*)serialData->read(2048).data(), 2048);
            }
            running = false;
        } else {
            usleep(100);
        }
    }
}

void DataHandlerThread::startRunning()
{
    keepGoing = true;
}

void DataHandlerThread::stopRunning()
{
    keepGoing = false;
}

void DataHandlerThread::close()
{
    keepGoing = false;
    stopThread = true;
}

bool DataHandlerThread::isRunning() const
{
    return running;
}

void DataHandlerThread::setNumUsbBlocksToRead(int numUsbBlocksToRead_)
{
    if (numUsbBlocksToRead_ > 32) {
        cerr << "DataHandlerThread::setNumUsbBlocksToRead: Buffer is too small to read " << numUsbBlocksToRead_ <<
                " blocks.  Increase BUFFER_SIZE_IN_BLOCKS." << endl;
    }
    numUsbBlocksToRead = numUsbBlocksToRead_;
}
