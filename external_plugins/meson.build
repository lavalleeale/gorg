project(
  'external_plugins',
  'c',
  version: '0.1.0',
  default_options: ['cpp_std=c++20', 'warning_level=3', 'buildtype=release'],
)

subproj_names = ['sample', 'obsidian', 'wallpaper']
all_tgts = []

foreach n : subproj_names
  sp = subproject(n)
  default_tgt = sp.get_variable('default_target', [])
  all_tgts += default_tgt
endforeach

alias_target('everything', all_tgts)
