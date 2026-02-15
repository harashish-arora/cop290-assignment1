#include "shapes/text_shape.h"

#include <QFont>
#include <QFontMetricsF>
#include <QPainterPath>

TextShape::TextShape(double x, double y, const std::string& text)
    : x(x), y(y), text(text) {
  fillColor = "#280078d4";
  strokeColor = "#ff000000";
  strokeWidth = 1.0;
}

void TextShape::draw(QPainter& painter) const {
  QFont f(QString::fromStdString(fontFamily), fontSize);
  painter.setFont(f);

  // Persistent background highlight from fill color
  QColor bg(fillColor.c_str());
  if (bg.isValid() && fillColor != "transparent" && fillColor != "none") {
    painter.setPen(Qt::NoPen);
    painter.setBrush(bg);
    painter.drawRect(boundingBox());
  }

  QColor c(strokeColor.c_str());
  if (!c.isValid() || strokeColor == "transparent" || strokeColor == "none") {
    c = Qt::black;
  }
  QString qText = QString::fromStdString(text);
  QPainterPath path;
  path.addText(QPointF(x, y), f, qText);

  painter.setPen(Qt::NoPen);
  painter.setBrush(c);
  painter.drawPath(path);

  if (strokeWidth > 1.0) {
    QPen p(c);
    p.setWidthF(strokeWidth - 1.0);
    p.setJoinStyle(Qt::RoundJoin);
    painter.strokePath(path, p);
  }
}

QRectF TextShape::boundingBox() const {
  QFont f(QString::fromStdString(fontFamily), fontSize);
  QFontMetricsF fm(f);

  QString t = QString::fromStdString(text.empty() ? " " : text);
  double w = fm.horizontalAdvance(t);
  double h = fm.height();
  return QRectF(x, y - fm.ascent(), w, h);
}

bool TextShape::contains(double px, double py) const {
  return boundingBox().contains(px, py);
}

std::string TextShape::escapeXml(const std::string& s) {
  std::string out;
  out.reserve(s.size());
  for (char ch : s) {
    switch (ch) {
      case '&':
        out += "&amp;";
        break;
      case '<':
        out += "&lt;";
        break;
      case '>':
        out += "&gt;";
        break;
      case '"':
        out += "&quot;";
        break;
      case '\'':
        out += "&apos;";
        break;
      default:
        out += ch;
        break;
    }
  }
  return out;
}

std::string TextShape::toSVG() const {
  return "<text data-shape=\"text\" x=\"" + std::to_string(x) + "\" y=\"" +
         std::to_string(y) + "\" font-family=\"" + escapeXml(fontFamily) +
         "\" font-size=\"" + std::to_string(fontSize) +
         "\" data-highlight-fill=\"" + escapeXml(fillColor) + "\" " +
         svgColorAttr("fill", strokeColor) + " stroke-width=\"" +
         std::to_string(strokeWidth) + "\">" + escapeXml(text) + "</text>";
}

void TextShape::moveBy(double dx, double dy) {
  x += dx;
  y += dy;
}

std::shared_ptr<GraphicsObject> TextShape::clone() const {
  auto copy = std::make_shared<TextShape>(x, y, text);
  copy->fontFamily = fontFamily;
  copy->fontSize = fontSize;
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

void TextShape::setFromBoundingBox(const QRectF& box) {
  QFont f(QString::fromStdString(fontFamily), fontSize);
  QFontMetricsF fm(f);
  x = box.x();
  y = box.y() + fm.ascent();
}

void TextShape::setText(const std::string& value) { text = value; }
const std::string& TextShape::getText() const { return text; }

void TextShape::setFontFamily(const std::string& family) {
  fontFamily = family;
}
const std::string& TextShape::getFontFamily() const { return fontFamily; }

void TextShape::setFontSize(int size) {
  if (size < 6) size = 6;
  if (size > 200) size = 200;
  fontSize = size;
}
int TextShape::getFontSize() const { return fontSize; }
