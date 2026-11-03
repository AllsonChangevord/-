# Wayland connect

Минимальный клиент на C: подключается к сокету Wayland-композитора
через `$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY` (по умолчанию `wayland-0`).

Писалось своими ручками и знаниями, мой первый проект судите строго. 

Сборка и запуск (нужен запущенный Wayland-сеанс):

    gcc -Wall -Wextra -O2 -o wl_connect wl_connect.c
    ./wl_connect
