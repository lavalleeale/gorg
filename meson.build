project('gorg', 'cpp',
  version : '0.2',
  default_options : [
    'cpp_std=c++17',
    'warning_level=3',
    'buildtype=release'
  ])

# Dependencies
gtkmm_dep = dependency('gtkmm-3.0')
curl_dep = dependency('libcurl')

# Source files
src_files = files(
    './main.cpp',
    './plugins/chat.cpp',
    './plugins/drun.cpp',
    './plugins/equation.cpp',
    './plugins/run.cpp',
    './plugins/web.cpp',
    './plugins/dmenu.cpp',
    './TransparentWindow.cpp',
    './settings.cpp',
    './finder.cpp',
    './gorg.cpp',
)

# Include directories
inc_dirs = include_directories('include')

# Executable
executable('gorg',
  sources: src_files,
  include_directories: inc_dirs,
  dependencies: [gtkmm_dep, curl_dep],
  install: true)
