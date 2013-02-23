#include "jobmatrix.h"
#include "ui_jobmatrix.h"
#include "structuredtextparser.h"
#include "context.h"
#include "QDebug"
#include <QtGui>

extern Context ctx;

using namespace std;

JobMatrix::JobMatrix(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::JobMatrix)
{
  ui->setupUi(this);
}

JobMatrix::~JobMatrix()
{
  delete ui;
}

void JobMatrix::on_CloseButton_clicked()
{
  this->close();
}

void JobMatrix::SetMatrix(StructuredTextDataStore* ds)
{
  QVBoxLayout *layout = new QVBoxLayout();
  StructuredTextDataStore::BlockIterPair ip;
  QString text;
  int steps,layers;

  steps = layers = 0;
  ip = ds->getBlocksByKey("STEP");
  for (StructuredTextDataStore::BlockIter it = ip.first; it != ip.second; ++it)
    steps++;
  ip = ds->getBlocksByKey("LAYER");
  for (StructuredTextDataStore::BlockIter it = ip.first; it != ip.second; ++it)
    layers++;

  QTableWidget *table = new QTableWidget(layers,steps);


  steps = 1;
  ip = ds->getBlocksByKey("STEP");
  for (StructuredTextDataStore::BlockIter it = ip.first; it != ip.second; ++it)
  {
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText((QString)it->second->get("NAME").c_str());
    m_step_name.append((QString)it->second->get("NAME").c_str());
    table->setHorizontalHeaderItem(steps - 1,item);

    steps++;
  }

  layers = 0;
  ip = ds->getBlocksByKey("LAYER");
  for (StructuredTextDataStore::BlockIter it = ip.first; it != ip.second; ++it)
  {
    QTableWidgetItem *item = new QTableWidgetItem();
    text = (QString)it->second->get("TYPE").c_str();
    if(text == "SILK_SCREEN")
      text = "(ss ,";
    else if(text == "SOLDER_MASK")
      text = "(sm ,";
    else if(text == "SIGNAL")
      text = "(sig,";
    else if(text == "DRILL")
      text = "(srl,";
    else if(text == "DOCUMENT")
      text = "(doc,";
    else if(text == "ROUT")
      text = "(rt ,";
    else if(text == "SOLDER_PASTE")
      text = "(sp ,";
    else
      text = "( ,";
    if(it->second->get("POLARITY") == "POSITIVE")
      text += "p)  ";
    else
      text += "n)  ";
    m_layer_name.append((QString)it->second->get("NAME").c_str());
    text += (QString)it->second->get("NAME").c_str();
    item->setText(text);
    table->setVerticalHeaderItem(layers,item);

    for(int i=0;i<m_step_name.size();i++)
    {
      text = m_step_name[i] + "/" + (QString)it->second->get("NAME").c_str();
      QTableWidgetItem *btn = new QTableWidgetItem(text);
      table->setItem(layers,i,btn);

      QString pathTmpl = "steps/%1/layers/%2";
      text = pathTmpl.arg(m_step_name[i].toAscii().data()).arg(
          QString::fromStdString(it->second->get("NAME")));

      if (QFile(ctx.loader->featuresPath(text)).size() == 0) {
        btn->setText("");
      }
    }
    layers++;
  }

  connect(table,SIGNAL(itemClicked(QTableWidgetItem*)),this,
      SLOT(showLayer(QTableWidgetItem *)));
  table->verticalHeader()->setMovable(true);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  layout->addWidget(table);
  ui->scrollWidget->setLayout(layout);
}

void JobMatrix::showLayer(QTableWidgetItem *item)
{
  QStringList params = item->text().split("/");
  ViewerWindow* w = new ViewerWindow;
  w->setStep(params[0]);
  w->setLayers(m_layer_name);
  w->showLayer(params[1]);
  w->show();
}
