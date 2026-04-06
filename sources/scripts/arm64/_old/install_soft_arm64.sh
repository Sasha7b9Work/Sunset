#!/usr/bin/env bash
# Скрипт установки для Orange Pi 5 Plus (ARM64/aarch64)
# Ubuntu 24.04 с KDE Neon Plasma

set -e    # Stop script when an error occurs

echo "Установка зависимостей для Orange Pi 5 Plus (ARM64)..."
echo "Архитектура: $(uname -m)"

# Обновление системы
sudo apt-get update
sudo apt-get -y upgrade

# Основные инструменты разработки
sudo apt-get -y install nginx htop openssh-server openssh-client iptables-persistent mc links
sudo apt-get -y install cmake gcc g++ make python3 python3-pip python3-venv build-essential gdb

# Библиотеки разработки
sudo apt-get -y install libssl-dev libx11-dev libxrandr-dev mesa-common-dev libgl1-mesa-dev libasound2-dev

# GTK3 и зависимости wxWidgets для ARM64
sudo apt-get install libgtk-3-dev -y
sudo apt-get install curl cmake -y
sudo apt install libcurl4-openssl-dev -y
sudo apt-get install libsecret-1-dev -y

# Дополнительные пакеты для корректного GUI отображения на ARM64
echo "Установка дополнительных GUI библиотек для ARM64..."
sudo apt-get install -y libcairo2-dev libpango1.0-dev libatk1.0-dev libgdk-pixbuf2.0-dev
sudo apt-get install -y libxinerama-dev libxcursor-dev libxi-dev libxcomposite-dev
sudo apt-get install -y libjpeg-dev libpng-dev libtiff-dev libsm-dev
sudo apt-get install -y libexpat1-dev zlib1g-dev libpcre2-dev

# Cairo для векторной графики
sudo apt-get install -y libcairo-gobject2 libcairo-script-interpreter2

# Fonts для корректного отображения текста
sudo apt-get install -y fonts-liberation fonts-dejavu-core fonts-noto-core
sudo apt-get install -y fontconfig libfontconfig1-dev

# Wayland поддержка (если используется)
sudo apt-get install -y libwayland-dev libwayland-egl1-mesa libxkbcommon-dev

# Установка WebKit с проверкой версии (Ubuntu 24.04 использует 4.1)
echo "Попытка установки WebKit2GTK..."
if apt-cache show libwebkit2gtk-4.1-dev >/dev/null 2>&1; then
    echo "Устанавливаем libwebkit2gtk-4.1-dev (Ubuntu 24.04+)"
    sudo apt-get install libwebkit2gtk-4.1-dev -y
elif apt-cache show libwebkit2gtk-4.0-dev >/dev/null 2>&1; then
    echo "Устанавливаем libwebkit2gtk-4.0-dev (старые версии)"
    sudo apt-get install libwebkit2gtk-4.0-dev -y
else
    echo "Предупреждение: Пакет WebKit2GTK не найден. Продолжаем без него..."
fi

# GStreamer для мультимедиа
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio -y

# SDL2 для работы с аудио/видео
sudo apt-get install libsdl2-2.0-0 -y
sudo apt-get install libsdl2-dev -y

# Системные уведомления
sudo apt-get install libnotify-dev -y

# GPIO библиотека для SPI управления
echo "Установка libgpiod для GPIO управления..."
sudo apt-get install libgpiod-dev gpiod -y

# Проверяем установку libgpiod
echo "Проверка установки libgpiod..."
if pkg-config --exists libgpiod; then
    echo "✅ libgpiod успешно установлена"
    echo "Версия: $(pkg-config --modversion libgpiod)"
    echo "Флаги компиляции: $(pkg-config --cflags libgpiod)"
    echo "Флаги линковки: $(pkg-config --libs libgpiod)"
else
    echo "❌ Ошибка: libgpiod не найдена"
fi

# Проверяем доступность GPIO устройств
echo "Проверка GPIO устройств..."
if ls /dev/gpiochip* > /dev/null 2>&1; then
    echo "✅ GPIO устройства найдены:"
    ls -la /dev/gpiochip*
else
    echo "⚠️  GPIO устройства не найдены"
    echo "Возможно, нужно загрузить драйвер GPIO или включить его в настройках ядра"
fi

# Проверяем утилиты GPIO
echo "Проверка утилит GPIO..."
if command -v gpiodetect > /dev/null 2>&1; then
    echo "✅ Утилиты GPIO доступны"
    echo "Доступные GPIO чипы:"
    gpiodetect
else
    echo "❌ Утилиты GPIO не найдены"
fi

# OpenGL
sudo apt install libglu1-mesa-dev freeglut3-dev -y

# Инструменты для создания deb-пакетов
sudo apt-get install -y dh-make devscripts dpkg debconf debhelper lintian

# Дополнительные ARM64-специфичные библиотеки
echo "Проверка ARM64 библиотек..."
ls -la /usr/lib/aarch64-linux-gnu/libX11* || echo "Предупреждение: libX11 не найдена в стандартном ARM64 пути"
ls -la /usr/lib/aarch64-linux-gnu/libxkbcommon* || echo "Предупреждение: libxkbcommon не найдена в стандартном ARM64 пути"
ls -la /usr/lib/aarch64-linux-gnu/libpng* || echo "Предупреждение: libpng не найдена в стандартном ARM64 пути"

# Проверка GUI библиотек
echo "Проверка GUI библиотек для ARM64..."
pkg-config --exists gtk+-3.0 && echo "✅ GTK3 найден: $(pkg-config --modversion gtk+-3.0)" || echo "❌ GTK3 не найден"
pkg-config --exists cairo && echo "✅ Cairo найден: $(pkg-config --modversion cairo)" || echo "❌ Cairo не найден"
pkg-config --exists x11 && echo "✅ X11 найден: $(pkg-config --modversion x11)" || echo "❌ X11 не найден"

# Проверка дисплея
echo "Проверка графического окружения..."
echo "DISPLAY: ${DISPLAY:-не установлен}"
echo "WAYLAND_DISPLAY: ${WAYLAND_DISPLAY:-не установлен}"

# Тест простого GUI приложения
if command -v xeyes >/dev/null 2>&1; then
    echo "✅ xeyes доступен для тестирования GUI"
else
    echo "Установка xeyes для тестирования GUI..."
    sudo apt-get install -y x11-apps
fi

echo "Установка завершена для архитектуры: $(uname -m)"
echo "Теперь можно запустить сборку wxWidgets и основного проекта"
