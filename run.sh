#!/bin/bash
# Скрипт для запуска Qt приложения с правильным окружением

# Очищаем LD_LIBRARY_PATH от snap путей и устанавливаем только системные
export LD_LIBRARY_PATH="/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:/usr/local/lib"

# Устанавливаем путь к Qt плагинам
export QT_QPA_PLATFORM_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt6/plugins:/usr/lib/x86_64-linux-gnu/qt5/plugins"

# Запускаем приложение
cd "$(dirname "$0")"
exec ./build/PsychologicalTests "$@"


