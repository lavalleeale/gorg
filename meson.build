project(
  'gorg',
  'cpp',
  version: '0.4.2',
  default_options: ['cpp_std=c++17', 'warning_level=3', 'buildtype=release', 'b_lto=true'],
)

# Dependencies
gtkmm_dep = dependency('gtkmm-3.0')
curl_dep = dependency('libcurl')

# Source files
src_files = files(
  './TransparentWindow.cpp',
  './finder.cpp',
  './gorg.cpp',
  './main.cpp',
  './pluginLoader.cpp',
  './plugins/ai.cpp',
  './plugins/dmenu.cpp',
  './plugins/drun.cpp',
  './plugins/equation.cpp',
  './plugins/run.cpp',
  './plugins/web.cpp',
  './settings.cpp',
)

# Include directories
inc_dirs = include_directories('include')

# Executable
executable(
  'gorg',
  sources: src_files,
  include_directories: inc_dirs,
  dependencies: [gtkmm_dep, curl_dep],
  cpp_pch: './pch/gorg_pch.h',
  install: true,
)