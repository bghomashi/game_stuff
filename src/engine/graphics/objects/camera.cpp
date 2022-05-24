
#include "engine/graphics/objects/camera.h"
#include "engine/graphics/graphics.h"


void Camera::Draw() const {
    OGL::MatrixStack::TranslateModelView(-position.x, -position.y, 0.f);
}
void Camera::SetViewport() const {
    OGL::MatrixStack::OrthoProjection(0.0, viewport.width, 0.0, viewport.height, -1.0, 1.0);
    OGL::Device::SetViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}