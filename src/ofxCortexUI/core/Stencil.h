#pragma once

namespace ofxCortex { namespace ui {

struct Stencil {
  static void beginDrawingMask()
  {
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Disable drawing colors to the screen
    glDepthMask(GL_FALSE); //Disable drawing to depth buffer
    glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
                                    //Make pixels in the stencil buffer be set to 1 when the stencil test passes
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  }
  
  static void endDrawingMask()
  {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Enable drawing colors to the screen
    glDepthMask(GL_TRUE); //Eable drawing to depth buffer
  }
  
  static void beginUsingMask()
  {
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  }
  
  static void endUsingMask()
  {
    glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
  }
 
protected:
  Stencil() = default;
  
};

}}
