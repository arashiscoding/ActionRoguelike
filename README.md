<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Action Rogue-Like</h3>
  <p align="center">
    A Demonstration of Unreal Engine Gameplay Programming
</div>

<!-- ABOUT THE PROJECT -->
## About The Project
![img-StartupScreen]
Focused on the C++ programming side, Action Roguelike was a playground for me to get familiar with different Unreal Engine frameworks.

## Features
Adhering to Epic Games coding standard and C++ best practices guideline, here are some of the features of the project

- Third-person Action Character Movement
- Event-based logic to drive UI and gameplay reactions.
- Mix of C++ & Blueprint and how to combine them effectively.
- **Action System** - an implementation of custom Gameplay Ability System
  - Projectile Attack
  - Blackhole Ability
  - Teleport Ability 
  - "Thorns" buff (reflecting damage)
  - Burning Damage-over-time effect
- Local multiplayer 
  - Hosting and joining sessions
  - Supports all features via Unreal Engine replication framework
- AI
  - Enemy bots with Behavior Trees (Roam, See, Chase, Attack, Flee/Heal)
  - Environment Query System for attack and cover locations by AI

![img-GameplayFeatures]

- **SaveGame System** for persisting progress of character and world state.
- Powerup pickups to heal, gain credits and actions
- GameplayTags
  - to mark-up Actors, Buffs, Actions
  - to implement situational functionalities, like having a specific key to open something
- UI (UMG)
  - Main menu to host/join game
  - UI elements for player attributes and projected widgets for powerups and enemy health.
  - C++ Localized Text
- Utilizing Asset Manager for async loading of data assets and UI icons

![img-Abilities]


# Credits
Many thanks to Mr. [Tom Looman](https://www.tomlooman.com/) for being such a nice and dedicated mentor through the teaching of [Professional Game Developmnet in C++ and Unreal Engine](https://courses.tomlooman.com/p/unrealengine-cpp?coupon_code=COMMUNITY15&src=github) course, taught in Standford University (CS193U).

Another thanks to [Saman Ghaedhosseini](mailto:saman3dgh@yahoo.com) for his continuous, unwavering support.

**Game Assets:** Licensed for use with the Unreal Engine only. Without a custom license you cannot use to create sequels, remasters, or otherwise emulate the original game or use the original game's trademarks, character names, or other IP to advertise or name your game. (Unreal Engine EULA applies) (Please note this applies to the Game Assets that refer to Epic Games' Paragon project)

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[img-StartupScreen]: https://raw.githubusercontent.com/arashiscoding/ActionRoguelike/refs/heads/main/ReadmeResources/StartupScreen.jpg
[img-GameplayFeatures]: https://raw.githubusercontent.com/arashiscoding/ActionRoguelike/refs/heads/main/ReadmeResources/GameplayFeatures.jpg
[img-Abilities]: https://raw.githubusercontent.com/arashiscoding/ActionRoguelike/refs/heads/main/ReadmeResources/Abilities.jpg