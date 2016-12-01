#include "dataprocessor_ma.h"

DataProcessor_MA::DataProcessor_MA()
{

}

void DataProcessor_MA::parseFrameMarkers(QByteArray rawData){
    if(leftOverData.size() > 0){
        for(int i=leftOverData.size()-1;i>=0;i--){
            rawData.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }

    firstFrameMarker = findfirstFrameMarkers(rawData);
    lastFrameMarker = findlastFrameMarkers(rawData);

    for(int i=0;i<lastFrameMarker-5;i=i+1){
        if (i%5 == firstFrameMarker){
            fullWord_rawData = (((quint8) rawData.at(i+1) << 8 | (quint8) rawData.at(i+2))-32768);
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[0].append(fullWord_rawData*(0.000000195/65536));

            fullWord_rawData = (((quint8) rawData.at(i+3) << 8 | (quint8) rawData.at(i+4))-32768);
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[1].append(fullWord_rawData*(0.000000195/65536));

            if(RecordEnabled){
                RecordData((quint8) rawData.at(i));
                RecordData(END_OF_LINE);
            }
            ChannelData[2].append((quint8) rawData.at(i));
            total_data_count = total_data_count+1;
            X_axis.append(total_data_count*0.000048);
        }
    }
    for(int i = lastFrameMarker; i < rawData.size(); i++){
        leftOverData.append(rawData.at(i));
    }
}

int DataProcessor_MA::findfirstFrameMarkers(QByteArray rawData){
    for(int i=0; i<rawData.size()-10;i++){
        if((quint8)rawData.at(i+5) == (quint8)rawData.at(i)+1
                && (quint8)rawData.at(i+10) == (quint8)rawData.at(i+5)+1){
            return i;
        }
        else if((quint8)rawData.at(i+5) == 0 && (quint8)rawData.at(i) == 250){
            return i;
        }
        else if((quint8)rawData.at(i+10) == 0 && (quint8)rawData.at(i+5) == 250){
            return i;
        }
    }
    return -1;
}

int DataProcessor_MA::findlastFrameMarkers(QByteArray rawData){
    if(rawData.size()>10){
        for(int i=rawData.size()-1;i>10;i--){
            if((quint8)rawData.at(i-5)+1 == (quint8)rawData.at(i)
                    && (quint8)rawData.at(i-10)+1 == (quint8)rawData.at(i-5)){
                return i;
            }
            else if((quint8)rawData.at(i-5) == 250 && (quint8)rawData.at(i) == 0){
                return i;
            }
            else if((quint8)rawData.at(i-10) == 250 && (quint8)rawData.at(i-5) == 0){
                return i;
            }
        }
    }
    return -1;
}
