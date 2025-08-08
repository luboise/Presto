pub type Size2D = Size<u16>;

#[derive(Copy, Clone)]
pub struct Size<T: Copy + Clone> {
    pub width: T,
    pub height: T,
}
