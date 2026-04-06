#!/bin/bash

# Скрипт для запуска GUI приложения Linia2 с поддержкой дисплея на ARM64

# Создаем лог-файл с временной меткой
LOG_FILE="$(dirname "$0")/run_arm64_$(date +%Y%m%d_%H%M%S).log"
exec > >(tee -a "$LOG_FILE") 2>&1

echo "=== $(date '+%Y-%m-%d %H:%M:%S') - Запуск Linia2 на ARM64 ==="
echo "=== Настройка окружения для ARM64 GUI приложения ==="
echo "Системная информация:"
echo "  ОС: $(uname -s)"
echo "  Архитектура: $(uname -m)"
echo "  Ядро: $(uname -r)"
echo "  Пользователь: $(whoami)"
echo "  Рабочая директория: $(pwd)"
echo ""

# Проверяем, запущена ли система с GUI
echo "$(date '+%H:%M:%S') - Проверяем GUI окружение..."
if [ -z "$DISPLAY" ]; then
    echo "$(date '+%H:%M:%S') - ⚠️  DISPLAY не установлен. Пытаемся настроить для ARM64..."
    
    # Попробуем найти активную X11 сессию
    DISPLAY_NUM=$(ps aux | grep Xorg | grep -v grep | head -1 | sed 's/.*:\([0-9]\+\).*/\1/')
    
    if [ ! -z "$DISPLAY_NUM" ]; then
        export DISPLAY=:$DISPLAY_NUM
        echo "$(date '+%H:%M:%S') - ✅ Установлен DISPLAY=:$DISPLAY_NUM"
    else
        echo "$(date '+%H:%M:%S') - ❌ Активная X11 сессия не найдена"
        echo "Возможные решения для ARM64:"
        echo "1. Запустите скрипт из графического терминала"
        echo "2. Подключитесь через SSH с флагом -X: ssh -X user@orangepi"
        echo "3. Используйте VNC для удаленного доступа к рабочему столу"
        echo "4. Запустите виртуальный X сервер: sudo apt install xvfb && export DISPLAY=:99 && Xvfb :99 -screen 0 1024x768x16 &"
        echo "$(date '+%H:%M:%S') - Скрипт завершается с ошибкой"
        exit 1
    fi
fi

echo "$(date '+%H:%M:%S') - ✅ DISPLAY установлен: $DISPLAY"

# Настройка специфичных переменных для ARM64 GUI
echo "$(date '+%H:%M:%S') - Настройка ARM64 специфичных переменных..."
export GDK_BACKEND=x11
export LIBGL_ALWAYS_SOFTWARE=1
export MESA_GL_VERSION_OVERRIDE=3.3
export WX_GRAPHICS_RENDERER=cairo
export GTK_THEME=Chicago95

echo "$(date '+%H:%M:%S') - ARM64 переменные установлены:"
echo "  GDK_BACKEND: $GDK_BACKEND"
echo "  LIBGL_ALWAYS_SOFTWARE: $LIBGL_ALWAYS_SOFTWARE"
echo "  WX_GRAPHICS_RENDERER: $WX_GRAPHICS_RENDERER"

# Проверяем доступность X сервера
echo "$(date '+%H:%M:%S') - Проверяем доступность X сервера..."
if ! xset q &>/dev/null; then
    echo "$(date '+%H:%M:%S') - ❌ X сервер недоступен"
    echo "Возможные решения для ARM64:"
    echo "1. Убедитесь, что графическая оболочка запущена"
    echo "2. Проверьте права доступа к X сессии: xhost +local:"
    echo "3. Для SSH подключения используйте: ssh -X или ssh -Y"
    echo "$(date '+%H:%M:%S') - Скрипт завершается с ошибкой"
    exit 1
fi

echo "$(date '+%H:%M:%S') - ✅ X сервер доступен"

# Переходим в папку с исполняемым файлом
echo "$(date '+%H:%M:%S') - Переходим в папку с исполняемым файлом..."
cd "$(dirname "$0")/../../generated/Linia2" || {
    echo "$(date '+%H:%M:%S') - ❌ Не найдена папка с исполняемым файлом: ../../generated/Linia2"
    echo "$(date '+%H:%M:%S') - Скрипт завершается с ошибкой"
    exit 1
}

# Проверяем наличие исполняемого файла
echo "$(date '+%H:%M:%S') - Проверяем наличие исполняемого файла..."
if [ ! -f "./Linia2" ]; then
    echo "$(date '+%H:%M:%S') - ❌ Исполняемый файл Linia2 не найден"
    echo "Сначала выполните сборку проекта: ./full_build_arm64.sh"
    echo "$(date '+%H:%M:%S') - Скрипт завершается с ошибкой"
    exit 1
fi

# Проверяем наличие конфигурационного файла
echo "$(date '+%H:%M:%S') - Проверяем наличие конфигурационного файла..."
if [ ! -f "CIC.loc" ] && [ -f "../../../files/Linia.loc" ]; then
    echo "$(date '+%H:%M:%S') - 📁 Копируем конфигурационный файл..."
    cp "../../../files/Linia.loc" "./CIC.loc"
fi

echo "$(date '+%H:%M:%S') - 🚀 Запускаем приложение Linia2..."
echo "Исполняемый файл: $(pwd)/Linia2"
echo "DISPLAY: $DISPLAY"

# Запускаем приложение
echo "$(date '+%H:%M:%S') - Начало выполнения приложения..."
./Linia2
APP_EXIT_CODE=$?

echo "$(date '+%H:%M:%S') - 🏁 Приложение завершено с кодом: $APP_EXIT_CODE"
echo ""
echo "=== $(date '+%Y-%m-%d %H:%M:%S') - Конец выполнения скрипта ==="
echo "Лог сохранен в файл: $LOG_FILE"

exit $APP_EXIT_CODE
