#!/usr/bin/env bash
set -euo pipefail

# --- Параметры пакета ---

PKG_NAME="visualization"
PKG_VER="1.0.0"
ARCH="$(dpkg --print-architecture)"                   # amd64, arm64 и т.п.
TRIPLET="$(dpkg-architecture -qDEB_HOST_MULTIARCH)"   # x86_64-linux-gnu и т.п.

# --- Пути проекта ---

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build/visualization"             # где лежит собранный бинарник
BIN_NAME="visualization"                        # имя бинарника
LIBD_DIR="$ROOT_DIR/../libs"               #! ваши готовые библиотеки

# --- Директория сборки пакета ---

PKG_DIR="$ROOT_DIR/pkg-${PKG_NAME}_${PKG_VER}"
rm -rf "$PKG_DIR"
install -d "$PKG_DIR/DEBIAN"
install -d "$PKG_DIR/usr/bin"
install -d "$PKG_DIR/usr/lib/$TRIPLET"

# --- Копируем бинарь и библиотеки ---

install -m755 "$BUILD_DIR/$BIN_NAME" "$PKG_DIR/usr/bin/$BIN_NAME"

# Важно сохранить симлинки версий, если они есть

# Копируем все *.so и *.so.* как есть

shopt -s nullglob
for f in "$LIBD_DIR"/*.so "$LIBD_DIR"/*.so.*; do

# cp -a сохраняет симлинки и права; при необходимости поправим права ниже

cp -a "$f" "$PKG_DIR/usr/lib/$TRIPLET/"
done
shopt -u nullglob

# Приводим права для реальных файлов .so (симлинки не трогаем)

find "$PKG_DIR/usr/lib/$TRIPLET" -type f -name "*.so*" -exec chmod 755 {} +

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