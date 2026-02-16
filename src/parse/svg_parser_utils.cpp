// svg parser utils cpp
// implementation for svg parser utils

#include <QColor>

#include "parse/svg_parser_internal.h"

namespace SvgParser {

// parse key value attributes from one xml tag text
AttrMap parseAttributes(const std::string& tag) {
  AttrMap attrs;
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

// parse number attribute with fallback on missing or invalid values
double num(const AttrMap& m, const std::string& key, double fallback) {
  auto it = m.find(key);
  if (it == m.end()) return fallback;
  try {
    return std::stod(it->second);
  } catch (...) {
    return fallback;
  }
}

// parse string attribute with fallback
std::string str(const AttrMap& m, const std::string& key,
                const std::string& fallback) {
  auto it = m.find(key);
  return (it != m.end()) ? it->second : fallback;
}

// convert escaped xml entities back to plain text
std::string unescapeXml(std::string s) {
  auto rep = [&](const std::string& from, const std::string& to) {
    size_t p = 0;
    while ((p = s.find(from, p)) != std::string::npos) {
      s.replace(p, from.size(), to);
      p += to.size();
    }
  };
  rep("&lt;", "<");
  rep("&gt;", ">");
  rep("&quot;", "\"");
  rep("&apos;", "'");
  rep("&amp;", "&");
  return s;
}

// rebuild color with alpha from color and opacity attributes
std::string rebuildColor(const AttrMap& m, const std::string& prefix) {
  std::string colorStr = str(m, prefix, "black");
  if (colorStr == "none") return "transparent";
  double opacity = num(m, prefix + "-opacity", 1.0);
  QColor c(colorStr.c_str());
  if (!c.isValid()) return colorStr;
  c.setAlphaF(opacity);
  return c.name(QColor::HexArgb).toStdString();
}

}  // namespace svgparser
