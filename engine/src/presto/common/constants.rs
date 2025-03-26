type PrNumericId = u32;

enum RenderLibrary {
    Unset = 0,
    Vulkan = 1,
    OpenGL = 2,
    DirectX = 3,
}

type PrBitType = u16;

pub const fn as_bit<T>(x: u8) -> PrBitType {
    1 << x
}

const PR_MIN_NEAR_DISTANCE: f64 = 0.1;

type MeshContextIdT = u32;

// Pipelines
type MeshRegistrationIdT = u32;
const PR_UNREGISTERED: MeshRegistrationIdT = -1;

type PipelineIdT = u16;
type MaterialIdT = u16;

const PR_MIN_USER_PIPELINE_ID: PipelineIdT = { 10 };
const PR_MIN_RUNTIME_MESH_ID: MeshRegistrationIdT = { 10 };

const PR_PIPELINE_ANY: PipelineIdT = { 0 };
const PR_PIPELINE_DEFAULT_3D: PipelineIdT = { 1 };
const PR_PIPELINE_DEFAULT_UI: PipelineIdT = { 2 };
const PR_PIPELINE_NONE: PipelineIdT = { -1 };

// Textures
type TextureIdT = u16;
const PR_DEFAULT_TEXTURE: TextureIdT = { 9 };

const PR_MESH_QUAD: MeshRegistrationIdT = { 1 };

const PR_ANY_NAME: str = "PRESTO_ANY_NAME";

const DEFAULT_FOV: f32 = 120;
