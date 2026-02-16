// svg_parser_shapes.cpp
// implementation for parsing SVG shapes into GraphicsObjects
// implementations of the freehand and polygon are parsed when
// they contain the tags

#include <sstream>

#include "parse/svg_parser_internal.h"
#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

namespace SvgParser {

// apply common style attributes like fill, stroke and stroke-width to a shape
static void applyStyle(const std::shared_ptr<GraphicsObject>& s,
                       const AttrMap& a) {
  s->setFillColor(rebuildColor(a, "fill"));
  s->setStrokeColor(rebuildColor(a, "stroke"));
  s->setStrokeWidth(num(a, "stroke-width", 1.0));
}

// parses a rect tag, creating either a Rectangle or RoundedRectangle
// depending on presence of rx/ry attributes, and applies styles
void parseRect(const AttrMap& a, ShapeVec& out) {
  double x = num(a, "x"), y = num(a, "y");
  double w = num(a, "width"), h = num(a, "height");
  double rrx = num(a, "rx"), rry = num(a, "ry");
  if (rrx > 0 || rry > 0) {
    auto s = std::make_shared<RoundedRectangle>(x, y, w, h, rrx, rry);
    applyStyle(s, a);
    out.push_back(s);
  } else {
    auto s = std::make_shared<Rectangle>(x, y, w, h);
    applyStyle(s, a);
    out.push_back(s);
  }
}

// parse circle tag
void parseCircle(const AttrMap& a, ShapeVec& out) {
  auto s = std::make_shared<Circle>(num(a, "cx"), num(a, "cy"), num(a, "r"));
  applyStyle(s, a);
  out.push_back(s);
}

// parse ellipse tag into circle class with separate rx ry
void parseEllipse(const AttrMap& a, ShapeVec& out) {
  auto s = std::make_shared<Circle>(num(a, "cx"), num(a, "cy"), num(a, "rx"),
                                    num(a, "ry"));
  applyStyle(s, a);
  out.push_back(s);
}

// parse line tag
void parseLine(const AttrMap& a, ShapeVec& out) {
  auto s = std::make_shared<Line>(num(a, "x1"), num(a, "y1"), num(a, "x2"),
                                  num(a, "y2"));
  applyStyle(s, a);
  out.push_back(s);
}

// parse text tag and preserve font and fill details
void parseText(const AttrMap& a, const std::string& raw, ShapeVec& out) {
  auto s =
      std::make_shared<TextShape>(num(a, "x"), num(a, "y"), unescapeXml(raw));
  std::string sp = rebuildColor(a, "stroke");
  std::string fp = rebuildColor(a, "fill");
  bool hasStroke = a.find("stroke") != a.end();
  s->setStrokeColor(hasStroke ? sp : fp);
  std::string hl = str(a, "data-highlight-fill");
  s->setFillColor(hl.empty() ? fp : hl);
  s->setStrokeWidth(num(a, "stroke-width", 1.0));
  std::string ff = str(a, "font-family");
  if (!ff.empty()) s->setFontFamily(ff);
  s->setFontSize(static_cast<int>(num(a, "font-size", 16.0)));
  out.push_back(s);
}

// parse polyline when tagged as freehand
void parsePolyline(const AttrMap& a, ShapeVec& out) {
  if (str(a, "data-shape") != "freehand") return;
  auto fh = std::make_shared<Freehand>();
  std::istringstream iss(str(a, "points"));
  std::string pair;
  while (iss >> pair) {
    size_t comma = pair.find(',');
    if (comma != std::string::npos) {
      fh->addPoint(std::stod(pair.substr(0, comma)),
                   std::stod(pair.substr(comma + 1)));
    }
  }
  fh->setStrokeColor(rebuildColor(a, "stroke"));
  fh->setStrokeWidth(num(a, "stroke-width", 1.0));
  out.push_back(fh);
}

// parse polygon when tagged as hexagon
void parsePolygon(const AttrMap& a, ShapeVec& out) {
  if (str(a, "data-shape") != "hexagon") return;
  auto h = std::make_shared<Hexagon>(num(a, "data-cx"), num(a, "data-cy"),
                                     num(a, "data-rx"), num(a, "data-ry"));
  if (str(a, "data-orientation") == "pointy") h->setPointyTop(true);
  h->setFillColor(rebuildColor(a, "fill"));
  h->setStrokeColor(rebuildColor(a, "stroke"));
  h->setStrokeWidth(num(a, "stroke-width", 1.0));
  out.push_back(h);
}

}  // namespace SvgParser
