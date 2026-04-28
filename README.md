# uni — Universal Package Manager for Linux

> **One command to rule them all:** `uni install htop` instead of `apt install`, `snap install`, or `flatpak install`

![Version](https://img.shields.io/badge/version-2.2.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---



## 🚀 Quick Start

### Installation (One-Liner)

```bash
curl -fsSL https://raw.githubusercontent.com/YOUR_USER/YOUR_REPO/main/installer.sh | bash
```

This single command will:
- ✅ Check and install dependencies (`python3`, `curl`)
- ✅ Download and install `uni` to `~/.local/bin`
- ✅ Add `~/.local/bin` to your PATH
- ✅ Initialize the package registry
- ✅ Run an interactive tutorial

**After installation:**
```bash
source ~/.bashrc  # Reload your shell
uni install package-name  # Start using uni!
```

---

## 📦 Core Commands

### Installing Packages

```bash
# Search everywhere (apt, flatpak, snap, GitHub)
uni install htop
# → Shows you all available options, you pick

# Force a specific backend
uni install htop --source apt
uni install htop --source flatpak

# Install from GitHub (auto-detects your CPU architecture)
uni install user/repo                              # Latest release
uni install https://github.com/user/repo
uni install https://github.com/user/repo/releases/latest

# Install local or remote files
uni install /path/to/file.deb
uni install https://example.com/app.AppImage
uni install archive.zip
```

### Removing Packages

```bash
# Remove (keep config files)
uni remove htop

# Purge (remove package + all data & configs)
uni purge htop
# For flatpak, also removes ~/.var/app/<id>
```

### Managing Packages

```bash
# List all packages installed via uni
uni list

# Update all backends at once (apt, flatpak, snap in parallel)
uni update

# Update uni itself
uni self-update
uni --update

# Check version
uni version
uni --version
```

### Intercept Mode (Advanced)

Route all package manager commands through `uni` automatically:

```bash
# Enable: apt/snap/flatpak now route through uni
uni intercept on

# Disable cleanly
uni intercept off

# Check status
uni intercept status

# After enabling, reload your shell:
source ~/.bashrc
```

Now `apt install foo` → `uni install foo` automatically!

### Other Commands

```bash
# Interactive tutorial
uni tutorial

# Show help
uni help

# Debug mode
uni intercept package-name
```

---

## 🔧 Supported Backends

| Backend | Type | Use Case |
|---------|------|----------|
| **apt/nala** | Native packages | Debian, Ubuntu, Pop!_OS, Linux Mint |
| **flatpak** | Containerized apps | Universal Linux apps (best for desktops) |
| **snap** | Universal packages | Cross-distro packages |
| **GitHub** | Release downloads | Latest binaries, AppImages, archives |

---

## 💾 Registry & Storage

`uni` tracks all installed packages in a single JSON registry:

```
~/.local/share/uni/installed.json
```

Example:
```json
{
  "htop": {
    "backend": "apt",
    "id": "htop"
  },
  "flatseal": {
    "backend": "flatpak",
    "id": "com.github.flatseal"
  },
  "gh-cli": {
    "backend": "github",
    "id": "cli/cli"
  }
}
```

### Other Directories

```
~/.local/bin/uni                    # Main executable
~/.local/share/uni/appimages/       # Downloaded AppImages
~/.local/share/uni/icons/           # Desktop app icons
~/.cache/uni/                       # Search cache (1-hour TTL)
```

---

##  System Requirements

- **OS:** Linux only
- **Shell:** Bash 4+
- **Dependencies:** `python3`, `curl`
- **Architectures:** x86_64, aarch64, armv7l (others may work)
- **Optional:** `aria2c` (for faster parallel downloads)

---

## ⚡ Performance Tips

### Use aria2c for faster downloads
```bash
sudo apt install aria2
# uni will automatically use it for parallel downloads
```

### Enable intercept mode for convenience
```bash
uni intercept on
source ~/.bashrc
# Now apt/snap/flatpak all route through uni
```

### Run updates in background
```bash
uni update &  # Background process
```

---

##  Configuration

### Shell Integration

After installation, your shell RC files are updated:

```bash
export PATH="$HOME/.local/bin:$PATH"  # uni
```

To manually add `uni` to PATH:
```bash
export PATH="$HOME/.local/bin:$PATH"
```

### Changing Install Directory

By default, `uni` is installed to `~/.local/bin`. To install elsewhere:

```bash
# Modify the installer before running
UNI_RAW_URL="..." INSTALL_DIR="/custom/path" bash installer.sh
```

---

## ⚠️ Disclaimer

This code has been **improved with the help of AI assistance**. The original `uni` concept and core functionality were enhanced for:
- Better user experience
- Improved error handling
- Enhanced documentation
- Additional features
- Code quality improvements

---

## 📄 License

GNU General Public License v3.0. — See LICENSE file for details
