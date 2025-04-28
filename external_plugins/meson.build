project(
    'sample_plugin',
    'cpp',
    version: '0.0.1',
    default_options: ['cpp_std=c++17', 'warning_level=3', 'buildtype=release'],
)

# Define the include directory located in the parent directory
parent_inc = include_directories('../include')

# Define the source file(s)
sources = ['sample.cpp']

# Define the library
library(
    'sample_plugin',
    sources,
    include_directories: parent_inc,
    dependencies: [dependency('gtkmm-3.0')],
)