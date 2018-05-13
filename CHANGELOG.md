## [v0.3.1]
### Changed
- Better

## [v0.3.0]
### Added
- Added function to generate stairs in the dungeon.

### Changed
- Smart pointers were being overused. Changed pointer/reference policy.
- Major refactoring

## [v0.2.4]
### Added
- Added FastNoise <github.com/Auburns/FastNoise> (MIT License) as support library
- Added the possibility to generate world maps
- Added comments in header files

### Changed
- Major Refactoring
- MapBuilder now cannot be "chain called" in order to build a map

## [v0.2.3]
### Added
- Added a function to check if a tile is adjacent to a room

### Removed
- Removed a redundant security check in `GetTile()`

## [v0.2.2]
### Added
- Added a `name` field to Tag which is used for comparsion
- Added an `explored` tag to the default tags

### Changed
- Redesigned the `RndManager` class
- Major refactoring
- Removed from the headers all typedefs used for smart pointers
- Changed the header guard flags to a stricter naming convention
- Changed indentation standards

## [v0.2.1]
### Added
- Added a universally unique identifier generator
- Added a general libpmg.hpp header

### Changed
- Parametrized every hard coded value into include/global.hpp
- Major refactoring
- MapBuilder now returns a unique pointer to Map

## [v0.2.0]
### Added 
- Added a new changelog, following the standards in <http://keepachangelog.com/en/1.0.0/>
- Added the new PriorityQueue class

### Changed
- Parametrized every hard coded value into include/global.hpp
- Major refactoring
- The user can now extend Tag and, and add custom tags to the map
- Removed all raw pointers in favor of smart pointers
- Fully compiled using --std=c++1z

### Removed
- Removed deprecation warning from `MapBuilder::UpdateRect()`
- Removed `Map::Normalize()`
- Removed `std::shared_ptr<Tile> GetTile(std::shared_ptr<Location> loc)`

### Fixed
- Fixed crash when placing doors on map borders
- Now corridors and rooms don't collide as much as before
- Doors now are placed upon floor tiles
