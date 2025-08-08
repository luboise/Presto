pub(crate) use crate::traits::Resize;
mod glfw_window;

pub fn create(params: WindowParams) -> Box<impl WindowLike> {
    glfw_window::GLFWWindow::from_params(params)
}

pub struct WindowParams {
    pub width: u16,
    pub height: u16,
}

pub trait WindowLike: Resize {
    fn from_params(params: WindowParams) -> Box<Self>;
}
