#include "feature.h"

#include "context.h"
#include <typeinfo>

extern Context ctx;

Features::Features(QString path)
{
  FeaturesParser parser(path);
  m_ds = parser.parse();
  m_records = m_ds->records();
}

void Features::add(QGraphicsScene* scene)
{
  for (QList<Record*>::const_iterator it = m_records.begin();
      it != m_records.end(); ++it) {
    Record* rec = *it;
    rec->add(scene);
  }
}
