project('sljit', 'c',
  version : '1.0.0',
  default_options : 'optimization=2')

add_project_arguments('-DSLJIT_VERBOSE', language : 'c')

inc = include_directories('sljit_src')

#################################################
# Sources
#################################################

sljit_sources = [
  'sljit_src/sljitLir.c'
]

#################################################
# Library
#################################################

install_headers([
  'sljit_src/sljitConfig.h'
])

sljit = shared_library(
  'sljit',
  sljit_sources,
  include_directories : inc,
  install : true)

sljit_static = static_library(
  'sljit',
  sljit_sources,
  include_directories : inc,
  install : true)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : sljit,
                 version : '1.0',
                 name : 'sljit',
                 filebase : 'sljit',
                 description : 'Stackless JIT library')

sljit_dep = declare_dependency(
  include_directories : inc,
  link_with : sljit)


