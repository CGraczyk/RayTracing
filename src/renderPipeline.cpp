#include "renderPipeline.hpp"

void runRenderer(RenderFunction renderFunc) {
    Canvas canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Call the user-provided function to set pixel values
    renderFunc(canvas);
    
    // Render the final result
    canvas.render();
}