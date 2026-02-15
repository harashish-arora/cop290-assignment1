#include "parse/svg_parser.h"

#include <QColor>
#include <fstream>
#include <map>
#include <sstream>

#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

namespace SvgParser {

// Extract all key="value" pairs from a tag string like:
static std::map<std::string, std::string> parseAttributes(
    const std::string& tag) {
  std::map<std::string, std::string> attrs;
  size_t pos = 0;
  while (pos < tag.size()) {
    while (pos < tag.size() && (tag[pos] == ' ' || tag[pos] == '/')) pos++;

    size_t eq = tag.find('=', pos);
    if (eq == std::string::npos) break;

    std::string key = tag.substr(pos, eq - pos);

    while (!key.empty() && key.back() == ' ') key.pop_back();

    size_t q1 = tag.find('"', eq);
    if (q1 == std::string::npos) break;
    q1++;

    size_t q2 = tag.find('"', q1);
    if (q2 == std::string::npos) break;

    attrs[key] = tag.substr(q1, q2 - q1);
    pos = q2 + 1;
  }
  return attrs;
}

static double num(const std::map<std::string, std::string>& m,
                  const std::string& key, double fallback = 0.0) {
  auto it = m.find(key);
  if (it == m.end()) return fallback;
  try {
    return std::stod(it->second);
  } catch (...) {
    return fallback;
  }
}

static std::string str(const std::map<std::string, std::string>& m,
                       const std::string& key,
                       const std::string& fallback = "") {
  auto it = m.find(key);
  return (it != m.end()) ? it->second : fallback;
}

static std::string unescapeXml(std::string s) {
  auto replaceAll = [&](const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
      s.replace(pos, from.size(), to);
      pos += to.size();
    }
  };
  replaceAll("&lt;", "<");
  replaceAll("&gt;", ">");
  replaceAll("&quot;", "\"");
  replaceAll("&apos;", "'");
  replaceAll("&amp;", "&");
  return s;
}

// Reconstruct a QColor-compatible string from SVG color + opacity.
static std::string rebuildColor(const std::map<std::string, std::string>& m,
                                const std::string& prefix) {
  std::string colorStr = str(m, prefix, "black");
  if (colorStr == "none") return "transparent";

  double opacity = num(m, prefix + "-opacity", 1.0);

  QColor c(colorStr.c_str());
  if (!c.isValid()) return colorStr;

  c.setAlphaF(opacity);
  return c.name(QColor::HexArgb).toStdString();
}

std::vector<std::shared_ptr<GraphicsObject>> load(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) return {};
  std::ostringstream ss;
  ss << file.rdbuf();
  std::string content = ss.str();

  std::vector<std::shared_ptr<GraphicsObject>> shapes;

  size_t pos = 0;
  while (pos < content.size()) {
    size_t open = content.find('<', pos);
    if (open == std::string::npos) break;

    size_t close = content.find('>', open);
    if (close == std::string::npos) break;

    std::string tag = content.substr(open + 1, close - open - 1);
    pos = close + 1;

    if (tag.empty() || tag[0] == '/' || tag[0] == '?' || tag[0] == '!')
      continue;

    size_t nameEnd = tag.find_first_of(" />");
    std::string tagName = tag.substr(0, nameEnd);

    if (tagName == "rect") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      double x = num(attrs, "x");
      double y = num(attrs, "y");
      double w = num(attrs, "width");
      double h = num(attrs, "height");
      double rrx = num(attrs, "rx");
      double rry = num(attrs, "ry");
      if (rrx > 0 || rry > 0) {
        auto rr = std::make_shared<RoundedRectangle>(x, y, w, h, rrx, rry);
        rr->setFillColor(rebuildColor(attrs, "fill"));
        rr->setStrokeColor(rebuildColor(attrs, "stroke"));
        rr->setStrokeWidth(num(attrs, "stroke-width", 1.0));
        shapes.push_back(rr);
      } else {
        auto rect = std::make_shared<Rectangle>(x, y, w, h);
        rect->setFillColor(rebuildColor(attrs, "fill"));
        rect->setStrokeColor(rebuildColor(attrs, "stroke"));
        rect->setStrokeWidth(num(attrs, "stroke-width", 1.0));
        shapes.push_back(rect);
      }

    } else if (tagName == "circle") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      double cx = num(attrs, "cx");
      double cy = num(attrs, "cy");
      double r = num(attrs, "r");
      auto circle = std::make_shared<Circle>(cx, cy, r);
      circle->setFillColor(rebuildColor(attrs, "fill"));
      circle->setStrokeColor(rebuildColor(attrs, "stroke"));
      circle->setStrokeWidth(num(attrs, "stroke-width", 1.0));
      shapes.push_back(circle);

    } else if (tagName == "ellipse") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      double cx = num(attrs, "cx");
      double cy = num(attrs, "cy");
      double erx = num(attrs, "rx");
      double ery = num(attrs, "ry");
      auto ellipse = std::make_shared<Circle>(cx, cy, erx, ery);
      ellipse->setFillColor(rebuildColor(attrs, "fill"));
      ellipse->setStrokeColor(rebuildColor(attrs, "stroke"));
      ellipse->setStrokeWidth(num(attrs, "stroke-width", 1.0));
      shapes.push_back(ellipse);

    } else if (tagName == "line") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      double lx1 = num(attrs, "x1");
      double ly1 = num(attrs, "y1");
      double lx2 = num(attrs, "x2");
      double ly2 = num(attrs, "y2");
      auto line = std::make_shared<Line>(lx1, ly1, lx2, ly2);
      line->setFillColor(rebuildColor(attrs, "fill"));
      line->setStrokeColor(rebuildColor(attrs, "stroke"));
      line->setStrokeWidth(num(attrs, "stroke-width", 1.0));
      shapes.push_back(line);

    } else if (tagName == "text") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      size_t textClose = content.find("</text>", pos);
      if (textClose == std::string::npos) continue;
      std::string rawText = content.substr(pos, textClose - pos);
      pos = textClose + 7;

      double tx = num(attrs, "x");
      double ty = num(attrs, "y");
      auto text = std::make_shared<TextShape>(tx, ty, unescapeXml(rawText));

      // Visible text color: prefer stroke, fallback to fill (legacy files)
      std::string strokeParsed = rebuildColor(attrs, "stroke");
      std::string fillParsed = rebuildColor(attrs, "fill");
      bool hasStroke = attrs.find("stroke") != attrs.end();
      text->setStrokeColor(hasStroke ? strokeParsed : fillParsed);

      // Highlight color for edit mode
      std::string highlight = str(attrs, "data-highlight-fill");
      text->setFillColor(highlight.empty() ? fillParsed : highlight);

      text->setStrokeWidth(num(attrs, "stroke-width", 1.0));
      std::string ff = str(attrs, "font-family");
      if (!ff.empty()) text->setFontFamily(ff);
      int fs = static_cast<int>(num(attrs, "font-size", 16.0));
      text->setFontSize(fs);
      shapes.push_back(text);

    } else if (tagName == "polyline") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      if (str(attrs, "data-shape") == "freehand") {
        auto fh = std::make_shared<Freehand>();
        std::string ptsStr = str(attrs, "points");
        std::istringstream iss(ptsStr);
        std::string pair;
        while (iss >> pair) {
          size_t comma = pair.find(',');
          if (comma != std::string::npos) {
            double px = std::stod(pair.substr(0, comma));
            double py = std::stod(pair.substr(comma + 1));
            fh->addPoint(px, py);
          }
        }
        fh->setStrokeColor(rebuildColor(attrs, "stroke"));
        fh->setStrokeWidth(num(attrs, "stroke-width", 1.0));
        shapes.push_back(fh);
      }

    } else if (tagName == "polygon") {
      auto attrs = parseAttributes(tag.substr(nameEnd));
      std::string shapeType = str(attrs, "data-shape");
      if (shapeType == "hexagon") {
        double hcx = num(attrs, "data-cx");
        double hcy = num(attrs, "data-cy");
        double hrx = num(attrs, "data-rx");
        double hry = num(attrs, "data-ry");
        auto hex = std::make_shared<Hexagon>(hcx, hcy, hrx, hry);
        if (str(attrs, "data-orientation") == "pointy") hex->setPointyTop(true);
        hex->setFillColor(rebuildColor(attrs, "fill"));
        hex->setStrokeColor(rebuildColor(attrs, "stroke"));
        hex->setStrokeWidth(num(attrs, "stroke-width", 1.0));
        shapes.push_back(hex);
      }
    }
  }

  return shapes;
}

}  // namespace SvgParser
