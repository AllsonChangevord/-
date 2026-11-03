#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

static int wayland_display_connect(void) {
    const char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == NULL) {
        fprintf(stderr, "XDG_RUNTIME_DIR is not set\n");
        return -EINVAL;
    }

    const char *wayland_display = getenv("WAYLAND_DISPLAY");
    if (wayland_display == NULL) {
        wayland_display = "wayland-0";
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    // Безопасное формирование пути к сокету в XDG_RUNTIME_DIR
    int ret = snprintf(addr.sun_path, sizeof(addr.sun_path), "%s/%s", xdg_runtime_dir, wayland_display);
    if (ret < 0 || (size_t)ret >= sizeof(addr.sun_path)) {
        fprintf(stderr, "Wayland socket path is too long\n");
        return -ENAMETOOLONG;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -errno;
    }

    // Правильный расчет длины адреса для Unix-сокета
    socklen_t addr_len = sizeof(sa_family_t) + strlen(addr.sun_path);

    if (connect(fd, (struct sockaddr *)&addr, addr_len) == -1) {
        perror("connect to Wayland");
        close(fd);
        return -errno;
    }

    printf("Connected to Wayland socket! fd = %d\n", fd);
    return fd;
}

int main(void) {
    int fd = wayland_display_connect();
    if (fd >= 0) {
        close(fd);
    }
    return 0;
}
