#!/usr/bin/env bash
set -euo pipefail

# --- Параметры пакета ---
PKG_NAME="visualization"
PKG_VER="1.3.0"
ARCH="$(dpkg --print-architecture)"                   # amd64, arm64 и т.п.
TRIPLET="$(dpkg-architecture -qDEB_HOST_MULTIARCH)"   # x86_64-linux-gnu и т.п.

# --- Пути проекта ---
ROOT_DIR="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD_DIR="$ROOT_DIR/cmake-build-release-astra/visualization"             # где лежит собранный бинарник
echo $BUILD_DIR
BIN_NAME="Visualization"

# --- Директория сборки пакета ---
PKG_DIR="$HOME/tmp/pkg-${PKG_NAME}_${PKG_VER}"
rm -rf "$PKG_DIR"
install -d "$PKG_DIR/DEBIAN"
install -d "$PKG_DIR/usr/bin"
install -d "$PKG_DIR/usr/lib/$TRIPLET"

# --- Копируем бинарь---
install -m755 "$BUILD_DIR/$BIN_NAME" "$PKG_DIR/usr/bin/$BIN_NAME"

# --- control ---
cat > "$PKG_DIR/DEBIAN/control" <<EOF
Package: $PKG_NAME
Version: $PKG_VER
Section: utils
Priority: optional
Architecture: $ARCH
Maintainer: harikeshi
Depends:
Description: Приложение визуализации.
EOF

# --- postinst/postrm (ldconfig) ---
cat > "$PKG_DIR/DEBIAN/postinst" <<'EOF'
#!/bin/sh
set -e
ldconfig
exit 0
EOF
chmod 0755 "$PKG_DIR/DEBIAN/postinst"

cat > "$PKG_DIR/DEBIAN/postrm" <<'EOF'
#!/bin/sh
set -e
ldconfig
exit 0
EOF
chmod 0755 "$PKG_DIR/DEBIAN/postrm"

# --- Сборка .deb ---
OUT_DEB="${PKG_NAME}_${PKG_VER}_${ARCH}.deb"
dpkg-deb --build "$PKG_DIR" "$ROOT_DIR/$OUT_DEB"

echo "Готово: $OUT_DEB"
echo "Установка: sudo dpkg -i name или sudo apt install ./$OUT_DEB"
