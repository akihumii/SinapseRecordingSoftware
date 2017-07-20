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
#include "datathread.h"

using namespace std;

DataThread::DataThread(DataStreamFifo* incomingFifo_, QVector<QQueue<double>> outgoingFifo_, DataProcessor* neutrinoData_, QObject *parent):
    QThread(parent),
    incomingFifo(incomingFifo_),
    outgoingFifo(outgoingFifo_),
    NeutrinoData(neutrinoData_)
{
    keepGoing = false;
    running = false;
    stopThread = false;
    numUsbBlocksToRead = 1;
    unsigned int bufferSize = 32 * 2 * 31744;
    cout << "DataThread: Allocating " << bufferSize / 1.0e6 << " MBytes for USB buffer." << endl;
    dataBuffer = new unsigned char [bufferSize];
}

DataThread::~DataThread()
{
    delete [] dataBuffer;
}

void DataThread::run()
{
    while (!stopThread) {
        if (keepGoing) {
            running = true;
            while (keepGoing && !stopThread) {
                if(incomingFifo->readFromBuffer(dataBuffer, 20480)){
                    qDebug() << "successfully read from datafifo" << currentThreadId();
                    NeutrinoData->processData(BITMODE8, (const char *)dataBuffer);
                    for(int i = 0; i < 10; i++){
                            if(NeutrinoData->retrieveData(i).size() >  4096){
//                                outgoingFifo[0]->writeToBuffer(dataBuffer, 2048);
                                for(int j = 0; j < NeutrinoData->retrieveData(i).size(); j++){
                                    outgoingFifo[i].append(NeutrinoData->retrieveData(i).at(j));
                                }
                            qDebug() << outgoingFifo[i].size();
                            }
                    }
                }
            }
            running = false;
        } else {
            usleep(100);
        }
    }
}

void DataThread::startRunning()
{
    keepGoing = true;
}

void DataThread::stopRunning()
{
    keepGoing = false;
}

void DataThread::close()
{
    keepGoing = false;
    stopThread = true;
}

bool DataThread::isRunning() const
{
    return running;
}

void DataThread::setNumUsbBlocksToRead(int numUsbBlocksToRead_)
{
    if (numUsbBlocksToRead_ > 32) {
        cerr << "DataThread::setNumUsbBlocksToRead: Buffer is too small to read " << numUsbBlocksToRead_ <<
                " blocks.  Increase BUFFER_SIZE_IN_BLOCKS." << endl;
    }
    numUsbBlocksToRead = numUsbBlocksToRead_;
}
