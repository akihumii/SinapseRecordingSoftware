#include "dataanalyzer.h"

DataAnalyzer::DataAnalyzer(){
    setWindowTitle(tr("Data Analyzer"));
    createAction();
    createMenu();
    createLayout();
}

void DataAnalyzer::createAction(){
}

void DataAnalyzer::createLayout(){
    openFileButton = new QPushButton(tr("Open File"));
    openFileLineEdit = new QLineEdit;
    connect(openFileButton, SIGNAL(clicked(bool)), this, SLOT(on_openFile_triggered()));
    dataGraph = new QCustomPlot;
    dataGraph->addGraph();
    dataGraph->xAxis->setVisible(true);
    dataGraph->axisRect()->setAutoMargins(QCP::msNone);
    dataGraph->axisRect()->setMargins(QMargins(75,0,0,15));
    dataGraph->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
    dataGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

    connect(dataGraph, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(dataGraph, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    QHBoxLayout *openFileLayout = new QHBoxLayout;
    openFileLayout->addWidget(openFileLineEdit);
    openFileLayout->addWidget(openFileButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(openFileLayout);
    mainLayout->addWidget(dataGraph);

    setLayout(mainLayout);
}

void DataAnalyzer::createMenu(){
}


void DataAnalyzer::on_openFile_triggered(){
    QStringList items;
    items << tr("Channel 1")
          << tr("Channel 2")
          << tr("Channel 3")
          << tr("Channel 4")
          << tr("Channel 5")
          << tr("Channel 6")
          << tr("Channel 7")
          << tr("Channel 8")
          << tr("Channel 9")
          << tr("Channel 10");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Channel Selection"),
                                            tr("Which Channel Would You Like to Analyze?"), items, 0, false, &ok);
    QString filename =  QFileDialog::getOpenFileName(
                                            this,
                                            "Open Document",
                                            QDir::currentPath(),
                                            "All files (*.*)");
    if(!filename.isNull()){
        openFileLineEdit->setText(filename.toUtf8());
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
            QVector<double> X_axis;
            int total_data_points = 0;
            while(!in.atEnd()){
                QString line = in.readLine();
                for(int i = 0; i < count; i++){
                    channelData[i].append(line.split(" ,")[i].toInt()*1.2/256);
                }
                total_data_points++;
                X_axis.append(total_data_points*0.000056);
            }
            dataGraph->graph()->setData(X_axis, channelData[item.split("Channel ")[1].toInt()-1]);
            dataGraph->replot();
        }
    }
}

DataAnalyzer::~DataAnalyzer(){
}


void DataAnalyzer::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (dataGraph->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      dataGraph->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    dataGraph->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    dataGraph->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (dataGraph->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      dataGraph->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    dataGraph->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    dataGraph->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
//  for (int i=0; i<dataGraph->graphCount(); ++i)
//  {
//    QCPGraph *graph = dataGraph->graph(i);
//    QCPPlottableLegendItem *item = dataGraph->legend->itemWithPlottable(graph);
//    if (item->selected() || graph->selected())
//    {
//      item->setSelected(true);
//      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
//    }
//  }
}

void DataAnalyzer::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (dataGraph->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    dataGraph->axisRect()->setRangeZoom(dataGraph->xAxis->orientation());
  else if (dataGraph->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    dataGraph->axisRect()->setRangeZoom(dataGraph->yAxis->orientation());
  else
    dataGraph->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
