env G_DEBUG=gc-friendly G_SLICE=always-malloc valgrind --tool=memcheck --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp --suppressions=./additional.supp --log-file=vg-dump --leak-check=full --leak-resolution=high --num-callers=40 --show-leak-kinds=definite ./build/Lupus
