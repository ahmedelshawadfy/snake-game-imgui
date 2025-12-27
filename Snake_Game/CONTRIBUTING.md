# Contributing to Snake Game UE5

## Project Overview

This project is a migration of the original ImGui-based Snake Game to Unreal Engine 5 using Paper2D for 2D rendering. The project demonstrates best practices for UE5 development, actor-based architecture, and event-driven gameplay systems.

## Development Environment Setup

### Prerequisites
- **Unreal Engine 5.4+** (latest stable version recommended)
- **Visual Studio 2022** with C++ workload installed
- **Git** for version control
- **Windows 10/11** (development target platform)

### Folder Structure

```
snake-game-ue5/
??? .github/
?   ??? workflows/
??? .gitignore
??? .gitattributes
??? README.md
??? CONTRIBUTING.md
??? LICENSE
??? Source/
?   ??? SnakeGame/
?   ?   ??? Public/
?   ?   ?   ??? SnakeGameMode.h
?   ?   ?   ??? SnakePawn.h
?   ?   ?   ??? SnakeSegment.h
?   ?   ?   ??? FoodActor.h
?   ?   ?   ??? ObstacleActor.h
?   ?   ??? Private/
?   ?       ??? SnakeGameMode.cpp
?   ?       ??? SnakePawn.cpp
?   ?       ??? SnakeSegment.cpp
?   ?       ??? FoodActor.cpp
?   ?       ??? ObstacleActor.cpp
?   ??? SnakeGame.Build.cs
?   ??? SnakeGameEditor.Target.cs
?   ??? SnakeGameClient.Target.cs
??? Content/
?   ??? Maps/
?   ?   ??? GameMap.umap
?   ??? Sprites/
?   ?   ??? SnakeSegment.uasset
?   ?   ??? Food.uasset
?   ?   ??? Obstacle.uasset
?   ??? UI/
?   ?   ??? HUD_Main.uasset
?   ?   ??? HUD_GameOver.uasset
?   ??? Materials/
??? Docs/
?   ??? IDE_SETUP_GUIDE.md
?   ??? ENGINE_COMPARISON.md
??? Binaries/ (git ignored)
??? Intermediate/ (git ignored)
??? Saved/ (git ignored)
??? SnakeGame.uproject
??? SnakeGame.sln (generated)
```

## C++ Coding Standards

### Naming Conventions
- **Classes**: PascalCase with `A` prefix for Actors, `U` for Objects (e.g., `ASnakePawn`, `USnakeComponent`)
- **Functions**: PascalCase (e.g., `MoveSnake()`, `CheckCollisions()`)
- **Variables**: camelCase for locals (e.g., `moveDelay`), with `b` prefix for booleans (e.g., `bIsMoving`)
- **Member variables**: Follow Unreal convention with `UPROPERTY()` macro
- **Constants**: CONSTANT_CASE (e.g., `DEFAULT_GRID_WIDTH`)

### Unreal-Specific Standards
- Use `UCLASS()`, `UFUNCTION()`, `UPROPERTY()` macros for reflection
- Always use `UPROPERTY()` with appropriate specifiers:
  - `EditAnywhere` - editable in editor and instances
  - `VisibleAnywhere` - visible but not editable
  - `BlueprintReadOnly` / `BlueprintReadWrite` - blueprint accessible
  - `Category = "Section"` - organize properties
- Use `nullptr` instead of `NULL` or `0`
- Use smart pointers when necessary (`TSharedPtr`, `TUniquePtr`)
- Prefer `AActor::Tick()` for frame updates
- Use `UCLASS()` and `GENERATED_BODY()` macros

### Code Style
- Indentation: 4 spaces (Visual Studio default)
- Line length: 120 characters maximum
- Braces: Allman style for multi-line statements
- Comments: Use `//` for single-line, `/* */` for multi-line blocks
- Always include `IWYU` (Include What You Use) headers only

### Documentation
- Add comment headers to public functions and properties
- Document `UPROPERTY()` values with inline comments
- Use descriptive variable names over cryptic abbreviations
- Include `UPROPERTY()` tooltips for clarity

## Game Architecture

### Core Systems
1. **ASnakeGameMode** - Manages game state, scoring, and win/loss conditions
2. **ASnakePawn** - Handles snake entity and input
3. **ASnakeSegment** - Individual snake segment visual representation
4. **AFoodActor** - Food spawning and consumption logic
5. **AObstacleActor** - Moving obstacle behavior and collisions

### Key Design Patterns
- **Event-Driven Architecture**: Use `DECLARE_DYNAMIC_MULTICAST_DELEGATE` for loose coupling
- **Actor-Based Design**: Leverage Unreal's Actor system for game entities
- **Tick-Based Updates**: Use `Tick()` for frame-based logic
- **Grid-Based Movement**: Convert world coordinates to grid positions

### Input Handling
- Enhanced Input System (UE5 standard)
- Keyboard inputs: Arrow keys, WASD
- Game controls: Space (Start/Pause), R (Reset)
- Configured in `DefaultInput.ini` and Input Mappings

## Build and Compilation

### Generating Visual Studio Files
```bash
# From project directory
.\GenerateProjectFiles.bat

# Or right-click .uproject -> Generate Visual Studio project files
```

### Building
- **From Visual Studio**: `Build > Build Solution` (F7)
- **From Editor**: `Tools > Compile` (Ctrl+Alt+F11)
- **Command Line**: Use `Build.bat` from Engine directory

### First-Time Setup
1. Generate Visual Studio files via `.uproject` context menu
2. Open `.sln` in Visual Studio 2022
3. Set `SnakeGameEditor` as startup project
4. Press F5 to build and launch

## Testing Guidelines

### Gameplay Tests
- ? All four movement directions work
- ? Snake cannot reverse into itself
- ? Food is consumed and score increases by 10
- ? Food spawns at valid locations (not on snake/obstacle)
- ? Obstacle moves randomly and wraps boundaries
- ? Collision detection:
  - Wall collision ends game
  - Obstacle collision ends game
  - Self-collision ends game
- ? Game pause/resume functionality
- ? Game reset clears state properly

### Performance Targets
- 60 FPS on development builds
- Minimal memory footprint
- Paper2D optimizations applied
- No frame rate drops during gameplay

## Asset Guidelines

### Sprites (32x32 pixels)
- **Snake Segment**: Green `#00FF00` with grid outline
- **Food**: Red `#FF0000` with rounded corners
- **Obstacle**: Orange `#FF8000` with yellow border
- Format: PNG with transparent background

### Materials
- Use Paper2D material slots
- Keep shaders simple for 2D performance
- Support sprite color modifications via material parameters

## Git Workflow

### Branch Strategy
- `main` - Stable, playable build
- `develop` - Integration branch for features
- `feature/` - Feature development
- `bugfix/` - Bug fixes

### Commit Standards
- Present tense: "Add snake movement" not "Added snake movement"
- Reference issues: "Fix #42 - collision detection"
- Scope to single feature/fix per commit
- Exclude generated files, binaries, intermediates

### Pull Request Process
1. Create feature branch from `develop`
2. Implement feature with tests
3. Submit PR with clear description
4. Await code review
5. Merge to `develop`, then to `main` on release

## Documentation

### Required Documents
- **README.md** - Quick start and overview
- **IDE_SETUP_GUIDE.md** - Development environment setup
- **ENGINE_COMPARISON.md** - ImGui vs UE5 differences
- **CONTRIBUTING.md** - This file
- Inline code comments for complex logic

### Comment Requirements
- Public API functions must have summary comments
- Complex algorithms need explanation comments
- Avoid obvious comments ("increment counter")
- Update comments when changing code

## Troubleshooting

### Common Issues

**Compilation Errors**
- Verify `.Build.cs` has all required modules
- Ensure macros are in correct header format
- Check for circular includes

**Editor Crashes**
- Delete `Intermediate/` and `Saved/` folders
- Regenerate Visual Studio files
- Update to latest engine version

**Input Not Responding**
- Verify input mappings in Project Settings
- Check `AutoPossessPlayer` setting in Pawn class
- Confirm Enhanced Input plugin is enabled

## Questions?

Refer to:
- [Unreal Engine 5 Documentation](https://docs.unrealengine.com/)
- [Paper2D Guide](https://docs.unrealengine.com/5.0/en-US/paper2d-in-unreal-engine/)
- Project README.md for feature status
- GitHub Issues for bug reports