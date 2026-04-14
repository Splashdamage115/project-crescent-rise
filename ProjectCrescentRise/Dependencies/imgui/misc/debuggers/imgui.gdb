# GDB configuration to aid debugging experience

# To enable these customizations edit $HOME/.gdbinit (or ./.gdbinit if local gdbinit is enabled) and add:
#   add-auto-load-safe-path /path/to/imgui.gdb
#   source /path/to/imgui.gdb
#
# More Information at:
#   * https:
#   * https:

# Disable stepping into trivial functions
skip -rfunction Im(Vec2|Vec4|Strv|Vector|Span)::.+
