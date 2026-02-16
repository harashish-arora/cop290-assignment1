// svg parser cpp
// reads an svg file and returns a vector of graphicsobjects

#include "parse/svg_parser.h"

#include <fstream>
#include <sstream>

#include "parse/svg_parser_internal.h"

namespace SvgParser {

// load parses svg text and builds shape objects for supported tags
std::vector<std::shared_ptr<GraphicsObject>> load(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) return {};
  std::ostringstream ss;
  ss << file.rdbuf();
  std::string content = ss.str();

  std::vector<std::shared_ptr<GraphicsObject>> shapes;
  size_t pos = 0;
  while (pos < content.size()) {
    // scan next tag and parse attribute list
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
    auto attrs = parseAttributes(tag.substr(nameEnd));

    // dispatch by tag name to specialized parsers
    if (tagName == "rect") {
      parseRect(attrs, shapes);
    } else if (tagName == "circle") {
      parseCircle(attrs, shapes);
    } else if (tagName == "ellipse") {
      parseEllipse(attrs, shapes);
    } else if (tagName == "line") {
      parseLine(attrs, shapes);
    } else if (tagName == "text") {
      size_t tc = content.find("</text>", pos);
      if (tc == std::string::npos) continue;
      std::string raw = content.substr(pos, tc - pos);
      pos = tc + 7;
      parseText(attrs, raw, shapes);
    } else if (tagName == "polyline") {
      parsePolyline(attrs, shapes);
    } else if (tagName == "polygon") {
      parsePolygon(attrs, shapes);
    }
  }
  return shapes;
}

}  // namespace svgparser
