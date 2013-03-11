#ifndef __LAYERFEATURES_H__
#define __LAYERFEATURES_H__

#include <QString>
#include <QGraphicsScene>
#include <QList>
#include <QGridLayout>
#include <QTextEdit>

#include "symbol.h"
#include "record.h"
#include "featuresparser.h"

class LayerFeatures: public Symbol {
public:
  LayerFeatures(QString step, QString path);
  virtual ~LayerFeatures();

  virtual QRectF boundingRect() const;
  void addToScene(QGraphicsScene* scene);
  QTableWidget *symbolCount();

  qreal x_datum(void) { return m_x_datum; }
  qreal y_datum(void) { return m_y_datum; }

  void setTransform(const QTransform& matrix, bool combine = false);
  void setPos(QPointF pos);
  void setPos(qreal x, qreal y);
  void setVisible(bool status);
  void setShowStepRepeat(bool status);

private:
  LayerFeatures* m_virtualParent;
  FeaturesDataStore* m_ds;
  LayerFeatures* m_profile;
  QRectF m_activeRect;
  qreal m_x_datum, m_y_datum;
  qreal m_x_origin, m_y_origin;
  bool m_showRepeat;
  QList<LayerFeatures*> m_repeats;
  QTransform m_trans;
};

#endif /* __LAYERFEATURES_H__ */