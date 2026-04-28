#!/usr/bin/env bash
# ============================================================
#  uni — installer (with built-in tutorial)
#  Usage: curl -fsSL https://raw.githubusercontent.com/dronzer-tb/UNI/refs/heads/master/installer.sh | bash
#
#  ⚠️  DISCLAIMER: This code was improved with the help of AI
#  ============================================================

set -euo pipefail

UNI_VERSION="2.2.1"
UNI_RAW_URL="https://raw.githubusercontent.com/dronzer-tb/UNI/refs/heads/master/uni"
INSTALL_DIR="$HOME/.local/bin"
INSTALL_PATH="$INSTALL_DIR/uni"

# ── Colors ───────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
BOLD='\033[1m'
DIM='\033[2m'
RESET='\033[0m'

info()    { echo -e "${CYAN}${BOLD} → ${RESET} $*"; }
success() { echo -e "${GREEN}${BOLD} ✓ ${RESET} $*"; }
warn()    { echo -e "${YELLOW}${BOLD} ! ${RESET} $*"; }
die()     { echo -e "${RED}${BOLD} ✗ ${RESET} $*" >&2; exit 1; }
header()  { echo ""; echo -e "${BOLD}${BLUE}$*${RESET}"; echo -e "${DIM}────────────────────────────────────────${RESET}"; }
pause()   { read -rsp "$(echo -e "  ${DIM}Press Enter to continue...${RESET}")" && echo ""; }

# ── Banner ───────────────────────────────────────────────────
echo ""
echo -e "${BOLD}${BLUE}  uni${RESET} ${DIM}v${UNI_VERSION}${RESET} — Universal Package Manager"
echo -e "  ${DIM}by Dronzer Studios / Kasniya${RESET}"
echo -e "${DIM}  ────────────────────────────────────────${RESET}"
echo ""

# ── OS check ─────────────────────────────────────────────────
if [[ "$(uname -s)" != "Linux" ]]; then
    die "uni only supports Linux."
fi

# ── Arch check ───────────────────────────────────────────────
ARCH="$(uname -m)"
case "$ARCH" in
    x86_64|aarch64|armv7l) ;;
    *) warn "Untested architecture: ${ARCH}. Proceeding anyway." ;;
esac

# ── Dependency check & install ───────────────────────────────
info "Checking dependencies..."

check_and_install_dep() {
    local cmd="$1" pkg="$2" apt_name="${3:-$pkg}"
    
    if ! command -v "$cmd" &>/dev/null; then
        warn "$cmd not found. Installing $apt_name..."
        if command -v apt &>/dev/null; then
            sudo apt-get update -qq && sudo apt-get install -y "$apt_name" >/dev/null 2>&1 || \
                die "Failed to install $apt_name. Please install manually."
        else
            die "$cmd is required but not installed. Please install it manually."
        fi
    fi
}

check_and_install_dep "python3" "python3" "python3"
check_and_install_dep "curl" "curl" "curl"

success "Dependencies OK"

# ── Create install directory ─────────────────────────────────
mkdir -p "$INSTALL_DIR"

# ── Download uni ─────────────────────────────────────────────
info "Downloading uni..."
if curl -Lf# "$UNI_RAW_URL" -o "$INSTALL_PATH" 2>/dev/null; then
    chmod +x "$INSTALL_PATH"
    success "uni installed to $INSTALL_PATH"
else
    die "Failed to download uni from $UNI_RAW_URL"
fi

# ── Add to PATH ──────────────────────────────────────────────
if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    info "Adding $INSTALL_DIR to PATH..."
    for rc_file in ~/.bashrc ~/.zshrc ~/.bash_profile ~/.zprofile; do
        if [[ -f "$rc_file" ]] && ! grep -q "export PATH.*\.local/bin" "$rc_file"; then
            echo "" >> "$rc_file"
            echo "export PATH=\"\$HOME/.local/bin:\$PATH\"  # uni" >> "$rc_file"
        fi
    done
    success "PATH updated"
fi

# ── Init uni registry ────────────────────────────────────────
info "Initializing uni..."
mkdir -p "$HOME/.local/share/uni" "$HOME/.cache/uni"
[[ -f "$HOME/.local/share/uni/installed.json" ]] || echo '{}' > "$HOME/.local/share/uni/installed.json"
success "uni is ready"
echo ""

# ── INTERACTIVE TUTORIAL ─────────────────────────────────────
cmd_tutorial() {
    clear
    echo ""
    echo -e "${BOLD}${BLUE}  Welcome to uni — the universal package manager${RESET}"
    echo -e "  ${DIM}An interactive walkthrough of the main commands.${RESET}"
    echo -e "${DIM}  ────────────────────────────────────────${RESET}"
    echo ""
    pause

    # ── 1. What is uni? ──────────────────────────────────────
    clear
    header "1 / 7  —  What is uni?"
    echo ""
    echo -e "  ${BOLD}uni${RESET} is a Universal Package Manager wrapper for Linux."
    echo -e "  It searches ${BOLD}apt, flatpak, snap,${RESET} and ${BOLD}GitHub${RESET} at the same time,"
    echo -e "  and lets you install packages from any source with one command."
    echo ""
    echo -e "  ${DIM}No more:${RESET}  ${DIM}apt install this, snap install that, flatpak install other...${RESET}"
    echo -e "  ${BOLD}Just:${RESET}  ${CYAN}uni install htop${RESET}"
    echo ""
    echo -e "  ${MAGENTA}💡 Tip:${RESET} uni tracks everything in one registry (~/.local/share/uni/installed.json)"
    echo ""
    pause

    # ── 2. Installing from anywhere ──────────────────────────
    clear
    header "2 / 7  —  Installing packages"
    echo ""
    echo -e "  ${BOLD}uni install htop${RESET}"
    echo -e "  ${DIM}→ searches all sources, shows you the options, you pick${RESET}"
    echo ""
    echo -e "  ${BOLD}uni install htop --source apt${RESET}"
    echo -e "  ${DIM}→ skip the search, go straight to apt${RESET}"
    echo ""
    echo -e "  ${BOLD}uni install user/repo${RESET}   ${DIM}or${RESET}   ${BOLD}uni install https://github.com/user/repo${RESET}"
    echo -e "  ${DIM}→ install latest GitHub release, auto-picks the best build for your CPU${RESET}"
    echo ""
    echo -e "  ${BOLD}uni install file.deb${RESET}"
    echo -e "  ${BOLD}uni install https://example.com/app.AppImage${RESET}"
    echo -e "  ${DIM}→ local or remote .deb / .AppImage / .zip files work directly${RESET}"
    echo ""
    echo -e "  ${MAGENTA}💡 Tip:${RESET} uni auto-detects your CPU (x86_64, arm64, armhf) for GitHub releases"
    echo ""
    pause

    # ── 3. Removing packages ─────────────────────────────────
    clear
    header "3 / 7  —  Removing packages"
    echo ""
    echo -e "  ${BOLD}uni remove htop${RESET}"
    echo -e "  ${DIM}→ removes the package, keeps config files${RESET}"
    echo ""
    echo -e "  ${BOLD}uni purge htop${RESET}"
    echo -e "  ${DIM}→ removes everything: package, config, AND data${RESET}"
    echo -e "  ${DIM}   (for flatpak, also wipes ~/.var/app/<id>)${RESET}"
    echo ""
    echo -e "  ${MAGENTA}⚠️  Warning:${RESET} Purge is permanent. Make sure you want it gone."
    echo ""
    pause

    # ── 4. Managing your packages ────────────────────────────
    clear
    header "4 / 7  —  Listing & updating"
    echo ""
    echo -e "  ${BOLD}uni list${RESET}"
    echo -e "  ${DIM}→ shows all packages installed via uni, with backend and ID${RESET}"
    echo ""
    echo -e "  ${BOLD}uni update${RESET}"
    echo -e "  ${DIM}→ runs all backends at once (apt, flatpak, snap)${RESET}"
    echo -e "  ${DIM}   flatpak and snap run in parallel for speed${RESET}"
    echo ""
    echo -e "  ${MAGENTA}💡 Tip:${RESET} This is safer than running apt/flatpak/snap separately"
    echo ""
    pause

    # ── 5. Keeping uni up to date ────────────────────────────
    clear
    header "5 / 7  —  Updating uni itself"
    echo ""
    echo -e "  ${BOLD}uni self-update${RESET}   ${DIM}(or: uni --update)${RESET}"
    echo -e "  ${DIM}→ checks GitHub for a newer version${RESET}"
    echo ""
    echo -e "  ${BOLD}uni version${RESET}   ${DIM}(or: uni --version)${RESET}"
    echo -e "  ${DIM}→ shows your current version${RESET}"
    echo ""
    echo -e "  ${MAGENTA}💡 Tip:${RESET} uni checks for updates once a day in the background"
    echo ""
    pause

    # ── 6. Intercept mode (advanced) ─────────────────────────
    clear
    header "6 / 7  —  Intercept mode"
    echo ""
    echo -e "  Want ${BOLD}apt, snap, flatpak${RESET} to all route through uni automatically?"
    echo ""
    echo -e "  ${BOLD}uni intercept on${RESET}"
    echo -e "  ${DIM}→ now ${CYAN}apt install foo${RESET}  becomes ${CYAN}uni install foo${RESET} automatically${RESET}"
    echo ""
    echo -e "  ${BOLD}uni intercept off${RESET}"
    echo -e "  ${DIM}→ disable it cleanly${RESET}"
    echo ""
    echo -e "  ${BOLD}uni intercept status${RESET}"
    echo -e "  ${DIM}→ check if it's enabled${RESET}"
    echo ""
    echo -e "  ${MAGENTA}⚠️  Note:${RESET} After enabling, reload your shell: ${CYAN}source ~/.bashrc${RESET}"
    echo ""
    pause

    # ── 7. Quick reference ───────────────────────────────────
    clear
    header "7 / 7  —  Quick reference"
    echo ""
    echo -e "  ${BOLD}uni install${RESET} <pkg>                  search everywhere"
    echo -e "  ${BOLD}uni install${RESET} <pkg> --source apt     force a backend"
    echo -e "  ${BOLD}uni install${RESET} owner/repo             from GitHub"
    echo -e "  ${BOLD}uni install${RESET} file.deb               local file"
    echo ""
    echo -e "  ${BOLD}uni remove${RESET}  <pkg>                  remove (keep config)"
    echo -e "  ${BOLD}uni purge${RESET}   <pkg>                  remove + wipe config"
    echo -e "  ${BOLD}uni list${RESET}                           what's installed"
    echo -e "  ${BOLD}uni update${RESET}                         update all backends"
    echo -e "  ${BOLD}uni self-update${RESET}                    update uni itself"
    echo -e "  ${BOLD}uni intercept${RESET} on/off/status        auto-route other PMs"
    echo -e "  ${BOLD}uni help${RESET}                           show this help"
    echo ""
    echo -e "  ${BOLD}uni tutorial${RESET}                       show this guide again"
    echo -e "${DIM}  ────────────────────────────────────────${RESET}"
    echo ""
    success "You're ready! Try:  ${CYAN}uni install htop${RESET}"
    echo ""
}

# ── Run tutorial ─────────────────────────────────────────────
info "Starting interactive tutorial..."
sleep 1
cmd_tutorial

echo -e "${MAGENTA}✨ Setup complete!${RESET}"
echo ""
echo -e "  ${DIM}To reload your shell and start using uni:${RESET}"
echo -e "  ${CYAN}\$ source ~/.bashrc${RESET}  ${DIM}(or ~/.zshrc)${RESET}"
echo ""
echo -e "  ${DIM}To run the tutorial again:${RESET}"
echo -e "  ${CYAN}\$ uni tutorial${RESET}"
echo ""

# ── AI Disclaimer ────────────────────────────────────────────
echo -e "${DIM}────────────────────────────────────────${RESET}"
echo -e "${YELLOW}${BOLD} ⚠️  DISCLAIMER${RESET}"
echo -e "${DIM}This installation script and uni codebase have been"
echo -e "improved with the help of AI assistance. The original code"
echo -e "has been enhanced for better UX, features, and reliability.${RESET}"
echo -e "${DIM}────────────────────────────────────────${RESET}"
echo ""
