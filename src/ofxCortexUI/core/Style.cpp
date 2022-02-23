#include "Style.h"

namespace ofxCortex { namespace ui {

void Style::Font::load(std::string path)
{
  ofFile fontFile(path);
  if (!fontFile.exists()) path = OF_TTF_MONO;
  
  ofTrueTypeFontSettings fontSettings(path, 8.0f);
  fontSettings.antialiased = true;
  fontSettings.contours = true;
  fontSettings.addRanges({
    ofUnicode::Latin1Supplement,
    ofUnicode::Arrows
  });
  ttf->load(fontSettings);
  
  if (ttf->isLoaded()) xHeight = ttf->stringHeight("X");
}

}}
