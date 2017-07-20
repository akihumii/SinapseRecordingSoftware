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

#ifndef DATAHANDLERTHREAD_H
#define DATAHANDLERTHREAD_H

#include <QObject>
#include <QThread>
#include "datastreamfifo.h"
#include "dataprocessor.h"
#include "../common/qtincludes.h"

#define BUFFER_SIZE_IN_BLOCKS 32

class DataHandlerThread : public QThread
{
    Q_OBJECT
public:
    explicit DataHandlerThread(DataStreamFifo* dataFifo_, QSerialPort *serialData_,  QObject *parent = 0);
    ~DataHandlerThread();

    void run() override;
    void startRunning();
    void stopRunning();
    bool isRunning() const;
    void close();
    void setNumUsbBlocksToRead(int numUsbBlocksToRead_);

signals:
//    void finished();

public slots:

private:
    QSerialPort* serialData;
    DataStreamFifo* dataFifo;
    volatile bool keepGoing;
    volatile bool running;
    volatile bool stopThread;
    volatile int numUsbBlocksToRead;

    unsigned char* dataBuffer;

};

#endif // DATAHANDLERTHREAD_H
