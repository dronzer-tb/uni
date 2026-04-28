# UNI — Universal Package Manager Wrapper v2.0.0

A unified command interface for installing, managing, and updating software across different package management backends on Linux.

## Overview

**UNI** abstracts away the complexity of managing packages from multiple sources—APT, Flatpak, and GitHub releases—through a single, consistent command structure. Whether you're installing from system repositories, Flatpak remotes, or downloading the latest release from GitHub, UNI handles asset selection, installation, and registry management automatically.

### Key Features

**Unified Interface** — One command for all package sources  
**Smart Asset Detection** — Automatically selects the best binary for your system architecture  
**Multiple Backends** — APT (system packages), Flatpak (sandboxed apps), and GitHub releases  
**Central Registry** — Track installed packages across all backends  
**Complete Removal** — Purge packages and all associated data  
**GitHub Integration** — Download and install latest releases with minimal input  
**Lightweight & Fast** — Minimal dependencies (bash, python3, curl)  
**User-Level Installation** — Works without requiring sudo for most operations  

---

## Installation

Clone the repository and link the script to your PATH:

```bash
git clone https://github.com/dronzer-tb/UNI
chmod +x ~/uni/uni-v2.0.0.txt
ln -s ~/uni/uni-v2.0.0.txt ~/.local/bin/uni
```

Ensure `~/.local/bin` is in your `$PATH`:

```bash
export PATH="$PATH:$HOME/.local/bin"
```

### Requirements

- **bash** — v4.0+
- **python3** — v3.6+
- **curl** — for downloading files

Optional for GitHub integration:
- **gh** — GitHub CLI (recommended for release fetching)

---

## Quick Start

### Basic Commands

```bash
# Install a package
uni install <package>

# List installed packages
uni list

# Update installed packages
uni update

# Remove a package
uni remove <package>

# Completely remove a package and all data
uni purge <package>

# Show help
uni help
```

### Install from Different Sources

#### APT (System Repository)

```bash
uni install firefox
```

#### Flatpak

```bash
uni install flatpak:org.gnome.Calendar
```

#### GitHub Release

```bash
uni install github:username/repo-name
```

---

## How It Works

### Package Installation Flow

1. **Identify Backend** — UNI determines the source (APT, Flatpak, or GitHub)
2. **Fetch Metadata** — Retrieves available versions and assets
3. **Detect Architecture** — Identifies your system architecture (x86_64, ARM, etc.)
4. **Smart Asset Selection** — Scores and ranks available binaries:
   - Prefers `.deb` packages for system repos
   - Favors `.appimage` for portable apps
   - Supports `.tar.gz`, `.tar.xz`, and `.zip` archives
   - Automatically rejects incompatible architectures and platforms
5. **Install** — Deploys the package using the appropriate method
6. **Register** — Tracks installation in the central registry

### Asset Scoring

For GitHub releases, UNI intelligently selects the best asset by scoring based on:

- **Architecture Match** (+50 points) — Prioritizes your system's CPU architecture
- **Format Preference** — `.deb` (+40), `.appimage` (+30), archives (+5-10)
- **Linux Compatibility** (+20) — Favors Linux-specific builds
- **Penalties** — Filters out checksums (-500), Windows/macOS builds (-300), debug symbols (-100)

If multiple assets have similar scores, you're prompted to select manually.

---

## Registry & Storage

UNI organizes installed packages and metadata in user-level directories:

```
~/.local/share/uni/
├── installed.json          # Central registry of all installed packages
├── appimages/              # Cached AppImage files
├── icons/                  # Package icons for desktop integration
└── applications/           # Desktop entry files
```

Each entry in `installed.json` tracks:
- Package name
- Installation backend
- Package ID or path
- Installation timestamp

---

## Command Reference

### `uni install <PACKAGE>`

Install a package from the appropriate backend.

**Syntax:**
- `uni install <name>` — Install from APT (system default)
- `uni install flatpak:<id>` — Install a Flatpak app (e.g., `org.kde.Dolphin`)
- `uni install github:<owner>/<repo>` — Install from GitHub release

**Example:**
```bash
uni install vlc                              # Install VLC via APT
uni install flatpak:com.spotify.Client       # Install Spotify Flatpak
uni install github:cli/cli                   # Install GitHub CLI from latest release
```

### `uni list [BACKEND]`

List installed packages. Optionally filter by backend.

**Example:**
```bash
uni list                    # Show all installed packages
uni list apt                # Show APT packages only
uni list flatpak            # Show Flatpak packages only
uni list github             # Show GitHub-installed packages
```

### `uni update [PACKAGE]`

Check for updates and upgrade packages.

**Example:**
```bash
uni update                  # Update all packages
uni update vlc              # Update only VLC
```

### `uni remove <PACKAGE>`

Remove a package while preserving user data and configuration.

**Example:**
```bash
uni remove vlc
```

### `uni purge <PACKAGE>`

Completely remove a package and all associated data, caches, and configuration.

**Warning:** This action is irreversible.

**Example:**
```bash
uni purge vlc
```

### `uni help`

Display help and usage information.

---

## Supported Backends

### APT (Debian/Ubuntu)

Installs packages from the system repository. Best for essential system tools and libraries.

```bash
uni install git
uni install python3-dev
```

### Flatpak

Installs sandboxed applications. Ideal for desktop apps and third-party software.

```bash
uni install flatpak:org.gnome.Calendar
uni install flatpak:com.obsproject.Studio
```

Browse available Flatpak apps at [flathub.org](https://flathub.org)

### GitHub

Automatically downloads and installs the latest release from any GitHub repository. UNI handles:
- Release fetching
- Asset scoring and selection
- Binary extraction and placement
- Desktop integration (where applicable)

```bash
uni install github:cli/cli
uni install github:BurntSushi/ripgrep
uni install github:junegunn/fzf
```

---

## Architecture Support

UNI automatically detects and prioritizes binaries for:

- **x86_64** (AMD64) — Intel/AMD 64-bit processors
- **ARM64** (aarch64) — Apple Silicon, Raspberry Pi 4+
- **ARMv7l** — 32-bit ARM (Raspberry Pi 3, etc.)
- **i686** — 32-bit x86 processors

If no binary matches your architecture, UNI will prompt you to select manually from available options.

---

## Advanced Usage

### Manual Asset Selection

When installing from GitHub, if auto-detection finds ambiguous matches, you'll see a menu:

```
 ! Could not auto-detect asset for aarch64. Please select:

  1) tool-1.2.3-aarch64.deb
  2) tool-1.2.3-arm64.tar.gz
  3) tool-1.2.3-amd64.deb

Select asset [1-3]: 1
```

### Viewing Installed Packages

Query the registry directly:

```bash
cat ~/.local/share/uni/installed.json | python3 -m json.tool
```

### Removing AppImages

Installed AppImages are cached and can be removed manually:

```bash
ls ~/.local/share/uni/appimages/
rm ~/.local/share/uni/appimages/toolname.AppImage
```

---

## Configuration

### Environment Variables

Control UNI behavior with environment variables:

| Variable | Purpose | Example |
|----------|---------|---------|
| `UNI_REGISTRY` | Override registry location | `export UNI_REGISTRY=~/.config/uni/registry.json` |
| `UNI_APPIMAGE_DIR` | Custom AppImage cache location | `export UNI_APPIMAGE_DIR=/mnt/storage/apps` |

---

## Development & Contribution

### Project Structure

```
uni/
├── uni-v2.0.0.txt          # Main executable
├── README.md                # This file
└── LICENSE                  # GPL v3
```

### License

This project is licensed under the **GNU General Public License v3.0**. See [LICENSE](LICENSE) for details.
