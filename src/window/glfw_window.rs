use crate::{traits::Resize, types::Size2D, window::WindowLike};

pub(super) struct GLFWWindow {
    size: Size2D,
}

impl WindowLike for GLFWWindow {
    fn from_params(params: super::WindowParams) -> Box<Self> {
        Box::new(GLFWWindow {
            size: Size2D {
                width: params.width,
                height: params.height,
            },
        })
    }
}

impl Resize for GLFWWindow {
    fn set_size<S: Into<Size2D>>(&mut self, new_size: Size2D) {
        self.size = new_size;
    }

    fn get_size(&self) -> Size2D {
        return self.size;
    }
}
