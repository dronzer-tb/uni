# Changelog

All notable changes to uni are documented in this file.

## [2.2.0] — 2024-01-20

### Added
- Search caching system (1 hour default TTL)
- Update checking for installed packages
- Package intercept mode for debugging
- Self-update functionality
- Interactive tutorial system
- Enhanced registry with better metadata
- Configuration file support (~/.config/uni/config)
- Batch operation improvements
- Package source statistics tracking
- Improved error recovery for network failures

### Changed
- Enhanced error messages with better context
- Improved UI feedback and responsiveness
- Better logging and debugging output

### Fixed
- Various stability improvements
- Better handling of edge cases

## [2.1.3] — 2024-01-15

### Fixed
- AppImage permissions handling
- GitHub API rate limiting
- Snap package detection
- Home directory resolution under sudo
- Configuration file edge cases

### Improved
- Search performance optimization
- CLI output formatting
- Error messages and logging

## [2.0.0] — 2024-01-10

### Added
- Flatpak integration
- Snap package support
- GitHub Releases installation
- AppImage detection and installation
- Archive extraction (.tar.gz, .tar.xz, .zip)
- Desktop integration for AppImages
- Smart architecture detection
- Multi-backend support

### Changed
- Complete rewrite for multi-backend support
- Improved registry system
- Enhanced UI with better visual feedback

### Removed
- Single-backend limitation

## [1.3.0] — 2024-01-01

### Added
- Core installation functionality
- apt/nala support
- Basic CLI interface
- Remove and list commands
- Package registry system
- Initial documentation

### Status
- Foundation release of uni

---

## Version Format

This project follows [Semantic Versioning](https://semver.org/):
- MAJOR version when breaking changes
- MINOR version when adding features
- PATCH version when fixing bugs

---

For detailed information about changes, see git history or GitHub releases.

---
[2.2.0]: https://github.com/dronzer-tb/uni/releases/tag/v2.2.0
[2.1.3]: https://github.com/dronzer-tb/uni/releases/tag/v2.1.3
[2.0.0]: https://github.com/dronzer-tb/uni/releases/tag/v2.0.0
[1.3.0]: https://github.com/dronzer-tb/uni/releases/tag/v1.3.0
