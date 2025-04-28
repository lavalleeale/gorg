project('external_plugins', 'c')

subproj_names = ['sample', 'obsidian']
all_tgts = []

foreach n : subproj_names
  sp          = subproject(n)
  default_tgt = sp.get_variable('default_target', [])
  all_tgts   += default_tgt
endforeach

alias_target('everything', all_tgts)
