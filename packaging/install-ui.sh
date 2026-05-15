#!/usr/bin/env bash
# Install the optional Qt UI (uni-ui) and its integration units.
#
# This is intentionally separate from bootstrap-installer.sh so the headless
# CLI install path stays minimal. Run after `uni` itself is installed.
#
# Usage: ./packaging/install-ui.sh

set -euo pipefail

BLUE='\033[0;34m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
RED='\033[0;31m'; BOLD='\033[1m'; DIM='\033[2m'; RESET='\033[0m'
info()    { echo -e "${BLUE}${BOLD} → ${RESET} $*"; }
success() { echo -e "${GREEN}${BOLD} ✓ ${RESET} $*"; }
warn()    { echo -e "${YELLOW}${BOLD} ! ${RESET} $*"; }
die()     { echo -e "${RED}${BOLD} ✗ ${RESET} $*" >&2; exit 1; }

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
UI_DIR="$REPO_ROOT/uni-ui"
INSTALL_BIN="$HOME/.local/bin"
DESKTOP_DIR="$HOME/.local/share/applications"
SYSTEMD_USER_DIR="$HOME/.config/systemd/user"

# ── Build prerequisites ──────────────────────────────────────
command -v cmake >/dev/null  || die "cmake is required. Install: uni install cmake --source apt"
command -v g++   >/dev/null  || die "g++ is required. Install: uni install build-essential --source apt"

if ! find /usr -name 'Qt6Config.cmake' 2>/dev/null | head -1 | grep -q .; then
    die "Qt6 development files not found. Install: uni install qt6-base-dev qt6-tools-dev --source apt"
fi

# ── Build ────────────────────────────────────────────────────
info "Configuring (cmake)..."
cmake -S "$UI_DIR" -B "$UI_DIR/build" -DCMAKE_BUILD_TYPE=Release >/dev/null

info "Compiling..."
cmake --build "$UI_DIR/build" -j"$(nproc)" >/dev/null

# ── Install binary ───────────────────────────────────────────
mkdir -p "$INSTALL_BIN"
install -m 0755 "$UI_DIR/build/uni-ui" "$INSTALL_BIN/uni-ui"
success "Installed uni-ui → $INSTALL_BIN/uni-ui"

# ── Desktop entry ────────────────────────────────────────────
mkdir -p "$DESKTOP_DIR"
install -m 0644 "$REPO_ROOT/packaging/uni.desktop" "$DESKTOP_DIR/uni.desktop"
success "Installed desktop entry → $DESKTOP_DIR/uni.desktop"

# ── systemd user units (timer for index refresh) ─────────────
if command -v systemctl >/dev/null && systemctl --user --no-pager status >/dev/null 2>&1; then
    mkdir -p "$SYSTEMD_USER_DIR"
    install -m 0644 "$REPO_ROOT/packaging/uni-index.service" "$SYSTEMD_USER_DIR/"
    install -m 0644 "$REPO_ROOT/packaging/uni-index.timer"   "$SYSTEMD_USER_DIR/"
    systemctl --user daemon-reload
    systemctl --user enable --now uni-index.timer >/dev/null
    success "Enabled uni-index.timer (refreshes index every 6h)"
else
    warn "systemd --user not available; skipping timer install."
    warn "uni-ui will refresh the index on launch instead."
fi

echo
success "uni-ui ${BOLD}0.6.0${RESET} installed. Launch from your app menu or run: ${BOLD}uni-ui${RESET}"
