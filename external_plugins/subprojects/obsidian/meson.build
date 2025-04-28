project(
    'obsidian_plugin',
    'cpp',
    version: '0.0.1',
    default_options: ['cpp_std=c++17', 'warning_level=3', 'buildtype=release'],
)

# Define the library
default_target = library(
    'obsidian_plugin',
    ['obsidian.cpp'],
    include_directories: include_directories('../../../include'),
    dependencies: dependency('gtkmm-3.0'),
)
