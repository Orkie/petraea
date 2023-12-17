project('petraea', 'c',
  version : '1.0.0',
  license : 'MIT',
  default_options : 'optimization=0')

sljit_dep = dependency('sljit')
lightning_dep = dependency('lightning')

inc = include_directories('include')

#################################################
# Sources
#################################################

petraea_sources = [
  'source/cpu.c',
  'source/decoder.c',
  'source/executor.c',
  'source/util.c',
  'source/emitter-sljit.c',
  'source/codecache.c'
]

test_sources = [
  'test/cpu.test.c',
  'test/decoder.test.c',
  'test/executor.test.c',
  'test/util.test.c',
  'test/jit.test.c',
  'test/codecache.test.c'
]

#################################################
# Library
#################################################

install_headers([
  'include/cpu.h',
  'include/instr.h'
])

libpetraea = shared_library(
  'petraea',
  petraea_sources,
  include_directories : inc,
  dependencies : [lightning_dep, sljit_dep],
  install : true)

libpetraea_static = static_library(
  'petraea',
  petraea_sources,
  include_directories : inc,
  dependencies : [lightning_dep, sljit_dep],
  install : true)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : libpetraea,
                 version : '1.0',
                 name : 'libpetraea',
                 filebase : 'petraea',
                 description : 'An ARM emulator core.')

libpetraea_dep = declare_dependency(
  include_directories : inc,
  link_with : libpetraea)

#################################################
# Unit tests
#################################################

criterion_dep = dependency('criterion')

testexe = executable('petraea.test', test_sources,
                     include_directories : inc,
		     dependencies : [criterion_dep, sljit_dep, lightning_dep],
		     link_with : libpetraea)
test('petraea test', testexe)

