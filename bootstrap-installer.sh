#!/usr/bin/env bash
# ============================================================
#  uni — bootstrap installer (minimal & memorable)
#  Copy-paste into terminal:
#
#  bash <(curl -fsSL https://raw.githubusercontent.com/dronzer-tb/UNI/refs/heads/master/bootstrap-installer.sh)
#
#  Even shorter (if you prefer):
#  curl -fsSL https://raw.githubusercontent.com/dronzer-tb/UNI/refs/heads/master/bootstrap-installer.sh | bash
#
#  ⚠️  DISCLAIMER: Code improved with AI help
#  ============================================================

set -euo pipefail

UNI_VERSION="0.5.0"
UNI_REPO="${UNI_REPO:-dronzer-tb/UNI}"
INSTALL_DIR="$HOME/.local/bin"
INSTALL_PATH="$INSTALL_DIR/uni"

# Colors (minimal)
RED='\033[0;31m' GREEN='\033[0;32m' YELLOW='\033[1;33m' BLUE='\033[0;34m'
CYAN='\033[0;36m' BOLD='\033[1m' DIM='\033[2m' RESET='\033[0m'

# Minimal UI
_info()  { echo -e "${CYAN}${BOLD} → ${RESET}$*"; }
_ok()    { echo -e "${GREEN}${BOLD} ✓ ${RESET}$*"; }
_warn()  { echo -e "${YELLOW}${BOLD} ! ${RESET}$*"; }
_die()   { echo -e "${RED}${BOLD} ✗ ${RESET}$*" >&2; exit 1; }

# Quick banner
echo ""
echo -e "${BOLD}${BLUE}uni${RESET} ${DIM}v${UNI_VERSION}${RESET} — Universal Package Manager"
echo ""

# Minimal checks
[[ "$(uname -s)" == "Linux" ]] || _die "Linux only"
command -v python3 &>/dev/null || _die "python3 required"
command -v curl &>/dev/null || _die "curl required"

# ── DISCLAIMER & CONFIRMATION ────────────────────────────────
echo -e "${YELLOW}${BOLD}⚠️  DISCLAIMER${RESET}"
echo -e "${DIM}This installation script and uni codebase have been"
echo -e "improved with the help of AI assistance. The original code"
echo -e "has been enhanced for better UX, features, and reliability.${RESET}"
echo ""
echo -e "${BOLD}What will be installed:${RESET}"
echo -e "  ${DIM}• uni binary → ${INSTALL_PATH}${RESET}"
echo -e "  ${DIM}• Config dir → $HOME/.local/share/uni/${RESET}"
echo -e "  ${DIM}• Cache dir → $HOME/.cache/uni/${RESET}"
echo -e "  ${DIM}• PATH updated in shell configs${RESET}"
echo ""

read -p "$(echo -e "${CYAN}${BOLD}Do you want to proceed? [y/N]:${RESET} ")" -r confirm
if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
    echo -e "${YELLOW}Installation cancelled.${RESET}"
    exit 0
fi

echo ""

# Install
mkdir -p "$INSTALL_DIR"
_info "Downloading uni..."
curl -Lf# "https://raw.githubusercontent.com/dronzer-tb/UNI/refs/heads/master/uni" -o "$INSTALL_PATH" || _die "Download failed"
chmod +x "$INSTALL_PATH"
_ok "Downloaded"

# Add to PATH
if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    _info "Updating PATH in shell configs..."
    for rc in ~/.bashrc ~/.zshrc ~/.bash_profile ~/.zprofile; do
        [[ -f "$rc" ]] && ! grep -q "\.local/bin" "$rc" && echo "" >> "$rc" && echo "export PATH=\"\$HOME/.local/bin:\$PATH\"  # uni" >> "$rc"
    done
    _ok "PATH updated"
fi

# Init
_info "Initializing uni..."
mkdir -p "$HOME/.local/share/uni" "$HOME/.cache/uni"
echo '{}' > "$HOME/.local/share/uni/installed.json" 2>/dev/null || true
_ok "Ready"

echo ""
_ok "Installation complete!"
echo ""
echo -e "  ${DIM}Next steps:${RESET}"
echo -e "  ${CYAN}\$ source ~/.bashrc${RESET}  ${DIM}(reload your shell)${RESET}"
echo -e "  ${CYAN}\$ uni tutorial${RESET}      ${DIM}(learn how to use uni)${RESET}"
echo ""
