# The Backrooms - Unreal Engine 5 (Enhanced Graphics)

A premium horror exploration game set in The Backrooms. Experience procedurally generated nightmare levels with AAA-quality graphics, realistic lighting, and terrifying AI.

## 🎨 Enhanced Graphics Features

### Environment
- **Procedural Mesh Generation**: Real-time geometry creation for infinite level variety
- **Dynamic Lighting**: Fluorescent fixtures with realistic attenuation and shadows
- **Material System**: 
  - Damp carpet with micro-detail roughness
  - Decaying wallpaper with pattern overlay
  - Concrete surfaces with weathering
  - Realistic light scattering

### Entities (Highly Detailed)
- **Wretch**: Tall, lanky silhouette with glowing eyes in darkness
- **Hound**: Low, aggressive quadrupedal form with reflective eyes
- **Smiler**: Unsettling figure with wide grin, semi-transparent when not chasing

### Visual Effects
- **Vignette Darkening**: Edges darken as sanity decreases
- **Screen Shake**: Intensity scales with proximity to entities
- **Sanity Distortion**: Chromatic aberration and blur when sanity is critical
- **Flash Effects**: Bright transitions between levels
- **Ambient Occlusion**: Enhanced depth perception in corridors

### Lighting System
- **Fluorescent Lights**: Placed procedurally every 5 grid units
- **Warm Color Temperature**: 2400-2700K for eerie atmosphere (0.9, 0.85, 0.7)
- **Realistic Shadows**: Ray-traced shadows from light fixtures
- **Flicker Effects**: Occasional light flicker for horror ambiance
- **Color Grading**: Level-specific color palettes

## 📂 New Components

### BackroomsEnvironment
```cpp
- GenerateWalls()       // 3D procedural wall mesh
- GenerateFloor()       // Walkable surface with proper collision
- GenerateCeiling()     // Ceiling geometry and lighting grid
- ApplyMaterials()      // Dynamic material instances per level
- AddWallpaper()        // Level-specific surface details
- AddFluorescentLighting() // Dynamic light placement
```

### BackroomsEntity (Upgraded)
```cpp
- Enhanced AI detection (vision cone, hearing radius)
- Skeletal mesh support for animations
- Entity-specific behavior trees
- Line-of-sight checks
- Teleportation logic (Smiler)
```

### BackroomsPostProcessing
```cpp
- Screen shake with intensity scaling
- Vignette effects based on sanity
- Flash transitions
- Sanity-based visual distortion
```

## 🎯 Level Visual Themes

### Level 0 - Yellow Halls
- Warm yellow wallpaper (RGB: 0.53, 0.46, 0.22)
- Subtle diamond pattern texture
- High fluorescent brightness
- Single ominous silhouette (Wretch)

### Level 1 - Dark Corridors  
- Cold gray concrete (RGB: 0.23, 0.23, 0.23)
- Harsh fluorescent lighting
- Multiple hunting entities (Hounds)
- Reduced ambient light

### Level 2 - The Pipes
- Rust-stained brown (RGB: 0.29, 0.16, 0.10)
- Vertical pipe structures
- Flickering lights
- Narrow corridors

### Level 5 - Blood Level
- Deep purple walls (RGB: 0.16, 0.08, 0.12)
- Minimal lighting
- Smiling entities in shadows
- Sanity effects intensify

### Level 8 - The Void
- Almost pitch black (RGB: 0.06, 0.06, 0.08)
- Only flashlight reveals paths
- Multiple Smilers
- Extreme sanity drain

## 🔧 Graphics Settings

| Setting | Value | Notes |
|---------|-------|-------|
| Mesh LOD | 0 | Full detail |
| Shadow Quality | High | Ray-traced |
| Light Count | Dynamic | Up to 520 lights (26×20 grid) |
| Material Complexity | 3 layers | Albedo, Roughness, Normal |
| Post-Process | Full | Vignette, Shake, Distortion |
| Draw Distance | 50,000 UU | Render entire level |

## 🎬 Performance Optimization

- **Mesh Batching**: Static meshes batched per level
- **LOD System**: Distance-based detail reduction
- **Light Frustum Culling**: Only visible lights rendered
- **Procedural Generation**: Happens once per level load
- **Collision Optimization**: Static collision on environment

## Installation

1. Clone repository
2. Generate Visual Studio project files
3. Build with Development Editor configuration
4. Open in UE5 editor
5. Ensure materials are assigned in level BP

## Future Graphics Enhancements

- [ ] Nanite virtualized geometry for better LOD
- [ ] Lumen global illumination
- [ ] MetaHuman skeleton for entities
- [ ] Advanced material blending
- [ ] Ray-traced reflections
- [ ] Temporal super-sampling
