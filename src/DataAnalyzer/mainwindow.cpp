#include "mainwindow.h"

MainWindow::MainWindow(QString filename){
    if(!filename.isNull()){
        QFile rawData(filename);
        if(rawData.open(QIODevice::ReadOnly)){
            QTextStream in(&rawData);
            QString firstLine = in.readLine();
            int count = 0;
            for(int i = 0; i < firstLine.size(); i++){
                if(firstLine.at(i) == ','){
                    count++;
                }
            }
            QVector<double> channelData[count];
            int total_data_points = 0;
            while(!in.atEnd()){
                QString line = in.readLine();
                for(int i = 0; i < count; i++){
                    channelData[i].append(line.split(" ,")[i].toInt()*1.2/256);
                }
                total_data_points++;
            }
            for(int i = 0; i < count; i++){
                dataEnable[i] = new QCheckBox;
            }
        }
    }
}

MainWindow::~MainWindow()
{
}
