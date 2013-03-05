#include "squarethermalopencornerssymbol.h"

#include <QtGui>
#include <QRegExp>

#include "macros.h"


SquareThermalOpenCornersSymbol::SquareThermalOpenCornersSymbol(QString def, Polarity polarity):
    Symbol(def, "s_tho([0-9.]+)x([0-9.]+)x([0-9.]+)x([0-9.]+)x([0-9.]+)", polarity), m_def(def)
{
  QRegExp rx(m_pattern);
  if (!rx.exactMatch(def))
    throw InvalidSymbolException(def.toAscii());

  QStringList caps = rx.capturedTexts();
  m_od = caps[1].toDouble() / 1000.0;
  m_id = caps[2].toDouble() / 1000.0;
  m_angle = caps[3].toDouble();
  m_num_spokes = caps[4].toInt();
  m_gap = caps[5].toDouble() / 1000.0;

  m_bounding = painterPath().boundingRect();
}

QPainterPath SquareThermalOpenCornersSymbol::painterPath(void)
{
  QPainterPath path;

  qreal angle_div = 360.0 / m_num_spokes;
  QPainterPath sub;
  QMatrix mat;

  // From what we seen in Genesis 2000, num_spokes can only be 1, 2, 4
  // angle can only be multiple of 45
  if ((m_num_spokes != 1 && m_num_spokes != 2 && m_num_spokes != 4) ||
      ((int)m_angle % 45 != 0)) {
    return path;
  }

  path.addRect(-m_od / 2, -m_od / 2, m_od, m_od);
  path.addRect(-m_id / 2, -m_id / 2, m_id, m_id);

  if ((int)m_angle % 90 == 0) {
    QPainterPath bar;
    bar.addRect(0, -m_gap / 2, m_od, m_gap);

    for (int i = 0; i < m_num_spokes; ++i) {
      sub.addPath(mat.map(bar));
      mat.rotate(-angle_div);
    }
  } else {
    qreal side = m_gap * qCos(M_PI / 4) + (m_od - m_id) / 2;
    qreal offset = (m_od - side) / 2;

    QPainterPath box;
    box.addRect(-side / 2, -side / 2, side, side);

    for (int i = 0; i < m_num_spokes; ++i) {
      QMatrix mat;
      mat.translate(offset * sign(qCos((m_angle + angle_div * i) * D2R)),
                    -offset * sign(qSin((m_angle + angle_div * i) * D2R)));
      sub.addPath(mat.map(box));
    }
  }

  path = path.subtracted(sub);

  return path;
}
