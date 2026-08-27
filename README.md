# The Backrooms - Unreal Engine 5

A horror exploration game set in The Backrooms. Escape through procedurally generated levels while being hunted by mysterious entities.

## Features

- **Procedural Level Generation**: Each level is randomly generated with a different theme
- **Three Entity Types**: Wretch, Hound, and Smiler with unique behaviors
- **Sanity System**: Mental health deteriorates in darkness, recovered partially on level progression
- **Battery System**: Flashlight requires power management
- **5 Progressive Levels**: Increasing difficulty and atmosphere
- **Audio Design**: Procedurally generated ambient sound
- **First-Person Horror**: Immersive FPS perspective with flashlight mechanics

## Project Structure

```
Source/Backrooms/
├── Public/
│   ├── Core/
│   │   └── BackroomsGameMode.h
│   ├── Player/
│   │   └── BackroomsPlayerCharacter.h
│   ├── Level/
│   │   └── BackroomsLevelGenerator.h
│   └── AI/
│       └── BackroomsEntity.h
├── Private/
│   ├── Core/
│   ├── Player/
│   ├── Level/
│   └── AI/
└── Backrooms.Build.cs
```

## Level Themes

### Level 0 - The Yellow Halls
- Damp yellow carpet and wallpaper
- Single Wretch entity
- Highest ambient light

### Level 1 - The Dark Corridors
- Concrete walls and floors
- 2 Hound entities
- Lower light levels

### Level 2 - The Pipe Maze
- Rust and decay
- 2 Hound entities
- Increasingly dark

### Level 5 - The Blood Level
- Dark purple walls
- 2 Smiler entities
- Very low light

### Level 8 - The Void
- Nearly black environment
- 3 Smiler entities
- Minimal ambient light

## Controls

- **WASD**: Move
- **Mouse**: Look around / Aim flashlight
- **Shift**: Sprint
- **F**: Toggle flashlight

## Gameplay Mechanics

### Sanity
- Starts at 100%
- Decreases in darkness (3.2 per second)
- Flashlight reduces drain to 35%
- Recovered partially (+18) when progressing levels
- Game over when reaching 0%

### Battery
- Starts at 100%
- Drains while flashlight is on (1.15 per second, +0.4 while sprinting)
- Recharges when off (2.2 per second)
- Cannot toggle flashlight when battery ≤ 2%

### Entities
- **Wretch**: Sees through vision, requires line of sight
- **Hound**: Hears movement, senses by proximity
- **Smiler**: Triggered by direct flashlight illumination

## Installation & Building

1. Clone the repository
2. Right-click `.uproject` file → Generate Visual Studio project files
3. Open solution in Visual Studio
4. Build and launch in Unreal Editor

## Future Enhancements

- [ ] Advanced AI behavior trees
- [ ] Better procedural mesh generation
- [ ] Dynamic lighting improvements
- [ ] Audio implementation (ambience, footsteps, entity sounds)
- [ ] UI/HUD system
- [ ] Save/load functionality
- [ ] Difficulty settings
