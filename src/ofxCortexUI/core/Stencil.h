#pragma once

namespace ofxCortex { namespace ui {

struct Stencil {
  static void beginDrawingMask()
  {
//    glClearStencil(0);
//    glClear(GL_STENCIL_BUFFER_BIT);
//    glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
//    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Disable drawing colors to the screen
//    glDepthMask(GL_FALSE); //Disable drawing to depth buffer
//    glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
//                                    //Make pixels in the stencil buffer be set to 1 when the stencil test passes
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    glClearColor(0, 0, 0, 1);
    glClearStencil(0);
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Do not draw any pixels on the back buffer
    glEnable(GL_STENCIL_TEST); // Enables testing AND writing functionalities
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Do not test the current value in the stencil buffer, always accept any value on there for drawing
    glStencilMask(0xFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); // Make every test succeed
  }
  
  static void endDrawingMask()
  {
//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Enable drawing colors to the screen
//    glDepthMask(GL_TRUE); //Eable drawing to depth buffer
    
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Make sure you will no longer (over)write stencil values, even if any test succeeds
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Make sure we draw on the backbuffer again.
  }
  
  static void beginUsingMask()
  {
//    glStencilFunc(GL_EQUAL, 1, 1);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Now we will only draw pixels where the corresponding stencil buffer
  }
  
  static void endUsingMask()
  {
    glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
  }
 
protected:
  Stencil() = default;
  
};

}}
